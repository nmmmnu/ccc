#ifndef _MAD_CODER_H
#define _MAD_CODER_H

#include "mad.h"

#define MAXNUMTMP 50
//I think 50 is enought?!?!? :)))

class Coder{
protected:
   StringBuffer *sb;
   char numtmp[MAXNUMTMP];
public:
   Coder():sb(new StringBuffer){};
   virtual ~Coder(){
      delete(sb);
   };
   
   const char *URLDecode(const char *enc);
   const char *URLEncode(const char *dec);

   const char *HTMLEncode(const char *enc, boolean phpn = false);
   const char *SoftHTMLEncode(const char *enc, boolean phpn = false);
   
   const char *numberEncode(const char *enc);
   const char *floatEncode(const char *enc, int dec = 0);
   const char *CPPEncode(const char *enc, boolean sh = false);
   const char *CPPDecode(const char *enc);
   const char *sqlEncode(const char *enc);
   const char *sqlNullEncode(const char *enc);
};

#endif
//_MAD_CODER_H