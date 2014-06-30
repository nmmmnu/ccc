#include "mad.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>


unsigned short int stdloglevel = LOG_ERROR;
FILE *stdlog = stderr; 

#define printinthefile(x)	{	\
	va_list ap;			\
	va_start(ap, pattern);		\
	vfprintf(x, pattern, ap); 	\
	fflush(x);			\
	va_end(ap);			\
}

void printflog(unsigned short int level, const char *pattern, ...){
   if (stdloglevel >= level)
      printinthefile(stdlog);
};
void printflog(const char *pattern, ...){
   if (stdloglevel >= 1)
      printinthefile(stdlog);
};
void printferr(const char *pattern, ...){
   printinthefile(stderr);
};

#undef printinthefile

char *to_upper(char *c){
   if (isempty(c))
      return NULL;
   for(size_t i=0; i<strlen(c); i++)
      c[i]=toupper(c[i]);
   return c;
};

char *to_lower(char *c){
   if (isempty(c))
      return NULL;
   for(size_t i=0; i<strlen(c); i++)
      c[i]=tolower(c[i]);
   return c;
};

char *strcut(char *c){
   if (isempty(c))
      return NULL;
      
   size_t len = strlen(c);

   size_t i;

   for (i = len; i > 0; i--)
      if ( isspace(c[i - 1]) )
         c[i - 1] = '\0';
      else
         break;

   size_t e = 0;

   for (i = 0; i < len; i++)
      if ( isspace(c[i]) )
         e = i + 1;
      else
         break;

   if (e >= len)
      return NULL;

   return &c[e];
};

size_t file2char(const char *filename, char *buffer, size_t size){
   if (isempty(filename))
      return 0;

   FILE *f = fopen(filename, "r");

   if (f == NULL){
      buffer[0] = '\0';
      return 0;
   }

   size_t act = fread(buffer, 1, size, f);
   buffer[act] = '\0'; //(: may?!?!? :)

   fclose(f);

   return act;
}

char *filegets(char *buffer, int max, FILE *f){
   char *tmp = fgets(buffer, max, f);

   if (isempty(tmp))
      return NULL;

   for (size_t i = strlen(buffer) - 1; i > 0; i--)
      if (buffer[i - 1] == '\n'){
         buffer[i - 1] = '\0';
         break;
      }

   return buffer;
};

char *strconcat(const char *x1,
 const char *x2,
 const char *x3,
                const char *x4,
 const char *x5, const char *x6){

   StringBuffer *sb = new StringBuffer();
   sb->add(x1);
 sb->add(x2);
 sb->add(x3);
  
 sb->add(x4);
 sb->add(x5);
 sb->add(x6);
   
   char *dest = strdupn( sb->get() );
 //Anti Warning ;)

   delete(sb);
   return dest;
};

void replacechar(char *buffer, char c1, char c2){
   if (isempty(buffer))
      return;
   for (size_t i = 0; i < strlen(buffer); i++)
      if (buffer[i] == c1)
         buffer[i] = c2;
}

char *itoa(int value, char *buff){
   sprintf(buff, "%d", value); //BUG?!?! \0
   return buff;
   //itoa(value, tmp, 10); //bc - ok; gcc - error ?!?!?
};

char *itoa(int value){
   static char tmp[50];
   return itoa(value, tmp);
};

char *ltoa(long int value, char *buff){
   sprintf(buff, "%ld", value);
   return buff;
};

char *ltoa(long int value){
   static char tmp[250];
   return ltoa(value, tmp);
};

char *ftoa(double value, char *buff){
   sprintf(buff, "%f", value);
   return buff;
};

char *ftoa(double value){
   static char tmp[50];
   return ftoa(value, tmp);
};

int setenv(const char *name, const char *value, char *buffer){
   free(buffer);
   buffer = strconcat(name, "=", value);
   int  i = putenv(buffer);
   //free(buffer);
   return i;
};

void StringBuffer::add(const char c){
   char tmp[] = {c, '\0'};
   add(tmp);
};

void StringBuffer::add(const char *c){
   if (isempty(c))
      return;

   char *tmp = (char *) malloc(strlen(data) + strlen(c) + 1);
   strcpy(tmp, data);
   strcat(tmp, c);
   free(data);
   data = tmp;
};

void StringBuffer::toUpperCase(){
   to_upper(data);
/*   if (!isempty(data))
        for (unsigned int i = 0; i < strlen(data); i++)
           data[i] = toupper(data[i]);*/
};

void StringBuffer::toLowerCase(){
   to_lower(data);
/*   if (!isempty(data))
        for (unsigned int i = 0; i < strlen(data); i++)
           data[i] = tolower(data[i]);*/
};

const char *StringTokenizer::nextToken(){
   if ((index >= len) ||
       (data == NULL))
      return NULL;

   int index1 = index;

   //Is there are better way with strpbk() ? May be not...
   while ((index < len                ) &&
          (!strchr(delim, data[index])))
      index++;

   lastdelimiter = data[index];
   data[index]   = '\0';
   index++;

   return &data[index1];
};

const char *StringTokenizer::nextToEnd(){
   return ((index >= len) || (data == NULL)) ? NULLs : &data[index];
};

//================================================================
/*
//Some old functions here, just for fun

boolean isempty(const char *c){
   if (c == NULL)
      return true;

   return (strlen(c) == 0);
};
char *strdupn(const char *c){
   if (isempty(c))
      return NULL;

   return strdup(c);
};

int  strcmpn(const char *c1, const char *c2){
   if ((c1 == NULL) ||
       (c2 == NULL))
      return 1;
   return strcmp(c1, c2);
};

const char *StringTokenizer::nextToEnd(){
   if ((index >= len) ||
       (data == NULL))
      return NULL;

   return &data[index];
};

*/
//================================================================
/*
//Sorry these doesn't works (stack need)
#define isempty(c)       ( (!c) ? true : !strlen(c) )
#define strdupn(c)       (isempty(c)) ? NULL : strdup(c)
#define strcmpn(c1, c2)  (isempty(c1) && isempty(c2)) ? 0 : (!c1 || !c2) ? 1 : strcmp (c1, c2)
#define stricmpn(c1, c2) (isempty(c1) && isempty(c2)) ? 0 : (!c1 || !c2) ? 1 : stricmp(c1, c2)
*/
