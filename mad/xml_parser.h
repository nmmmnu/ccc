#ifndef _XMLPARSER_
#define _XMLPARSER_

#include <string.h>

#include "mad.h"
#include "prop.h"

/*
This library provide control over the XML (or HTML of course) stream.
====================================================================
(C)opyleft 11.1999, Nikolay Mijaylov Mijaylov <nmmm@nmmm.nu>

Simple XML:
----------

<addrbook>
   <peson name="nmmm"    email="nmmm@nmmm.nu"    >
   <peson name="niki"    email="nmmm@hotmail.com">
   <peson name="Nikolay" email="nikolay@nmmm.nu" >
</addrbook>
*/

#define XMLTAGPARSEKEY "tHe_TaG_hIm_SeFl_!@#$%^&*()-wow-really-cool-string-is-it?"
#define XMLMAXLEN (size_t) 64 * 1024

extern const char *NULL_XML_MATRIX[];

class XMLParser{
private:
   size_t len;
protected:
   char   data[XMLMAXLEN + 1];
   size_t pos;

   StringBuffer *doublebuffer;
   StringBuffer *doublebuffer1;

   const char *seekChar(char ch);

   static char *cutTag(char *s);
   static int   cmpTag(const char *tag1, const char *tag2);
public:
   XMLParser():doublebuffer (new StringBuffer()),
               doublebuffer1(new StringBuffer()){};
   virtual ~XMLParser(){
      delete(doublebuffer);
      delete(doublebuffer1);
   };

   boolean loadFromFile(const char *filename);
   boolean loadFromString(const char *s);

   boolean eof();

   //return text to the next mark
   const char *seekNextTag();

   const char *seekNextTag(const char *m[], const char *d[]);

   inline const char *seekNextTag(const char *m[]){
      return seekNextTag(m, NULL_XML_MATRIX);
   };

   //Lazy Boy
   inline const char *seekNextTag(const char *x, const char *y=NULL){
      const char *m[] = { x, NULL };
      const char *d[] = { y, NULL };
      return seekNextTag(m, d);
   };

   //return the mark. `seekNextTag()' must be invoked before
   const char *getNextTag();

   //Extract text between `double mark', such as `<p>Hello</p>'
   //If rmode is false, tag is processed to `<p>Hello<p>sir</p>'
   const char *extractDoubleTag(const char *tag, boolean rmode=true);

   //Extract param=value in tag
   static void parseTag(const char *tag, Properties *p);
};

#endif
