#include "a_dbc_coder.h"

const char *A_DBC_Coder::sqlNullEncode(const char *enc){
   sb->clear();
   if (isempty(enc))
      return "NULL";
      
   sb->add('\'');
   sb->add(enc);
   sb->add('\'');

   return sb->get();
};
