#include <stdio.h>

#include <xml/xml_parser.h>

#include <mad/prop.h>

#define STR "111<if>222<if a=3>333<else>444</if>555"

int main(){
   XMLParser *xml = new XMLParser();
   Properties *p = new Properties();

   xml->loadFromString(STR);

   printf("=%s=\n", STR);
/*
   while(!xml->eof()){
      if (!xml->eof()) printf("S>-%s-\n", xml->seekNextTag( (char *[]){"Mar","Bar","Var", NULL} ) );

      printf("T>-%s-\n", x);

      if (!xml->eof()) printf("D>-%s-\n", xml->extractDoubleTag("Br") );
   }
   printf("-%s-\n", xml->extractDoubleTag("else", false) );
*/

   printf("1)=-%s-\n", xml->seekNextTag());
   printf("2)=-%s-\n", xml->getNextTag());
   printf("1)=-%s-\n", xml->seekNextTag());
   printf("2)=-%s-\n", xml->getNextTag());
   printf("1)=-%s-\n", xml->seekNextTag());
   printf("2)=-%s-\n", xml->getNextTag());
   printf("1)=-%s-\n", xml->seekNextTag());
   printf("2)=-%s-\n", xml->getNextTag());

   //printf("1)=-%s-\n", xml->seekNextTag( (const char *[]){"if", NULL} ) );
   //printf("2)=-%s-\n", xml->getNextTag());
   //printf("3)=-%s-\n", xml->seekNextTag( (const char *[]){"else", NULL}, (const char *[]){"if", NULL} ) );
   
   printf("\n");

   delete(p);
   delete(xml);

   return 0;
}
