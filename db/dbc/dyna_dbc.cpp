/*============================
Dynamix Data Base Connector II
==============================

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/
*/

#include "a_dbc.h"

#include <dlfcn.h>

#define DBC_DATABASEDRIVER "DATABASEDRIVER"

const char *a_dbc_drivername         = "ADBC Dynamic Lib Loader Beta";
const char *a_dbc_drivercopyright    = "(c) 1998 - 2000, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class DynaDBConnection: public ADBConnection{
protected:
   void          *h;
   ADBConnection *con;
protected:
   boolean  openld(const char *driver);
   void     closeld();
public:
   boolean  connect(Properties *cfg);
   void     disconnect();
   
   ADBC    *getADBC();
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ADBConnection *getADBConnection(){ 
   return new DynaDBConnection();
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean DynaDBConnection::connect(Properties *cfg){
   if (!openld(cfg->get(DBC_DATABASEDRIVER, "AntiNULL")))
      return false;

   boolean b = con->connect(cfg);

   if (!b)
      closeld();

   return b;
};

void DynaDBConnection::disconnect(){
   con->disconnect();

   closeld();
};

ADBC *DynaDBConnection::getADBC(){
   return con->getADBC();
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void DynaDBConnection::closeld(){
   dlclose(h);
};

boolean DynaDBConnection::openld(const char *driver){
   h = dlopen (driver, RTLD_LAZY);

   if(!h){
      printferr("Can't open dynamic lib!\n");
      return false;
   }

   ADBConnection *(*action)() = (ADBConnection *(*)()) dlsym(h, "getADBConnection");

   if (!action){
      printferr("Can't get symbol from dynamic lib!\n");
      return false;
   }

   con = action();

   return true;
};
