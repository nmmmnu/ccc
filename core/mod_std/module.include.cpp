#include <stdio.h>
#include <stdlib.h>

#include "module.version.h"

#include "../main.h"

void CGICoreModule__INCLUDE__Process(Properties *, const char *);

CGICoreModule CGICoreModule__INCLUDE = {
	"include",
	"SSI-like file includes",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"include",
	NULL,
	NULL,
	CGICoreModule__INCLUDE__Process
};

void getrelativepath(const char *x, StringBuffer *sb){
   size_t  pos = strlen(x) - 1;
   boolean b   = false;
   
   for (size_t i = strlen(x); i > 0; i--)
      if (x[i - 1] == PATH_DELIMITER){
         pos = i - 1;
         b = true;
         break;
      }

   if (b)
      for (size_t j = 0; j <= pos; j++)
         sb->add(x[j]);
};

void CGICoreModule__INCLUDE__Process(Properties *x, const char *){
   if (!x               ) return;
   if (!x->exist("FILE")) return;
   
   char *file = (char *) x->get("FILE");

   //Little trick with preprocessor
#if UNIX_PATH_DELIMITER != PATH_DELIMITER
   replacechar(file, UNIX_PATH_DELIMITER, PATH_DELIMITER);
#endif

   StringBuffer *sb = new StringBuffer();

//I desided to remove it.
//#ifndef PARNOIDSECURITY
   if (x->getAsInt("ABSOLUTE") >= 1)
      sb->add(file);
   else 
//#endif
   if (file[0] == PATH_DELIMITER){
      //Fuck the Win and his drives like c:
      //BTW, this is not important at *ALL* here
      const char *dr = getenv("DOCUMENT_ROOT");

      if (!isempty(dr)){
         sb->add(dr);

         if (dr[strlen(dr) - 1] != PATH_DELIMITER)
            sb->add(PATH_DELIMITER);
      }

      sb->add(&file[1]);
   }else{
      getrelativepath(the_file, sb);
      sb->add(file);
   }

   {//Little trick with stack, for include relative files...
      const char *tf = the_file;
      the_file = sb->get();
      parseFromFile(the_file);
      the_file = tf;
   }

   delete(sb);
};
