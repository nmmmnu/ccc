#include "macro.h"

#include <string.h>

const char *AbstractMacroReplacer::replace(const char *source){
   if (isempty(source))
      return NULL;

   StringBuffer  *pole = new StringBuffer();

   sb->clear();

   boolean b = false;

   for (size_t i = 0; i < strlen(source); i++){
      char x = source[i];
      char y = source[i + 1];

      if (!b){
         if ((x == bdelim) && (y != bdelim))
   	    b = true; 
         else
	    sb->add(x);
      }else{
         if ((x == edelim) && (y != edelim)){
	    sb->add( replaceField( pole->get() ) );
            b = false;
            pole->clear();
         }else
            pole->add(x); 
      }

      /*
      if (x == y){
         if ((x != bdelim) && (x != edelim))
            if (b)
               pole->add(x);
            else
               sb->add(x);
         i++;
      }*/
      if ((x == y                        ) && 
          ((x == bdelim) || (x == edelim)))
         i++;
   }

   delete(pole);
   return sb->get();
};

const char *MacroReplacer::replaceField(const char *field){
/*
   if (pr != NULL)
      return pr->get( field );
   else
      return NULL;
*/
   return (pr) ? pr->get(field) : NULL;
};
