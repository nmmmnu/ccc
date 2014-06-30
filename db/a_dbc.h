/*=============================
Abstract Data Base Connector II
===============================
an Open DataBase Conection Interface for a C++

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     10.1998, Sofia
              01.1999, Sofia - little interface changes, 
	                       new idea about c++ linker
              04.1999, Sofia - A_DBC II
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/
*/

#ifndef _A_DBC_H
#define _A_DBC_H

#include <mad/mad.h>
#include <mad/prop.h>

//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//DBConnection is object for making database connection.
//it must be only one in 'normal' programs :))

#define a_dbc_version "3.2"

extern const char *a_dbc_drivername;
extern const char *a_dbc_drivercopyright;
extern const char *a_dbc_driverdistribution;

class ADBC;

class ADBConnection{
public:
   virtual ~ADBConnection(){};

   virtual boolean connect(Properties *cfg) = 0;
   virtual void disconnect() = 0;
   
   virtual const char *DriverName()         { return a_dbc_drivername; };
   virtual const char *DriverCopyright()    { return a_dbc_drivercopyright; };
   virtual const char *DriverDistribution() { return a_dbc_driverdistribution; };
   
   virtual ADBC *getADBC() = 0;
};

//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//A_DBC is object for making database querying.
//they may be many per program... :))

class ADBC{
protected:
   ADBConnection *DBConnection;
public:
   ADBC(ADBConnection *DBConnection):DBConnection(DBConnection){};
   virtual ~ADBC(){};

   virtual void setSQL(const char *sql) = 0;

   virtual boolean exec() = 0;

   virtual boolean open() = 0;
   virtual void close() = 0;

   virtual void next() = 0;
   virtual boolean eof() = 0;

   virtual unsigned int getFieldCount() = 0;
   virtual const char *getFieldName(unsigned int no) = 0;
   //virtual const char getFieldType(unsigned int no) = 0;
   virtual const char *getFieldData(unsigned int no) = 0;
   
   virtual long int getAffectedCount(){ return 1; };
   virtual long int getRecordCount(){ return -1; };
   virtual boolean gotoRecordNum(unsigned long int){ return false; };
};

extern "C"{
ADBConnection *getADBConnection();
}

#endif
