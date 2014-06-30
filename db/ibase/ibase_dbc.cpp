/*======================================
Borland InterBase Data Base Connector II
========================================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     1999, Sofia
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/
*/
/*
I personally think that InterBase API are not that flexible,
I think they are vary, very dark and `spagetty' :)))
If you found a BUG, please, please tell me...

Compilled on:
   o Interbase 4.0 Workgroup Server - Linux (Suppose any UNIX will works)
   
Tested with:
   o Interbase 4.0 Workgroup Server - Linux (Suppose any UNIX will works)
   o Interbase 5.0 - Windows (free 5 users license from C++Builder)

Based on Borland's `apifull.c' demo...
*/

#include "a_dbc.h"

#define DBC_DATABASE "DATABASE"
#define DBC_USER     "DATABASEUSER"
#define DBC_PASS     "DATABASEPASS"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

//InterBase include file
#include <ibase.h>

//Some multi-platform definitions from Borland
#include "ibase/align.h"

//Some Win_32 definitions from Borland
#include "ibase/example.h"

//Maximum data len
#define    MAXLEN    1024

//VarChar Structure
typedef struct vary{
   short vary_length;
   char  vary_string[1];
} VARY;


class IBaseDBConnection: public ADBConnection{
protected:
   long               status[20];   //Status Vector
   
   char *uid;
   char *upw;   
public:
   isc_db_handle      db;           //db Handle
   
   IBaseDBConnection():uid(NULL),
                       upw(NULL){};
   ~IBaseDBConnection(){
      free(uid);
      free(upw);
   };
   boolean connect(Properties *cfg);
   void disconnect();
   
   ADBC *getADBC();
};

const char *a_dbc_drivername         = "Borland InterBase ver 4.0 (UNIX/Win_32)";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class IBaseDBC: public ADBC{
private:
   isc_tr_handle trans;      //Transaction Handle
   short   num_cols;         //Column count
   long    fetch_stat;       //EOF const
   long    status[20];       //Status Vector
   XSQLDA  ISC_FAR *sqlda;   //Dynamic SQL Field Descroptor
   isc_stmt_handle stmt;     //IBase statement
   boolean ret;              //OK flag
   
   char *get_column (ARG(XSQLVAR ISC_FAR *, var)) ARGLIST(XSQLVAR *var);
public:
   IBaseDBC(ADBConnection *DBConnection):ADBC(DBConnection){
      /* 
       *    Allocate enough space for 20 fields.  
       *    If more fields get selected, re-allocate SQLDA later.
       */
      sqlda = (XSQLDA ISC_FAR *) malloc(XSQLDA_LENGTH (20));
      sqlda->sqln = 20;
      sqlda->version = 1;  //SQLDA version
      
      trans = NULL;
   };
   
   ~IBaseDBC(){
      if (trans)
         isc_commit_transaction(status, &trans);

      free(sqlda);
   };

   void setSQL(const char *sql);

   boolean exec();
   boolean open();
   void close(){};  //No need
   
   void next();
   boolean eof();
   
   unsigned int getFieldCount();
   const char *getFieldName(unsigned int no);
   //const char getFieldType(unsigned int no);
   const char *getFieldData(unsigned int no);
};

