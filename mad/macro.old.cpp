#include "macro.h"

#include <string.h>

#define ESCAPER	    '\\'

#define LS_NONE   0
#define LS_ESCAPE 1
#define LS_BEGIN  2
#define LS_END    3
#define LS_CHAR   4

inline void AbstractMacroReplacer::initLogic(){ logic_status = LS_NONE; };
inline char AbstractMacroReplacer::logic(char x){
   if (logic_status == LS_ESCAPE){
      logic_status = LS_CHAR;
      return x;
   }

   if (x == ESCAPER){
      logic_status = LS_ESCAPE;
      return ' ';
   }
   
   if (x == bdelim){
      logic_status = LS_BEGIN;
      return bdelim;
   }
   
   if (x == edelim){
      logic_status = LS_END;
      return edelim;
   }
   
   logic_status = LS_CHAR;
   return x;
};

const char *AbstractMacroReplacer::replace(const char *source){
   if (isempty(source))
      return NULL;

   StringBuffer *pole = new StringBuffer();

   sb->clear();

   int len = strlen(source);
   int i = 0;
   boolean b = false;

   while (i <= len){
      if (!b){
         char x = logic(source[i]);
	 /* 
	 We must NOT check 'i'
	 */
         if (logic_status == LS_BEGIN)
   
   	    b = true; 
         else
	    if (logic_status == LS_CHAR) sb->add(x);
      }else{
         char x = logic(source[i]);
	 
         if (logic_status == LS_END){
	    sb->add( replaceField( pole->get() ) );
            b = false;
            pole->clear();
         }
else
            pole->add(x);
 
	 /*
	 There we do not need to parse this correctly
	 because this is macros-name...
	 and there are NO '\\' escapes
	 */
      }

      i++;
   }

   delete(pole);
   return sb->get();
};

const char *MacroReplacer::replaceField(const char *field){
   if (pr != NULL)
      return pr->get( field );
   else
      return NULL;
};
