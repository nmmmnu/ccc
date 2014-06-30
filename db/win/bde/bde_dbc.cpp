/*===============================
BDE Data Base Connector Bridge II
=================================
Author:   Nikolay Mijaylov Mijaylov
CopyLeft: 10.1998 - 12.1999, Sofia
Remaked:  02.1999, Sofia

Note:     This is my first A_DBC driver,
          so as you see A_DBC is BDE like
Note 12.1999:
          But not so, am I right?
*/

#include <vcl\vcl.h>         //Borland C++ Builder "Visual" lib
#include <vcl\dbtables.hpp>  //BDE native lib
#include "../../a_dbc.h"

#define DBC_DATABASE      "DATABASE"       //BDE aliase or directory
#define DBC_DATABASE_USER "DATABASEUSER"
#define DBC_DATABASE_PASS "DATABASEPASS"
#define DBC_DATABASE_STR  "DATABASE_STR"   //TDataBase->DatabaseName

//Put something here :)
#define BDE_DBC_DEFAULT_DB_NAME "BDE_DBC_DB"
#define BDE_FIELD_MAX_LEN       1024 * 5class BDEDBConnection: public ADBConnection{public:   TDatabase *base;

   BDEDBConnection():base(NULL){};
   ~BDEDBConnection(){
      if (base)
         delete(base);
   };
   boolean connect(Properties *cfg);
   inline void disconnect()         { base->Close(); };

   ADBC *getADBC();
};

const char *a_dbc_drivername         = "BDE Borland Database Engine";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class BDEDBC: public ADBC{
protected:
   TQuery    *query;
public:
   BDEDBC(ADBConnection *DBConnection):ADBC(DBConnection),
                                       query(new TQuery(NULL)){};
   ~BDEDBC(){
      delete(query);
   };

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

ADBConnection *getADBConnection(){ return new BDEDBConnection(); };
ADBC *BDEDBConnection::getADBC(){ return new BDEDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean BDEDBConnection::connect(Properties *cfg){
   base = new TDatabase(NULL);
   try{
     base->LoginPrompt  = false;
     base->AliasName    = cfg->get(DBC_DATABASE, ".\\");
     base->DatabaseName = cfg->get(DBC_DATABASE_STR, BDE_DBC_DEFAULT_DB_NAME);

     //Strange BDE/ODBC Authentication
     if (cfg->exist(DBC_DATABASE_USER)){
        base->Params->Values["USER NAME"] = cfg->get(DBC_DATABASE_USER);
        if (cfg->exist(DBC_DATABASE_PASS))
           base->Params->Values["PASSWORD"] = cfg->get(DBC_DATABASE_PASS);
     }

     base->Open();
   }catch(...){}
   return base->Connected;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void BDEDBC::setSQL(const char *sql){
   query->DatabaseName = ((BDEDBConnection *) DBConnection)->base->DatabaseName;
   query->SQL->Text = !isempty(sql) ? sql : "";
};
boolean BDEDBC::exec(){
   try{
      query->ExecSQL();
   }catch(...){
      return false;
   }
   return true;
};
boolean BDEDBC::open(){
   try{
      query->Open();
   }catch(...){
      return false;
   }
   return true;
};
void BDEDBC::close(){
   try{
      query->Close();
   }catch(...){}
};
void BDEDBC::next(){
   try{
      query->Next();
   }catch(...){}
};
boolean BDEDBC::eof(){
   try{
      return query->Eof;
   }catch(...){
      return true;
   }
};
inline unsigned int BDEDBC::getFieldCount(){
   try{
      return query->FieldCount;
   }catch(...){
      return 0;
   }
};
inline const char *BDEDBC::getFieldName(unsigned int no){
   try{
      static char x[BDE_FIELD_MAX_LEN + 1];
      strncpy(x, query->Fields[no]->FieldName.c_str(), BDE_FIELD_MAX_LEN);
      x[BDE_FIELD_MAX_LEN] = '\0';
      return x;
   }catch(...){
      return "";
   }
};

inline const char *BDEDBC::getFieldData(unsigned int no){
   try{
      static char x[BDE_FIELD_MAX_LEN + 1];
      strncpy(x, query->Fields[no]->AsString.c_str(), BDE_FIELD_MAX_LEN);
      x[BDE_FIELD_MAX_LEN] = '\0';
      return x;
   }catch(...){
      return "";
   }
};
long int BDEDBC::getAffectedCount(){
   return query->RowsAffected == -1 ? 0 : query->RowsAffected;
};
long int BDEDBC::getRecordCount(){
   try{
      return query->RecordCount;
   }catch(...){
      return 0;
   }
};
boolean BDEDBC::gotoRecordNum(unsigned long int num){   try{      if (num > 1){
         query->First();
         query->MoveBy(num - 1);
         return true;
      }
   }catch(...){}
   return false;
};
