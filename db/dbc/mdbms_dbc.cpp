/*==========================
MDBMS Data Base Connector II
============================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     1999, Sofia
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/

Remarks:
   o MDBMS API ARE NOT STABLE FOR NOW, may be!?!?!?!?
   o I'm not sure if I get field names correctly :)))
*/

#include "a_dbc.h"

#include <libdbms.h> //MDBMS native library

#define DBC_HOST     "DATABASEHOST"
#define DBC_PORT     "DATABASEPORT"
#define DBC_DATABASE "DATABASE"
#define DBC_USER     "DATABASEUSER"
#define DBC_PASS     "DATABASEPASS"

class MDBMSDBConnection: public ADBConnection{
public:
   db_session *session;
   
   //~MDBMSDBConnection(){};
   boolean connect(Properties *cfg);
   void disconnect();
   
   ADBC *getADBC();
};

const char *a_dbc_drivername         = "Hint Tech MDBMS";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class MDBMSDBC: public ADBC{
private:
   int     res;
   //record  header;
   record *rec;
public:
   MDBMSDBC(ADBConnection *DBConnection):ADBC(DBConnection), 
                                         rec(NULL){};
   ~MDBMSDBC(){};

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
};

ADBConnection *getADBConnection(){ return new MDBMSDBConnection(); };
ADBC *MDBMSDBConnection::getADBC(){ return new MDBMSDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define SESSION ((MDBMSDBConnection *) DBConnection)->session

boolean MDBMSDBConnection::connect(Properties *cfg){
   char *tmp;
   char *tmp1;

   tmp=strdupn(cfg->get(DBC_HOST));
   db_sethostname(tmp);
   free(tmp);

   if (cfg->getAsInt(DBC_PORT, 0) != 0)
      db_setport(cfg->getAsInt(DBC_PORT));
   
   session=db_open_session();

   if (cfg->get(DBC_USER) != 0){
      tmp  = strdupn(cfg->get(DBC_USER));
      tmp1 = strdupn(cfg->get(DBC_PASS));
      boolean b = true;
      
      if (db_set_global_authentication(tmp, tmp1) != 0)
         b = false;
	 
      free(tmp);
      free(tmp1);
      
      if (!b)
         return false;
   }
   
   tmp = strdupn(cfg->get(DBC_DATABASE));
   boolean b = true;
   if (db_open_database(session, tmp) != 0)
         b = false;
   free(tmp);
   if (!b)
      return false;
   
   return true;
};

void MDBMSDBConnection::disconnect(){
   db_close_session(session);
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void MDBMSDBC::setSQL(const char *sql){
   char *tmp = strdupn(sql);
   res = db_exec_sql(SESSION, 1, "%s", tmp);
   free(tmp);
};

boolean MDBMSDBC::exec(){
   return res == 0;
};

boolean MDBMSDBC::open(){
/*
printf("\n\n%d\n\n",res);
if (SESSION->currentheader == NULL)
printf("\n\nheader is NULL\n\n");
*/
   if (res != 0) 
      return false;
   else if (SESSION->currentheader == NULL)
      return false;
   else
      next();
   
   return true;
};

void MDBMSDBC::close(){};

void MDBMSDBC::next(){
   if (rec)
      db_free_record(rec);
   rec=db_get_next_record(SESSION);
};

boolean MDBMSDBC::eof(){
   return rec == NULL;
};

unsigned int MDBMSDBC::getFieldCount(){
   return (unsigned int) SESSION->currentheader->nvalues;
};

const char *MDBMSDBC::getFieldName(unsigned int no){
   char val[1000];  //1000 is getted from MDBMS demo code...
   sprintf(val, SESSION->currentheader->values[no].value.string);
   return val;
};

const char *MDBMSDBC::getFieldData(unsigned int no){
   char val[1000];  //1000 is getted from MDBMS demo code...
   db_sprintvalue(rec->values+no, val);
   return val;
};
