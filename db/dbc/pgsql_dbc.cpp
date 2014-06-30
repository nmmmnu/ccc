/*=================================
Post Gre SQL 95 Data Base Connector
===================================
Remark:   Hey all, this is one of two my favorite RDBMS !!!!!!
          (: Second is MySQL :)))))

Author:   Nikolay Mijaylov Mijaylov
CopyLeft: 11.1998, Sofia

Remark:   Many RDBMS support same header file as PostGre, so you
          can change only native lib path... (and diver name :)
*/

#include "a_dbc.h"

#include <stdlib.h>
#include <libpq-fe.h>   //PostGres native lib

#define DBC_HOST		"DATABASEHOST"
#define DBC_PORT		"DATABASEPORT"
#define DBC_DATABASE		"DATABASE"
#define DBC_DATABASEUSER	"DATABASEUSER"
#define DBC_DATABASEPASS	"DATABASEPASS"

class PGSQLDBConnection: public ADBConnection{
public:
   PGconn *conn;
   
   PGSQLDBConnection():conn(NULL){};

   boolean connect(Properties *cfg);
   void disconnect();
   
   ADBC *getADBC();
};

const char *a_dbc_drivername         = "Postgre SQL (95)";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class PGSQLDBC: public ADBC{
protected:
   PGresult *res;
   int      rownum;
public:
    PGSQLDBC(ADBConnection *DBConnection):ADBC(DBConnection),
                                          res(NULL),
                                          rownum(0){};
    void setSQL(const char *sql);

    boolean exec();
    boolean open();
    void close();

    void next();
    boolean eof();

    unsigned int getFieldCount();
    const char *getFieldName(unsigned int no);
    //const char getFieldType(unsigned int no);
    const char *getFieldData(unsigned int no);
   
    long int getAffectedCount();
    long int getRecordCount();

    boolean gotoRecordNum(unsigned long int);
};

ADBConnection *getADBConnection(){ return new PGSQLDBConnection(); };

ADBC *PGSQLDBConnection::getADBC(){ return new PGSQLDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean PGSQLDBConnection::connect(Properties *cfg){
   conn = PQsetdbLogin(cfg->get(DBC_HOST),
                  cfg->get(DBC_PORT),
                  NULL,                    //pgoptions?!?!
                  NULL,                    //pgtty!?!?
                  cfg->get(DBC_DATABASE),
		  cfg->get(DBC_DATABASEUSER),   //Not a problem if
		  cfg->get(DBC_DATABASEPASS)    //they are NULL
                  );
   if (PQstatus(conn) == CONNECTION_BAD){
      PQfinish(conn);
      conn = NULL;
   }
   
   return conn != NULL;
};

void PGSQLDBConnection::disconnect(){
   PQfinish(conn);
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define CONN ((PGSQLDBConnection *) DBConnection)->conn

void PGSQLDBC::setSQL(const char *sql){
   if (!isempty(sql)){
      char *sql1 = strdup(sql); //Because sql is const :)
      res = PQexec(CONN, sql);
      free(sql1);
   }
   //printf(">>>%s<<<\n", sql);
};

boolean PGSQLDBC::exec(){
   if (res == NULL)
      return false;

   boolean b = ( PQresultStatus(res) != PGRES_BAD_RESPONSE   ) &&
               ( PQresultStatus(res) != PGRES_NONFATAL_ERROR ) &&
               ( PQresultStatus(res) != PGRES_FATAL_ERROR    ) &&
               ( PQresultStatus(res) != PGRES_EMPTY_QUERY    ); 
                                                //This last ?!?!?
   PQclear(res);
   return b;
};

boolean PGSQLDBC::open(){
   if (res == NULL)
      return false;

   return PQresultStatus(res) == PGRES_TUPLES_OK;
};

void PGSQLDBC::close(){
   PQclear(res);
};

void PGSQLDBC::next(){
   if (!eof())
      rownum++;
};

boolean PGSQLDBC::eof(){
   return rownum == getRecordCount();
};

unsigned int PGSQLDBC::getFieldCount(){
   return (unsigned int) PQnfields(res);
};

const char *PGSQLDBC::getFieldName(unsigned int no){
   return PQfname(res,no);
};

const char *PGSQLDBC::getFieldData(unsigned int no){
   return PQgetvalue(res, rownum, no);
};

long int PGSQLDBC::getAffectedCount(){
   return atol(PQcmdTuples(res));
};

long int PGSQLDBC::getRecordCount(){
   return PQntuples(res);
};

boolean PGSQLDBC::gotoRecordNum(unsigned long int num){
   rownum = num;
   return true;
};
