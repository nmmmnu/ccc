#include "textfile.h"

boolean TextFile::loadFromFile(const char *filename){
   len = file2char(filename, data, TEXT_FILE_DATA_LEN);
   
   return (len > 0);
};

void TextFile::iterate(){
   if (len <= 0)
      return;

   StringBuffer *sb = new StringBuffer();
   boolean b = false;

   for (size_t i = 0; i <= len; i++)
      if ((data[i] != '\n') && 
          (data[i] != '\r')){
         sb->add(data[i]);
         b = true;
      }else{
         if (!iterator(sb->get()))
            break;
         b = false;
         sb->clear();
      }

   if (b) //resolving <EOF> at the end
      iterator(sb->get() );

   delete(sb);
};
