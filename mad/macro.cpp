#include "macro.h"

#include <string.h>

/*
* Current implementation:
*       $[pole[-]
*/

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
         if ((x == hdelim) && (y == bdelim)){
   	    b = true;
            i++;
         }else
	    sb->add(x);
      }else{
         if (x == edelim){
	    sb->add( replaceField( pole->get() ) );
            b = false;
            pole->clear();
         }
else
            pole->add(x);
 
      }
   }

   delete(pole);
   return sb->get();
};
