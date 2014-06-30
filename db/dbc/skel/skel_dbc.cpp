/*=============================
Skel Data Base Connector II
===============================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     
Distribution: GPL
A_DBC home    http://w3.nsi.bg/linux/a_dbc/
*/

#include "a_dbc.h"
#include "../mad/mad.h"
#include "../mad/prop.h"

#include "coder/a_dbc_coder_cpp.cpp"
#include "coder/a_dbc_coder_null.cpp"

class SkelDBConnection: public ADBConnection{
public:
   ~SkelDBConnection(){};
   boolean connect(Properties *cfg);
   void disconnect();
   
   //const char *DriverName()         { return a_dbc_drivername; };
   //const char *DriverCopyright()    { return a_dbc_drivercopyright; };
   //const char *DriverDistribution() { return a_dbc_driverdistribution; };
   
   ADBC *getADBC();
};

const char *a_dbc_drivername         = "Skel";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class SkelDBC: public ADBC{
public:
   SkelDBC(ADBConnection *DBConnection):ADBC(DBConnection){};
   ~SkelDBC(){};

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

ADBConnection *getADBConnection(){ return new SkelDBConnection(); };
ADBC *SkelDBConnection::getADBC(){ return new SkelDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean SkelDBConnection::connect(Properties *cfg){};
void SkelDBConnection::disconnect(){};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void SkelDBC::setSQL(const char *sql){};
boolean SkelDBC::exec(){};
boolean SkelDBC::open(){};
void SkelDBC::close(){};
void SkelDBC::next(){};
boolean SkelDBC::eof(){};
unsigned int SkelDBC::getFieldCount(){};
const char *SkelDBC::getFieldName(unsigned int no){};
const char *SkelDBC::getFieldData(unsigned int no){};
long int SkelDBC::getAffectedCount(){};
long int SkelDBC::getRecordCount(){};
boolean SkelDBC::gotoRecordNum(unsigned long int num){};
