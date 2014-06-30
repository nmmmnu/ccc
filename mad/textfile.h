#ifndef _TEXTFILE_H
#define _TEXTFILE_H

#include "mad.h"

#define  TEXT_FILE_DATA_LEN 64000

class TextFile{
private:
   size_t len;  
protected:
   char data[TEXT_FILE_DATA_LEN + 1];
    
   virtual boolean iterator(const char *) = 0;
public:
   TextFile(){
      data[0] = '\0';
      len = 0;
   };
   virtual ~TextFile(){};
   
   boolean loadFromFile(const char *filename);
   //void saveToFile(const char *filename);
   void iterate();
   
   size_t getLen(){
      return len;
   }
};

#endif
