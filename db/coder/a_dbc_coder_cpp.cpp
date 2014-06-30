#include "a_dbc_coder.h"

//Hello \'sir\'

const char *A_DBC_Coder::sqlEncode(const char *enc){
   sb->clear();
   if (isempty(enc))
      return NULL;
      
   for (unsigned int i = 0; i < strlen(enc); i++)
      switch (enc[i]){
         case '\'': sb->add("\\\'");       break;
         case '\"': sb->add("\\\"");       break;
         case '\\': sb->add("\\\\");       break;

         default  : sb->add( enc[i] );
      }

   return sb->get();
};
