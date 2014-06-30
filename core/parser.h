//Provide interface to parsing for all modules

#ifndef CGI_CORE_PARSER_H_
#define CGI_CORE_PARSER_H_

//Lets childs knows about XML
#include <mad/xml_parser.h>

extern boolean echo_control;

void parseFromFile(const char *);
void parseFile(const char *);
char *getDataOut(const char *);
void printout(const char *);

#endif
