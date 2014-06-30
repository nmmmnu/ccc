/*
 * Standrd library with many functions for:
 *    1. DOS/Win/UNIX compatibility
 *    2. NULL Strings.
 *    3. boolean type :)
 *    4. String Buffer
 *    5. Java style String Tokenizer
 *
 * CopyLeft (c) 1998,99 by Nikolay Mijaylov
 *             "www.nmmm.nu" <nmmm@nmmm.nu>
 */

#ifndef _MAD_MAD_H
#define _MAD_MAD_H

#include "port.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG_FATAL_ERROR     1
#define LOG_ERROR           2
#define LOG_NON_FATAL_ERROR 3
#define LOG_WARNING         4
#define LOG_NOTIFY          5
#define LOG_NOTIFY_DEBUG    6

extern unsigned short int stdloglevel;
//stdlog looks fine :)
extern FILE *stdlog;

void printflog(unsigned short int level, const char *pattern, ...);
void printflog(const char *pattern, ...);
void printferr(const char *pattern, ...);

#ifndef MAX
#define MAX(a, b) ( a > b ? a : b )
#endif

#ifndef MIN
#define MIN(a, b) ( a < b ? a : b )
#endif

/*I'm still not sure do I need these
#define BUFF_1K  1024
#define BUFF_2K  BUFF_1K * 2
#define BUFF_4K  BUFF_1K * 4
#define BUFF_8K  BUFF_1K * 8
#define BUFF_16K BUFF_1K * 16
*/

//These *MUST* be inlines, not #defines
inline boolean isempty(const char *c){
   return (!c) ? true : (strlen(c) == 0);};
inline char *strdupn(const char *c){
   return (isempty(c)) ? NULLs : strdup(c);};
inline int  strcmpn (const char *c1, const char *c2){
   return (isempty(c1) && isempty(c2)) ? 0 : (!c1 || !c2) ? 1 : strcmp (c1, c2);};
inline int  stricmpn(const char *c1, const char *c2){
   return (isempty(c1) && isempty(c2)) ? 0 : (!c1 || !c2) ? 1 : stricmp(c1, c2);};

char *to_upper(char *c);
char *to_lower(char *c);

#define strtrim strcut
char *strcut(char *c);
size_t file2char(const char *filename, char *buffer, size_t size);
char *filegets(char *buffer, int max, FILE *f);
void replacechar(char *buffer, char c1, char c2);

//Don't smile :)))))))))))))
//I know how to fix it, but can you tell me WHY?!?!?!:)
char *strconcat(const char *x1 = NULL,
                const char *x2 = NULL,
                const char *x3 = NULL, const char *x4 = NULL,
                const char *x5 = NULL, const char *x6 = NULL);

//These *MUST* be inlines
inline int atoid(const char *s, int i){
   return isempty(s) ? i : atoi(s);};
inline long int atold(const char *s, long int i){
   return isempty(s) ? i : atoi(s);};
inline double atofd(const char *s, double i){
   return isempty(s) ? i : atoi(s);};
inline int ctoi(char value){
   int x = value - '0'; return (x < 1 || x > 9) ? 0 : x; };
inline char itoc(int value){
   return (value < 1 || value > 9) ? 0 : '0' + value; }; //Nedret Recep!!!

//This may be works under BC++
char *itoa(int value);
char *ltoa(long int value);
char *ftoa(double value);
//itoa with buffer, often for internal use of itoa...
char *itoa(int value,      char *buff); //Buff is max 15 chars :)
char *itoa(long int value, char *buff); //Buff is max ?? chars :)
char *ftoa(double value,   char *buff); //Buff is max ?? chars :)

int setenv(const char *name, const char *value, char *buffer);
#define getenvd(a, b) getenv(a) ? getenv(a) : b
/*This is much faster, but uses stack?
inline getenvd(const char *a, const char *b){
   const char x = getenv(a); return x ? x : b; }
*/

//noop() is used for macroses & functions
inline int noop(int a){
	return a;};
//this is for faster code
inline void noop(){
	return;};

class StringBuffer{
private:
   char    *data;
   boolean destroydata;
public:
   StringBuffer(boolean destroydata = true):data(strdup("")),
                                    destroydata(destroydata){};
   virtual ~StringBuffer(){
      if (destroydata)
         free(data);
   };
   void clear(){
      free(data);
      data = strdup("");
   };
   void add(const char c);
   void add(const char *c);
   const char *get(){
      return data;
   };

   void toUpperCase();
   void toLowerCase();
};

class StringTokenizer{
private:
   char   cbuffer[2];
   char  *data;
   const  char *delim;
   size_t len;
public:
   //This is *ONLY* for advanced use. Use this only if you
   //know what you exactly doing
   size_t index;

   //Use it after nextToken() to determine the delimiter
   //Unexperted value after retrieving last token (it's '\0').
   int lastdelimiter;

   //U can;t construct StringTokenizer with a NULL Pointer !!!
   StringTokenizer(const char *data, char delim):data(strdup(data)),
                                                 len(strlen(data)),
                                                 index(0),
                                                 lastdelimiter(0){
      cbuffer[0] = delim;
      cbuffer[1] = '\0';
      this->delim = cbuffer;
      /*
      //I don't want *ANY* warnings!!!
      char *boza = (char *) this->delim;

      boza[0] = delim;
      */
   };
   StringTokenizer(const char *data, const char *delim):
                               data(strdup(data)),
                               delim(delim),
                               len(strlen(data)),
                               index(0),
                               lastdelimiter(0){};
   virtual ~StringTokenizer(){
      free(data);
   };

   const char *nextToken();
   const char *nextToEnd();
};

#endif
//_MAD_MAD_H