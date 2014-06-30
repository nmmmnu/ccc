#include "macro.h"

#include <string.h>

#define ESCAPER	    '\\'

char StringEscaper::getChar(){
   char x = getChar_();
   pos++;
   return x;
};

char StringEscaper::getChar_(){
   if (s[pos] == ESCAPER){
      state = true;
      pos++;
      if (!eof())
         return returnEscape(s[pos]);
      else
         return '\0'; //We must return something...
   }

   state = false;
   return s[pos];
};

const char *AbstractMacroReplacer::replace(const char *source){
   if (isempty(source))
      return NULL;

   StringEscaper *se   = new StringEscaper(source);
   StringBuffer  *pole = new StringBuffer();

   sb->clear();

   boolean b = false;

   while (!se->eof()){
      char x = se->getChar();
      
      if (!b){
         if ((!se->getStatus() ) && (x == bdelim))
   	    b = true; 
         else
	    sb->add(x);
      }else{
         if ((!se->getStatus() ) && (x == edelim)){
	    sb->add( replaceField( pole->get() ) );
            b = false;
            pole->clear();
         }
else
            pole->add(x);
 
      }
   }

   delete(pole);
   delete(se);
   return sb->get();
};

const char *MacroReplacer::replaceField(const char *field){
   if (pr != NULL)
      return pr->get( field );
   else
      return NULL;
};
