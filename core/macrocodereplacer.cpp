#include "macrocodereplacer.h"

#define getOpt(a)				\
   if (l > a + 1)				\
      if (tmpfield[l - a - 1] == bdelim){	\
         MACROS  = tmpfield[l - a];		\
         MACROS1 = tmpfield[l - a + 1];		\
         tmpfield [l - a - 1] = '\0';		\
      }

const char *MacroCodeReplacer::replaceField(const char *field){
   if (isempty(field))
      return NULL;

   size_t l = strlen(field);
      
   char *tmpfield = strdupn(field);
   char MACROS  = MACROS_NONE;
   char MACROS1 = MACROS_NONE;

   getOpt(1);   //singles [xxx[a]
   getOpt(2);   //doubles [xxx[a]

   const char *res = MacroReplacer::replaceField(tmpfield);
   free(tmpfield);

   const char *tmp;
      
   switch(MACROS){
      case MACROS_HTML      : tmp = coder->HTMLEncode(res);           break;
      case MACROS_SOFT_HTML : tmp = coder->SoftHTMLEncode(res);       break;
      case MACROS_PHTML     : tmp = coder->HTMLEncode(res, true);     break;
      case MACROS_SOFT_PHTML: tmp = coder->SoftHTMLEncode(res, true); break;
      case MACROS_URL       : tmp = coder->URLEncode(res);            break;
      case MACROS_NUMBER    : tmp = coder->numberEncode(res);         break;
      case MACROS_FLOAT     : tmp = coder->floatEncode(res, ctoi(MACROS1)); break;
      case MACROS_SQL       : tmp = coder->sqlEncode(res);            break;
      case MACROS_SQLNULL   : tmp = coder->sqlNullEncode(res);        break;
      case MACROS_NO        : tmp = res;                              break;

      default               : tmp = res;
         printflog(LOG_NOTIFY, "Macrocode: Using macros without encoding.\n");
   }

   return tmp;
};
