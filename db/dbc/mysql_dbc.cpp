/*==========================
MySQL Data Base Connector II
============================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     1999, Sofia
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/
*/

#include "a_dbc.h"

#include <mysql.h> //MYSQL native library

#define DBC_HOST     "DATABASEHOST"
#define DBC_DATABASE "DATABASE"
#define DBC_USER     "DATABASEUSER"
#define DBC_PASS     "DATABASEPASS"
//If Result is live then it will be faster and will want less memory
//but you cannot use 'goto' and simillar.
//This is MYSQL only ability...
#define DBC_RESULT   "DATABASELIVEQUERY"

class MYSQLDBConnection: public ADBConnection{
protected:
   boolean con;
public:
   MYSQL   sock;
   int     dbc_res;

   boolean connect(Properties *cfg);
   void disconnect();
   
   ADBC *getADBC();
};

const char *a_dbc_drivername         = "TcX MySQL";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class MYSQLDBC: public ADBC{
protected:
   int               isok;
   MYSQL_RES         *result;
   MYSQL_ROW         row;
   MYSQL_FIELD       *field;
   unsigned long int  pos;
public:
   MYSQLDBC(ADBConnection *DBConnection):ADBC(DBConnection),
                                     isok(-1),
                                     result(NULL){};
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

   boolean gotoRecordNum(unsigned long int num);
};

ADBConnection *getADBConnection(){ return new MYSQLDBConnection(); };
ADBC *MYSQLDBConnection::getADBC(){ return new MYSQLDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean MYSQLDBConnection::connect(Properties *cfg){
   dbc_res = cfg->getAsInt(DBC_RESULT, 0);
    
   if (!mysql_connect( &sock, cfg->get(DBC_HOST, "localhost"),
                              cfg->get(DBC_USER, "nobody"),
                              cfg->get(DBC_PASS, "")
                              ))
      con = false; //Not Connected

   if (mysql_select_db(&sock, cfg->get(DBC_DATABASE, "")) < 0 )
      mysql_close(&sock);
   else
      con = true;
      
   return con;
};

void MYSQLDBConnection::disconnect(){
   mysql_close(&sock);
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define SOCK    ((MYSQLDBConnection *) DBConnection)->sock
#define DBC_RES ((MYSQLDBConnection *) DBConnection)->dbc_res

void MYSQLDBC::setSQL(const char *sql){
   if (!isempty(sql))
      isok = mysql_query( &SOCK, sql);
};

boolean MYSQLDBC::exec(){
   return isok >= 0;
};

boolean MYSQLDBC::open(){
   if (isok < 0)
      return false;

   if (DBC_RES == 1)
      result = mysql_use_result(&SOCK);
   else
      result = mysql_store_result(&SOCK);
   
   pos = 0;
   if (result != NULL)
      row = mysql_fetch_row(result);
      
   return (result != NULL);
};

void MYSQLDBC::close(){
   mysql_free_result(result);
};

void MYSQLDBC::next(){
   row = mysql_fetch_row(result);
   pos++;
};

boolean MYSQLDBC::eof(){
   if (DBC_RES == 1)
      return mysql_eof(result);
   else
      return  (long int) pos >= getRecordCount();
};

unsigned int MYSQLDBC::getFieldCount(){
   return mysql_num_fields(result);
};

const char *MYSQLDBC::getFieldName(unsigned int no){
   mysql_field_seek(result, no);
   field = mysql_fetch_field(result);
   return field->name;
};

const char *MYSQLDBC::getFieldData(unsigned int no){
   if (row)
      return row[no];
   return NULL;
};

long int MYSQLDBC::getAffectedCount(){
   if (result != NULL)
      return mysql_affected_rows(&SOCK);
   else
      return 0;
};

long int MYSQLDBC::getRecordCount(){
   if (result != NULL)
      return mysql_num_rows(result);
   else
      return 0;
};

boolean MYSQLDBC::gotoRecordNum(unsigned long int num){
   mysql_data_seek(result, num);
   next();
   pos=num;
   return true;
};
