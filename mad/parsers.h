/*
   Parsers - an open parse interface
 by Nikolay Mijaylov (c)08.1998, Sofia

object tree:

parser(8)=+-------File
          |
          +-------String=+--(QueryString)
                         |
                         +--(CookieString)
*/
#ifndef _MAD_PARSERS_H
#define _MAD_PARSERS_H

#include "mad.h"
#include "prop.h"
#include "string.h"

class Parser{
protected:
   Properties *p;
public:
   Parser(Properties *p):p(p){};
   virtual ~Parser(){};
   void clear(){
      p->clear();
   };
   virtual void parse() = 0;
};

class FileParser: public Parser{
protected:
   char *filename;
public:
   FileParser(Properties *p, const char *filename):Parser(p),
                                                   filename(strdup(filename)){};
   ~FileParser(){
      free(filename);
   };
   void parse(){
      p->load(filename);
   };
};

// About CGI String parser
// This appears in Core Web Programming from
// Prentice Hall Publishers, and may be freely used
// or adapted. 1997 Marty Hall, hall@apl.jhu.edu.

// Designed in C++ of me, Nikolay Mijaylov (c)08.1998
class StringParser: public Parser{
private:
   char *data;
   char delim1;
   char delim2;

public:
   StringParser(Properties *p, const char *data, char delim1, char delim2):
                                                      Parser(p),
                                                      data(strdupn(data)),
                                                      delim1(delim1),
                                                      delim2(delim2){};
   ~StringParser(){
      free(data);
   };
   void parse();
};

#endif
//_MAD_PARSERS_H