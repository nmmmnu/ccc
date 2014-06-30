/*=========================
Fake Data Base Connector II
===========================
This is a connection to the FAKE DataBase.
FAKE DataBase is a text file.
First row must holds field names, delimited with <TAB>.
Next rows hold the data, delimited with <TAB> too.
FAKE Connector always DUMP ALL DATA!!!

Author:   Nikolay Mijaylov Mijaylov
CopyLeft: 10.1998, Sofia
*/

#include "a_dbc.h"
#include "../mad/mad.h"
#include "../mad/prop.h"

#define MAXLEN 5000

class FakeDBConnection: public ADBConnection{
public:
   boolean connect(Properties *cfg){ return true; };
   void disconnect(){};

   const char *DriverName()         { return "MAD Software text db driver"; };
   const char *DriverCopyright()    { return "(c) 1998 - 99, Nikolay Mijaylov"; };
   const char *DriverDistribution() { return "GPL"; };

   ADBC *getADBC();
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class FakeDBC: public ADBC{
protected:
   FILE *f;
   char *fields;
   char *data;
   const char *extract(unsigned int no, const char *str1, char *buffer);
public:
   FakeDBC(ADBConnection *DBConnection):ADBC(DBConnection){};

   void setSQL(const char *sql);

   boolean exec(){ return false; }; //because U cannot execute this, for now
   boolean open();
   void close();

   void next();
   boolean eof();
   
   unsigned int getFieldCount();
   const char *getFieldName(unsigned int no);
   //const char getFieldType(unsigned int no);
   const char *getFieldData(unsigned int no);
};

ADBConnection *getADBConnection(){ return new FakeDBConnection(); };
ADBC *FakeDBConnection::getADBC(){ return new FakeDBC(this); };

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FakeDBC::setSQL(const char *sql){
   f = fopen(sql, "r"); //Fake db 'SQL' is only name of file to dump :)
   
   if (!f)
      return;
      
   char tmp[MAXLEN];
   if (filegets(tmp, MAXLEN, f) != NULL)
      fields = strdupn(tmp);
   else
      fields = NULL;
};

boolean FakeDBC::open(){
   if (!f)
      return false;

   if (!fields)
      return false;
      
   next();
   return true;
};

void FakeDBC::close(){
   fclose(f);
};

void FakeDBC::next(){
   if (data != NULL)
      free(data);
         
   char tmp[MAXLEN];
   if (filegets(tmp, MAXLEN, f) != NULL)
      data = strdupn(tmp);
   else
      data = NULL;   
};

boolean FakeDBC::eof(){
   return feof(f) && (data == NULL);
};

unsigned int FakeDBC::getFieldCount(){
   StringTokenizer *st  = new StringTokenizer(fields,'\t');

   unsigned int num = 0;
   while (st->nextToken() != NULL)
      num++;
      
   delete(st);
   
   return num;
};

const char *FakeDBC::extract(unsigned int no, const char *str1, char *buffer){
   sprintf(buffer, "%s", "");
   StringTokenizer *st = new StringTokenizer(str1, '\t');

   for (unsigned int num = 0; num < no; num++)
      sprintf(buffer, "%s", st->nextToken() );
/*
printf("===============\n");
printf("num>%d\n", no);
printf("str>%s\n", str1);
printf("buf>%s\n", tmp);
printf("===============\n");
*/
   delete(st);

   return buffer;
};

const char *FakeDBC::getFieldName(unsigned int no){
   static char res[MAXLEN];
   return extract(no + 1, fields, res);
};

const char *FakeDBC::getFieldData(unsigned int no){
   static char res[MAXLEN];
   return extract(no + 1, data, res);
};
