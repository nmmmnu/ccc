#include <string.h>
#include <ctype.h>

#include "xml_parser.h"

//Finxing '\n' after all using HTML tags, if any.
//This is not on spec, but's cool
//#define  NEWLINEFIX

#define  OPENTAG  '<'
#define  CLOSETAG '>'
#define  OFFTAG   "/" /*must be string*/
#define  OFFTAGc  OFFTAG[0]

const char *NULL_XML_MATRIX[]={NULL};

boolean XMLParser::loadFromFile(const char *filename){
   pos = 0;
   len = file2char(filename, data, XMLMAXLEN);
   return (len > 0);
};

boolean XMLParser::loadFromString(const char *s){
   len = (isempty(s)) ? 0 : strlen(s);
   if (len > XMLMAXLEN)
      len = XMLMAXLEN;

   if (len)
      strncpy(data, s, len);

   data[len] = '\0';
   pos=0;
   return len > 0;
};

boolean XMLParser::eof(){
   return (pos >= len);
};

const char *XMLParser::seekChar(char ch){
   if (eof())
      return NULL;

   size_t pos1 = pos;
   size_t i;

   for (i = pos; data[i]!='\0'; i++)
       if (data[i] == '\\')
          i++;
       elif (data[i] == ch)
          break;
   pos=i;

   data[pos]='\0';
   return &data[pos1];
};

const char *XMLParser::seekNextTag(){
   return seekChar(OPENTAG);
};

const char *XMLParser::seekNextTag(const char *m[], const char *d[]){
   doublebuffer->clear();
   boolean f = true;

   while (f && !eof()){
      doublebuffer->add(seekNextTag());

      size_t pos1  = pos;
      const char *s  = getNextTag();
      const char *ds = NULL;
      size_t i;

      for(i = 0; d[i]; i++)
         if (cmpTag(s, d[i]) > 0){
            ds=extractDoubleTag(d[i], true);
            break;
         }

      const char *cs=d[i];

      for(i = 0; m[i]; i++)
         if (cmpTag(s, m[i]) > 0){
            f = false;
            break;
         }

      if (f){
         if (!isempty(s)){
            doublebuffer->add(OPENTAG);
            doublebuffer->add(s);
            doublebuffer->add(CLOSETAG);
	 }
	 if (!isempty(ds)){
	    doublebuffer->add(ds);
            doublebuffer->add(OPENTAG);
            doublebuffer->add(OFFTAG);
            doublebuffer->add(cs);
            doublebuffer->add(CLOSETAG);
         }
      }else
      	 pos=pos1;
   }
   return doublebuffer->get();
};

const char *XMLParser::getNextTag(){
   pos++;
   const char *s = seekChar(CLOSETAG);
   pos++;

#ifdef NEWLINEFIX
   //Finxing '\n' after all using HTML tags, if any.
   //This is not on spec, but's cool
   //Of course we shall support old DOS too '\r\n'
   if (data[pos]=='\r')      pos++;
   if (data[pos]=='\n')      pos++;
#endif

   return s;
};

char *XMLParser::cutTag(char *s){
   if (isempty(s))
      return NULL;

   boolean f = false;

   size_t i;

   for (i=0; i < strlen(s); i++)
      if (!isspace(s[i])){
         if (s[i] == OFFTAG[0])
            f=true;
         else
            break;
	}

   if (f){
      i--;
      s[i]=OFFTAG[0]; //No problem :)
   }
   
   size_t j;
   for (j=i; j < strlen(s); j++)
      if (isspace(s[j])) break;

   s[j]='\0';

   return &s[i];
};

int XMLParser::cmpTag(const char *tag1, const char *tag2){
   char *tag_1=strdupn(tag1);
   char *tag_2=strdupn(tag2);

   char *t1   =cutTag(tag_1);
   char *t2   =cutTag(tag_2);
   char *t1_  =strconcat(OFFTAG, t1);

   int x = 0;
   if (!stricmpn(t1, t2)) 
      x++;
   else if (!strcmpn(t1_, t2))
      x--;

   free(tag_1);
   free(tag_2);
   free(t1_);

   return x;
};

const char *XMLParser::extractDoubleTag(const char *tag, boolean rmode){
   doublebuffer1->clear();

   const char *tag1=NULL;
   int   f = 0;

   while ((f >= 0) && 
          (!eof())){
      if (!isempty(tag1)){
         doublebuffer1->add(OPENTAG);
         doublebuffer1->add(tag1);
         doublebuffer1->add(CLOSETAG);
      }
      doublebuffer1->add(seekNextTag());
      tag1=getNextTag();

      if (rmode)
         f = f + cmpTag(tag, tag1);
      else
         f = (cmpTag(tag, tag1)!=0) ? -1 : 0;
   }

   return doublebuffer1->get();
};

#define CLEN pos < len
#define eatspaces() for (; CLEN &&  isspace(tag[pos]); pos++);

void XMLParser::parseTag(const char *tag, Properties *p){
   if (isempty(tag))
      return;

   size_t len = strlen(tag);
   size_t pos = 0;

   eatspaces();

   StringBuffer *sb1 = new StringBuffer();

   //Fixing bugs like <A>text< / A>
   if (tag[pos] == OFFTAGc){
      sb1->add(OFFTAGc);
      pos++;
      eatspaces();
   }

   for (; CLEN && !isspace(tag[pos]); pos++)
      sb1->add(tag[pos]);

   p->put(XMLTAGPARSEKEY, sb1->get());

   StringBuffer *sb2 = new StringBuffer();

   while (CLEN){
      sb1->clear();
      sb2->clear();

      eatspaces();

      for (; CLEN && tag[pos]!='=' && !isspace(tag[pos]); pos++)
         sb1->add(tag[pos]);

      eatspaces();

      //This affect tags like <checkbox checked name="punk_rock_song">
      if (tag[pos] == '='){
         pos++; //for =


         eatspaces();

         if (tag[pos]!='\"')
            for (; CLEN && !isspace(tag[pos]); pos++)
               sb2->add(tag[pos]);
         else{
            pos++; //for ""
            for (; CLEN && tag[pos]!='\"'; pos++){
               /*   The following code make escape of some
                 characters, This is no element of standard
                 XML/HTML, but is very useful and do not
                 affect to language...

                 Example:
                    <P position="colona \"X\"">
                 Instead:
                    <P position="colona &quot;X&quot;">

                 Note:
                    There are base "unescape" too
               */
               if (tag[pos] == '\\')
                  if (CLEN)
                     pos++;

               sb2->add(tag[pos]);
            }
         }
      }

      sb1->toUpperCase();
      p->put(sb1->get(), sb2->get());
   }
}
