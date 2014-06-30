/*========================
MFC Data Base Connector II
==========================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     2000, Sofia
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/
*/

#include <afxdb.h>

#include "a_dbc.h"
#include <mad/mad.h>
#include <mad/prop.h>

#define CONNECTIONSTRING "CONNECTIONSTRING"

class MFCDBConnection: public ADBConnection{
public:
   CDatabase *db;
public:
   MFCDBConnection():db(new CDatabase()){};
   ~MFCDBConnection(){
      delete(db);
   };
   boolean connect(Properties *cfg);
   void disconnect();
   
   ADBC *getADBC();
};

const char *a_dbc_drivername         = "MFC ODBC Bridge";
const char *a_dbc_drivercopyright    = "(c) 1998 - 2000, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class MFCDBC: public ADBC{
private:
   CRecordset *rs;
	boolean     isok;
	char       *sql;
public:
   MFCDBC(ADBConnection *DBConnection):ADBC(DBConnection),
          rs(new CRecordset( ((MFCDBConnection *) DBConnection)->db)),
          isok(false),
          sql(NULL){};

   ~MFCDBC(){
		delete(rs);
		if (sql)	free(sql);
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
   
   //long int getAffectedCount();

   //VC has strange support for this :)
   //long int getRecordCount();

   boolean gotoRecordNum(unsigned long int num);
};

ADBConnection *getADBConnection(){ return new MFCDBConnection(); };
ADBC *MFCDBConnection::getADBC(){ return new MFCDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean MFCDBConnection::connect(Properties *cfg){
   try{
      db->OpenEx(cfg->get(CONNECTIONSTRING), CDatabase::noOdbcDialog);
   }catch (CException *){
      return false;
   }
   return true;
};

void MFCDBConnection::disconnect(){
   try{
      db->Close();
   }catch (CException *){}
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define _safe(a) try{ a; }catch(CException *){ isok = false; }


void MFCDBC::setSQL(const char *sql1){
   sql = strdupn(sql1);
};

boolean MFCDBC::exec(){
   _safe( rs->m_pDatabase->ExecuteSQL(sql) );

   return isok;
};

boolean MFCDBC::open(){
   isok = true;
   _safe( rs->Open(CRecordset::forwardOnly, sql));
   return isok;
};

void MFCDBC::close(){
   _safe( rs->Close() );
};

void MFCDBC::next(){
   _safe( rs->MoveNext() );
};

boolean MFCDBC::eof(){
   _safe( return rs->IsEOF() );
   return true;
};

unsigned int MFCDBC::getFieldCount(){
   _safe( return rs->GetODBCFieldCount() );
   return 0;
};

const char *MFCDBC::getFieldName(unsigned int no){
   CODBCFieldInfo fi;

   _safe( rs->GetODBCFieldInfo(no, fi) );

   if (isok){
      const char *s = fi.m_strName; //Careful type cast
      return s;
   }


   return NULL;
};

const char *MFCDBC::getFieldData(unsigned int no){
   CString varValue;

   _safe( rs->GetFieldValue(no, varValue) );

   if (isok){
      const char *s = varValue; //Careful type cast
      return s;
   }


   return NULL;
};

boolean MFCDBC::gotoRecordNum(unsigned long int num){
   _safe( rs->Move(num, SQL_FETCH_ABSOLUTE) );

   return false;
};
