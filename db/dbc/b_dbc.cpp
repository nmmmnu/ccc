/*========================
Beagle Data Base Connector
==========================
Author:   Nikolay Mijaylov Mijaylov
CopyLeft: 11.1998, Sofia

WARNING:  IT IS STILL NOT STABLE TESTED !!!
          because in Beagle SQL still has errors...
	  zombye processes and etc...

   As u see this is a_dbc 2.6 api, not 3.0!!!
Sometimes, when Beagle going stable, I;ll fix
this file too...
*/

#include "a_dbc.h"

#include <bsql.h>   //Beagle native lib

#define DBC_HOST	"DATABASEHOST"
#define DBC_DATABASE	"DATABASE"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class BeagleDBConnection: public AbstractDBConnection{
public:
   int conn;
//-------------------------------------------------------
   BeagleDBConnection(){
      conn = 0;
   };   
//-------------------------------------------------------
   virtual ~BeagleDBConnection(){
      if (isConnected())
         BSQLDisconnect(conn);
   };
//-------------------------------------------------------
   virtual void construct(Properties *cfg);
//-------------------------------------------------------
   virtual boolean isConnected(){
      return conn > 0;
   };
//-------------------------------------------------------
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class BeagleDBC: public AbstractDBC{
protected:
   bresult *res;
   int      rownum;
public:
//-------------------------------------------------------
   BeagleDBC(AbstractDBConnection *DBConnection):AbstractDBC(DBConnection),
                                                 res(NULL),
						 rownum(0){};
//-------------------------------------------------------
   virtual void constructSQL(const char *sql);
//-------------------------------------------------------
   virtual boolean open();
//-------------------------------------------------------
   virtual boolean exec();
//-------------------------------------------------------
   virtual void close();
//-------------------------------------------------------
   virtual boolean bof();
   virtual boolean eof();
//-------------------------------------------------------
   virtual boolean first();
//-------------------------------------------------------
   virtual void next();
   virtual boolean prev();
//-------------------------------------------------------
   virtual void get(Properties *p);
//-------------------------------------------------------
   virtual void getFields(Properties *p);
//-------------------------------------------------------
   virtual long int getRecordCount();
   virtual long int getRecordNum();
   virtual boolean  gotoRecordNum(unsigned long int num);   
//-------------------------------------------------------
   virtual char *getDriverName(){
      return "Beagle SQL Driver";
   }
//-------------------------------------------------------
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
AbstractDBConnection *A_DBC_GetDBConnection(){ 
   return new BeagleDBConnection(); 
}

//-------------------------------------------------------

AbstractDBC *A_DBC_GetDBC(){ 
   return new BeagleDBC();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void BeagleDBConnection::construct(Properties *cfg){
      conn = BSQLConnect(cfg->get(DBC_HOST));
      if isConnected()
         BSQLSetCurrentDB(cfg->get(DBC_DATABASE));
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define CONN ((BeagleDBConnection *) DBConnection)->conn

void BeagleDBC::constructSQL(const char *sql){
   if ( !DBConnection->isConnected() )
      return;

   if (!isempty(sql)){
      char *sql1 = strdup(sql); //Because sql is const :)
      res = BSQLQueryDB(CONN, sql);
      free(sql1);
   }
};

boolean BeagleDBC::open(){
   if ( res == NULL )
      return false;
         
   first();

   return true;
};

boolean BeagleDBC::exec(){
   boolean b = ( res != NULL );
   BSQLFreeResult(res);
   return b;
};

void BeagleDBC::close(){
   BSQLFreeResult(res);
};

boolean BeagleDBC::bof(){
   return (rownum == 0);
};

boolean BeagleDBC::eof(){
   return ( (unsigned long int) rownum == getRecordCount() );
};

boolean BeagleDBC::first(){
   rownum = 0;
};

void BeagleDBC::next(){
   if (!eof())
      rownum++;
};

boolean BeagleDBC::prev(){
   if (!bof())
      rownum--;
};

void BeagleDBC::get(Properties *p){
   for (int i = 0; i < BSQLnfields(res); i++)
      p->put(BSQLFieldName(res,i), BSQLFieldValue(res, rownum, i) );
};

void BeagleDBC::getFields(Properties *p){
   char x[2];
   x[0] = FTUNKNOWN;
   x[1] = '\0';
   for (int i = 0; i < PQnfields(res); i++){
      switch(BSQLFieldType(res,i)){
         case FTYPE_CHAR      : x[0] = FTCHAR;
         case FTYPE_INT       :
         case FTYPE_FLOAT     : x[0] = FTNUMBER;
         case FTYPE_BLOB      : x[0] = FTBLOB;
         case FTYPE_TIME      : x[0] = FTIME;
         case FTYPE_TIMESTAMP :
         case FTYPE_DATE      : x[0] = FTDATE;
         default              : x[0] = FTUNKNOWN;	    
      }
      p->put(BSQLFieldName(res,i), x);
   }
};

long int BeagleDBC::getRecordCount(){
   if (DBConnection->isConnected())
      return BSQLntuples(res);

   return 0;
};

long int BeagleDBC::getRecordNum(){
   return rownum + 1;
};

boolean  BeagleDBC::gotoRecordNum(unsigned long int num){
   rownum = (int) num - 1;
};