ADBConnection *getADBConnection(){ return new IBaseDBConnection(); };
ADBC *IBaseDBConnection::getADBC(){ return new IBaseDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean IBaseDBConnection::connect(Properties *cfg){
   db    = NULL;
   char *tmp;
   
   setenv("ISC_USER",     cfg->get(DBC_USER), uid);
   setenv("ISC_PASSWORD", cfg->get(DBC_PASS), upw);
   
   tmp = strdupn( cfg->get(DBC_DATABASE) );
   return !( isc_attach_database(status,
                                 0,
                                 tmp,
                                 &db,
                                 0,
                                 NULL) );
   free(tmp);
};

void IBaseDBConnection::disconnect(){
   isc_detach_database(status, &db);
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define DB ((IBaseDBConnection *) DBConnection)->db

void IBaseDBC::setSQL(const char *sql){
   ret = true;
   /* Allocate a global statement */
   if (isc_dsql_allocate_statement(status, &DB, &stmt)){
      ret = false;
      return;
   }
   
   if (isc_start_transaction(status, &trans, 1, &DB, 0, NULL)){
      ret = false;
      return;
   }

   char *tmp = strdup(sql);
   if (isc_dsql_prepare(status, &trans, &stmt, 0, tmp, 1, sqlda))
      ret = false;

   free(tmp);
};


boolean IBaseDBC::exec(){
   if (!ret)
      return false;

   if (isc_dsql_execute(status, &trans, &stmt, 1, NULL))
      return false;
      
    /* Commit DDL statements if that is what sql_info says */
    if (isc_commit_transaction(status, &trans))
       return false;

    return true;
};

boolean IBaseDBC::open(){
   if (!ret)
      return false;

   //Borland's code, very spagetty:)))
   
   num_cols = sqlda->sqld;

   /* Need more room. */
   if (sqlda->sqln < num_cols){
      sqlda = (XSQLDA ISC_FAR *) realloc(sqlda, XSQLDA_LENGTH (num_cols));
      sqlda->sqln = num_cols;
      sqlda->version = 1;
      if (isc_dsql_describe(status, &stmt, 1, sqlda))
         return false;

      num_cols = sqlda->sqld;
   }

   /*
    *     Set up SQLDA.
    */
   long buffer[MAXLEN];
   XSQLVAR ISC_FAR *var;
   short length, alignment, type, offset, i;
   
   for (var = sqlda->sqlvar, offset = 0, i = 0;   i < num_cols;   var++, i++){
      length = alignment = var->sqllen;
      type = var->sqltype & ~1;

      if (type == SQL_TEXT)
         alignment = 1;
      else if (type == SQL_VARYING){   
         length += sizeof (short);
         alignment = sizeof (short);
      }
      /*  RISC machines are finicky about word alignment
       *  So the output buffer values must be placed on
       *  word boundaries where appropriate
       */
      
      offset = ALIGN (offset, alignment);
      var->sqldata = (char ISC_FAR *) buffer + offset;
      offset += length;
      offset = ALIGN (offset, sizeof (short));
      var->sqlind = (short*) ((char ISC_FAR *) buffer + offset);
      offset += sizeof (short);   
   }

   if (isc_dsql_execute(status, &trans, &stmt, 1, NULL))
      return false;
      
   next();
   return true;
};

void IBaseDBC::next(){
   fetch_stat = isc_dsql_fetch(status, &stmt, 1, sqlda);
};

boolean IBaseDBC::eof(){
   return fetch_stat != 0;
};

unsigned int IBaseDBC::getFieldCount(){
   return (unsigned int) num_cols;
};

const char *IBaseDBC::getFieldName(unsigned int no){
   //'sqlname' is not NULL terminating...
   return sqlda->sqlvar[no].sqlname;
};

const char *IBaseDBC::getFieldData(unsigned int no){
   return get_column((XSQLVAR ISC_FAR *) &sqlda->sqlvar[(int) no]);
};

//Borland code, very spagetty :)))
//Don't shut the programmer, he programming how he CAN :)))
char *IBaseDBC::get_column (ARG(XSQLVAR ISC_FAR *, var)) ARGLIST(XSQLVAR *var)
{
    short       dtype;
    static char data[MAXLEN]; //My change :)))
    static char *p;           //My change :)))
    char        date_s[20];
    VARY        *vary;
    short       len; struct tm    times;
    float       numeric;
    short       factor, i;

    dtype = var->sqltype & ~1;
    p = data;

    /* Null handling.  If the column is nullable and null */
    if ((var->sqltype & 1) && (*var->sqlind < 0))
    {
        switch (dtype)
        {
            case SQL_TEXT:
            case SQL_VARYING: len = var->sqllen;  break;
            case SQL_SHORT:   len = 6;            break;
            case SQL_LONG:    len = 10;           break;
            case SQL_FLOAT:
            case SQL_DOUBLE:  len = 22;           break;
            case SQL_DATE:    len = 10;           break;
            case SQL_BLOB:
            case SQL_ARRAY:
            default:          len = 17;           break;
        }

        sprintf(p, "%s", "");
    }
    else
    {
        switch (dtype)
        {
            case SQL_TEXT:
                sprintf(p, "%s", var->sqldata);
                break;

            case SQL_VARYING:
                vary = (VARY*) var->sqldata;
                vary->vary_string[vary->vary_length] = '\0';
                sprintf(p, "%-*s", var->sqllen, vary->vary_string);
                break;

            case SQL_SHORT:
                sprintf(p, "%d", *(short ISC_FAR *) (var->sqldata));
                break;

            case SQL_LONG:
                /* Numeric handling needs scale */
                if (var->sqlscale)
                {
                    factor = 1;
                    for (i = 1; i < -var->sqlscale; i++)
                        factor *= 10;
                    numeric = (float)*(long *) var->sqldata;
                    numeric /= factor;
                    sprintf(p, "%f", *(double ISC_FAR *) (var->sqldata));
                }
                else
                    sprintf(p, "%ld", *(long ISC_FAR *) (var->sqldata));
                break;

            case SQL_FLOAT:
                sprintf(p, "%f", *(float ISC_FAR *) (var->sqldata));
                break;

            case SQL_DOUBLE:
                /* Detect numeric/decimal scale and handle format */
                if (var->sqlscale)
                    sprintf(p, "%f", *(double ISC_FAR *) (var->sqldata));
                else
                    sprintf(p, "%f", *(double ISC_FAR *) (var->sqldata));
                break;

            case SQL_DATE:
                isc_decode_date((ISC_QUAD ISC_FAR *) var->sqldata, &times);
                sprintf(date_s, "%d/%d/%d",
                        times.tm_mday, times.tm_mon+1, times.tm_year);
                sprintf(p, "%*s", 10, date_s);
                break;

            case SQL_BLOB:
            case SQL_ARRAY:
            default:
                break;
        }
    }
    
   return p;
}
