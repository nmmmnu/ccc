/*=============================
Informix Data Base Connector II
===============================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     01.2000, Sofia
Distribution: see file "COPYING.INFORMIX"
A_DBC home    http://www.nmmm.nu/linux/a_dbc/

Important notes:
----------------
   (o) This driver is developed using free Informix ESQL/C for Linux.
   (o) See "README.INFORMIX"  for more info
   (o) See "COPYING.INFORMIX" for licence !!!
*/

#include "a_dbc.h"

const char *a_dbc_drivername         = "Informix Database Server";
const char *a_dbc_drivercopyright    = "(c) 1998 - 2000, Alber Shahugian, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "see file \"COPYING.INFORMIX\"";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class IxDBConnection: public ADBConnection{
public:
   boolean connect(Properties *cfg);
   void disconnect();
   
   ADBC *getADBC();
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Counter for Informix descriptors...
unsigned long int IX_DESCR_COUNT = 0;

class IxDBC: public ADBC{
protected:
   boolean _eof;
   int     affectedcount;

   char   *IX_DESCR;
   char   *IX_SEL;
   char   *IX_CURS;
public:
   IxDBC(ADBConnection *DBConnection):ADBC(DBConnection){
      //Preparing ESQL unique (in program only) strings
      IX_DESCR_COUNT++;

      IX_DESCR = strconcat("IXD_", itoa(IX_DESCR_COUNT));
      IX_SEL   = strconcat("IXS_", itoa(IX_DESCR_COUNT));
      IX_CURS  = strconcat("IXC_", itoa(IX_DESCR_COUNT));
   };
   ~IxDBC(){
      free(IX_DESCR);
      free(IX_SEL  );
      free(IX_CURS );

      IX_DESCR_COUNT--;
   }

   void setSQL(const char *sql);

   boolean exec();
   boolean open();
   void close();

   void next();

   unsigned int getFieldCount();
   const char *getFieldName(unsigned int no);
   const char *getFieldData(unsigned int no);

   //Do I need these as inlines?
   boolean eof(){ return _eof; };
   long int getAffectedCount(){ return affectedcount; };
   long int getRecordCount(){ return affectedcount; };
protected:
   void trim_r(char a[], size_t len);
   boolean  sqlstate;
   void calcAffectedCount();
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Informix ESQL/C preprocessed file
#include "informix/ix_dbc.esql.c"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ADBConnection *getADBConnection(){ return new IxDBConnection(); };
ADBC *IxDBConnection::getADBC(){ return new IxDBC(this); };
