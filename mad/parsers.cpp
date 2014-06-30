#include "parsers.h"
//#include <unistd.h>

void StringParser::parse(){
   if (isempty(data))
      return;

   StringTokenizer *st = new StringTokenizer(data, delim1);

   const char *namevalue = st->nextToken();

   while(namevalue != NULL){
      p->putParsedStr(namevalue, delim2);

      namevalue = st->nextToken();
   }
   delete(st);
};

/*
void EnvParser::parse(){
   int i = 0;
   while (environ[i])
      p->putParsedStr(environ[i++], '=');
};
*/
