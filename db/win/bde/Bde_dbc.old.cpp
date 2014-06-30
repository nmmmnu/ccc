/*============================
BDE Data Base Connector Bridge
==============================
Author:   Nikolay Mijaylov Mijaylov
CopyLeft: 10.1998, Sofia
Remaked:  02.1999, Sofia

Note:     This is my first A_DBC driver,
          so as you see A_DBC is BDE alike
*/

#include <vcl\vcl.h>         //Borland C++ Builder "Visual" lib
#include <vcl\dbtables.hpp>  //BDE native lib

#include "../../a_dbc.h"

#define DBC_DATABASE "DATABASE"

#define BDE_SYSDB    "CGI_CORE_DATABASE"    //For internal use only :)

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class BDEDBConnection: public AbstractDBConnection{
protected:
   TDatabase *base;
   static boolean con = false;
public:
   BDEDBConnection():base(NULL){};
//-------------------------------------------------------
   ~BDEDBConnection(){
      if (base != NULL)
         free(base);
      con = false;
   };
//-------------------------------------------------------
   void construct(Properties *cfg);
//-------------------------------------------------------
   boolean isConnected(){
      return (base != NULL) ? base->Connected : false;
   };
//-------------------------------------------------------
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class BDEDBC: public AbstractDBC{
protected:
   TQuery    *query;
public:
//-------------------------------------------------------
   BDEDBC(AbstractDBConnection *DBConnection):AbstractDBC(DBConnection){
      query = new TQuery(NULL);
      query->DatabaseName = BDE_SYSDB;
   };
//-------------------------------------------------------
   ~BDEDBC(){
      delete(query);
   };
//-------------------------------------------------------
   void constructSQL(const char *sql);
//-------------------------------------------------------
   boolean open();
//-------------------------------------------------------
   boolean exec();
//-------------------------------------------------------
   void close();
//-------------------------------------------------------
   boolean bof();
   boolean eof();
//-------------------------------------------------------
   boolean first();
//-------------------------------------------------------
   void next();
   boolean prev();
//-------------------------------------------------------
   void get(Properties *p);
//-------------------------------------------------------
   void getFields(Properties *p);
//-------------------------------------------------------
   long int getRecordCount();
   long int getRecordNum();
   boolean  gotoRecordNum(unsigned long int num);   
//-------------------------------------------------------
   //Uncomment if need :))
   //boolean uniDirectionl(){ return true; };
//-------------------------------------------------------
   char *getDriverName(){
      return "Borland Database Engine Driver for Microsoft Windows";
   };
//-------------------------------------------------------
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
AbstractDBConnection *A_DBC_GetDBConnection(){ 
   return new BDEDBConnection(); 
};

//-------------------------------------------------------

AbstractDBC *A_DBC_GetDBC(AbstractDBConnection *x){ 
   return new BDEDBC(x);
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void BDEDBConnection::construct(Properties *cfg){
   if (isConnected() )
      return;

   if (con)
      return;

   con = true;

   base = new TDatabase(NULL);
   base->LoginPrompt  = false;
   base->AliasName    = cfg->get(DBC_DATABASE, ".\\");
   base->DatabaseName = BDE_SYSDB;
   base->Open();
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void BDEDBC::constructSQL(const char *sql){
   if (!isempty(sql))
      query->SQL->Text = sql;
   else
      query->SQL->Text = "";
};

boolean BDEDBC::open(){
   try{
      query->Open();
   }catch(...){
      return false;
   }
   return true;
};

boolean BDEDBC::exec(){
   //RowsAffected ?!?!?!?
   try{
      query->ExecSQL();
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

boolean BDEDBC::bof(){
   try{
      return query->Bof;
   }catch(...){
      return true;
   }
};

boolean BDEDBC::eof(){
   try{
      return query->Eof;
   }catch(...){
      return true;
   }
};

boolean BDEDBC::first(){
   try{
      query->First();
   }catch(...){}
   return true;
};

void BDEDBC::next(){
   try{
      query->Next();
   }catch(...){}
};

boolean BDEDBC::prev(){
   try{
      query->Prior();
   }catch(...){}
   return true;
};

void BDEDBC::get(Properties *p){
   try{
      for (int i = 0; i < query->FieldCount; i++)
         p->put(   query->Fields[i]->FieldName.c_str(),
                   query->Fields[i]->AsString.c_str());
   }catch(...){}
};

void BDEDBC::getFields(Properties *p){
   try{
      p->clear();
      for (int i = 0; i < query->FieldCount; i++){
         char *c = " ";

         switch (query->Fields[i]->DataType){
            case  ftString    : c[0] = FTCHAR ; break;
            case  ftSmallint  :
            case  ftInteger   :
            case  ftWord      :
            case  ftBCD       :
            case  ftAutoInc   : c[0] = FTNUMBER; break;
            case  ftFloat     :
            case  ftCurrency  : c[0] = FTNUMBER; break;
            case  ftBoolean   : c[0] = FTBOOLEAN; break;
            case  ftDate      :
            case  ftDateTime  : c[0] = FTDATE; break;
            case  ftTime      : c[0] = FTTIME; break;
            case  ftBlob      : c[0] = FTBLOB; break;
            default           : c[0] = FTUNKNOWN;
         }

         p->put(   query->Fields[i]->FieldName.c_str(), c );
      }
   }catch(...){}
};

long int BDEDBC::getRecordCount(){
   try{
      return query->RecordCount;
   }catch(...){
      return 0;
   }
};

long int BDEDBC::getRecordNum(){
   try{
      return query->RecNo;
   }catch(...){
      return 0;
   }
};

boolean  BDEDBC::gotoRecordNum(unsigned long int num){
   try{
      query->First();
      if (num > 1)
         query->MoveBy(num - 1);
   }catch(...){}
   return true;
};
