#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mad/coder.cpp>

#define PROGNAME "shcode"

void help(){
   printf("\nShCODEr Version 1.0\n");
   printf(  "===================\n");
   printf("CopyLeft (c) 12.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>\n\n");

   printf("Usage:\n");
   printf("\t%s \"string-data\" --[encoding]\n", PROGNAME);
   printf("or\n");
   printf("\t%s[.encoding] \"string-data\"\n", PROGNAME);

   printf("While encoding is one of following:\n");

   const char *form = "\t%-5s %s\n";

   printf(form, "urld", "URLDecode");
   printf(form, "urle", "URLEncode");
   printf(form, "HTML", "(Hard)HTMLEncode");
   printf(form, "html", "(Soft)HTMLEncode");
   printf(form, "PHTML","(Hard)PHP HTMLEncode");
   printf(form, "phtml","(Soft)PHP HTMLEncode");
   printf(form, "cppe", "CPPEncode");
   printf(form, "she",  "ShellEncode");
   printf(form, "cppd", "CPPDecode");

   printf("\n");

   exit(1);
}

#define pcomp(a)			\
	!strcmp(param, "--"a) ||	\
	!strcmp(param,  "-"a) ||	\
	strstr(argv[0], PROGNAME"."a)

Coder *co = new Coder();

void fre(){
   delete(co);
}

int main(int argc, char *argv[]){
   if (argc < 2)
      help();

   atexit(fre);

   const char *result = "sorry not implemented yet!";
   const char *param  = argv[2] ? argv[2] : ""; //NULL fixing
   const char *data   = argv[1];

   if   (pcomp("urld"))  result = co->URLDecode(data);
   elif (pcomp("urle"))  result = co->URLEncode(data);
   elif (pcomp("HTML"))  result = co->HTMLEncode(data);
   elif (pcomp("html"))  result = co->SoftHTMLEncode(data);
   elif (pcomp("PHTML")) result = co->HTMLEncode(data, true);
   elif (pcomp("phtml")) result = co->SoftHTMLEncode(data, true);
   elif (pcomp("cppe"))  result = co->CPPEncode(data);
   elif (pcomp("she" ))  result = co->CPPEncode(data, true);
   elif (pcomp("cppd"))  result = co->CPPDecode(data);
   else{
      printferr("Unknown enconding type.\n");
      exit(1);
   }

   printf("%s\n", result);

   return 0;
}
