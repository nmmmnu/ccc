/*=========================
Fake Data Base Connector II
===========================
This is a connection to the FAKE DataBase.
FAKE DataBase is a text file.
First row must holds field names, delimited with <TAB>.
Next rows hold the data, delimited with <TAB> too.
Files like this can be easy created with many databases,
electonic tables (spreadsheets) etc.

FAKE Connector dump filtred data!!!
the `SQL' field is FILTER. It's like HTTP Query String:
	`field_name1=value1&field_name2=value2&...'
the FILTER rules:
	(o) the comparation is `=' (equal to)
	(o) the logic is `AND' (&&)

FAKE Connector can only ADD a row to `table'
the `SQL' field is row to ADD. It's like HTTP Query String:
	`field_name1=value1&field_name2=value2&...'
the ADD rules:
	(o) It is NOT nessacary that you have all fields.

Author:   Nikolay Mijaylov Mijaylov
CopyLeft: 1998 - 1999, Sofia
History:
	12.1999 - "little" core dump fix (: it was NULL string :))
	07.1999 - Finished `ADD' (I was too busy to finish it:)))
	06.1999 - Strong rebuild, Added Filtering
	**.**** - Some really small changes
	10.1998 - Build
*/

#include "a_dbc.h"
#include <mad/parsers.h>
#include <ctype.h>

#define MAXLEN 5000

#define DBC_DATABASE "DATABASE"

class FakeDBConnection: public ADBConnection{
public:
   const char *db_name;
   
   boolean connect(Properties *cfg){ 
      db_name = cfg->get(DBC_DATABASE, "");
      return true; 
   }
   void disconnect(){};

   ADBC *getADBC();
};

const char *a_dbc_drivername         = "MAD Software Fake (text db) driver";
const char *a_dbc_drivercopyright    = "(c) 1998 - 99, Nikolay Mijaylov";
const char *a_dbc_driverdistribution = "GPL";

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class FakeDBC: public ADBC{
protected:
   FILE       *f;
   char       *fields;
   char       *data;
   Properties *query;
   
   const char *extract(unsigned int no, const char *str1, char *buffer);
   boolean isIt(const char *name, const char *value);
   void read_next();
public:
   FakeDBC(ADBConnection *DBConnection):ADBC(DBConnection),
                                        data(NULL),
                                        query(new Properties()){};
   ~FakeDBC(){
      if (f) fclose(f); //Bugfix:)
      delete(query);
   };
   
   void setSQL(const char *sql);

   boolean exec();
   boolean open();
   void close(){}

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

#define DB_NAME ((FakeDBConnection *) DBConnection)->db_name

inline boolean FakeDBC::isIt(const char *name, const char *value){
   return query->exist(name) ? !strcmp(query->get(name), value) : true;
}

void FakeDBC::setSQL(const char *sql){
   f = fopen(DB_NAME, "r");

   if (!f)
      return;

   char tmp[MAXLEN];
   if (filegets(tmp, MAXLEN, f) != NULL)
      fields = strdupn(tmp);
   else
      fields = NULL;

   //Like Query String, but without encoding :) ...
   StringParser *qsp = new StringParser(query, sql, '&', '=');
   qsp->parse();
   delete(qsp);

   //printf(">>>%s\n", sql);
   //query->dump();
};

boolean FakeDBC::exec(){
   FILE *ff = fopen(DB_NAME, "a");

   if (!ff)
      return false;

   for (unsigned int i = 0; i < getFieldCount(); i++){
      if (i != 0)
         fprintf(ff, "\t");
      if (query->get(getFieldName(i)))
         fprintf(ff, "%s", query->get(getFieldName(i)) );
   }
   
   fprintf(ff, "\n");
   
   fclose(ff);
   
   return true;
};

boolean FakeDBC::open(){
   if (!f)
      return false;

   if (!fields)
      return false;

   next();

   return true;
};

void FakeDBC::read_next(){
   if (data != NULL)
      free(data);

   char tmp[MAXLEN];
   if (filegets(tmp, MAXLEN, f) != NULL)
      data = strdupn(tmp);
   else
      data = NULL;   

};

void FakeDBC::next(){
   while(true){

      if ( !eof() )
         read_next();
      else
         return;

      if (eof())
         return;

      boolean b = true;
      for (unsigned int i = 0; i < getFieldCount(); i++)
         if ( !isIt(getFieldName(i), getFieldData(i)) )
            b = false;

      //printf(">><%d>%s<\n", eof(), data);

      if (b)
         return;
   }
};

inline boolean FakeDBC::eof(){
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

   for (unsigned int num = 0; num < no; num++){
      const char *c = st->nextToken();
      sprintf(buffer, "%s", c ? c : "" );
   }

   delete(st);

   if (buffer)
      for (size_t i = strlen(buffer); i > 0; i--)
         if (isspace(buffer[i - 1]))
            buffer[i - 1] = '\0';
         else 
            break;

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
