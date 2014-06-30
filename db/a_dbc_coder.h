/*=============================
Abstract Data Base Connector II
===============================
an Open DataBase Conection Interface for a C++

--------------------------
SQL String Encoder library
--------------------------

Author:       Nikolay Mijaylov Mijaylov
e_mail:       nmmm@nsi.bg
CopyLeft:     10.1998, Sofia
              01.1999, Sofia - little interface changes, 
	                       new idea about c++ linker
              04.1999, Sofia - A_DBC II
Distribution: GPL
A_DBC home    http://www.nmmm.nu/linux/a_dbc/

--------------
The target of this library is to made database sensetive
SQL String encoding...

Curently Supots:
   1. C/C++ like encoding   \"Hello\"
   2. Pascal like encoding  ""Hello""
   3. "Simple" encoding     `Hello`
   
There are support for NULL fields too...
   1. It is one and same for all SQL RDBMS: 'Hello' or NULL
   2. Different for Fake_DBC:                Hello  or (none)
*/

#ifndef _A_DBC_CODER_H
#define _A_DBC_CODER_H

#include <mad/mad.h>

class A_DBC_Coder{
protected:
   StringBuffer *sb;
public:
   A_DBC_Coder():sb(new StringBuffer){};
   virtual ~A_DBC_Coder(){
      delete(sb);
   };

   const char *sqlEncode(const char *enc);
   const char *sqlNullEncode(const char *enc);
};

#endif
