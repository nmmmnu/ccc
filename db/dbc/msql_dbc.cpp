/*==========================
MSQL Data Base Connector II
============================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     1999, Sofia
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/
*/

#include "a_dbc.h"

#include <msql.h> //mSQL native library

#define DBC_HOST     "DATABASEHOST"
#define DBC_DATABASE "DATABASE"

class mSQLDBConnection: public ADBConnection{
public:
   int sock;
   int dbc_res;

   mSQLDBConnection():sock(-1){};

   boolean connect(Properties *cfg);
   void disconnect();

   ADBC *getADBC();
};

const char *a_dbc_drivername         = "Minerva MiniSQL";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class mSQLDBC: public ADBC{
protected:
   int               isok;
   m_result          *result;
   m_row             row;
   m_field           *field;
   long int          pos;
public:
   mSQLDBC(ADBConnection *DBConnection):ADBC(DBConnection),
                                     isok(-1),
                                     result(NULL){}
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
   
   long int getRecordCount();
   boolean gotoRecordNum(unsigned long int num);
};

ADBConnection *getADBConnection(){ return new mSQLDBConnection(); };
ADBC *mSQLDBConnection::getADBC(){ return new mSQLDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean mSQLDBConnection::connect(Properties *cfg){
   char *tmp = strdupn( cfg->get(DBC_HOST) );
   sock = msqlConnect(tmp);
   free(tmp);
   
   if (sock < 0)
      return false;

   tmp = strdupn( cfg->get(DBC_DATABASE) );
   if (msqlSelectDB(sock, tmp) < 0){
      //msqlClose(sock);
      sock = -1;
   }
   free(tmp);

   return sock >= 0;
};

void mSQLDBConnection::disconnect(){
   msqlClose(sock);
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void mSQLDBC::setSQL(const char *sql){
   if (!isempty(sql)){
      char *sql1 = strdup(sql); //Because sql is const :)

      isok = msqlQuery( ((mSQLDBConnection *) DBConnection)->sock, sql1);
         
      free(sql1);
   }
};

boolean mSQLDBC::exec(){
   return isok >= 0;
};

boolean mSQLDBC::open(){
   if (isok < 0)
      return false;
         
   result = msqlStoreResult();
   if (result)
      next();

   return ( result != NULL );
};

void mSQLDBC::close(){
   msqlFreeResult(result);
};

void mSQLDBC::next(){
   row = msqlFetchRow(result);
   pos++;
};

boolean mSQLDBC::eof(){
   if (result != NULL)
      return pos > getRecordCount();
   else
      return true;
};

unsigned int mSQLDBC::getFieldCount(){
   return msqlNumFields(result);
};

const char *mSQLDBC::getFieldName(unsigned int no){
   msqlFieldSeek(result, no);
   field = msqlFetchField(result);
   return field->name;
};

const char *mSQLDBC::getFieldData(unsigned int no){
   if (row)
      return row[no];
   return NULL;
};

long int mSQLDBC::getRecordCount(){
   if (result != NULL)
      return msqlNumRows(result);
   else
      return 0;
};

boolean mSQLDBC::gotoRecordNum(unsigned long int num){
   msqlDataSeek(result, num);
   next();
   pos=num;
   return true;
};
