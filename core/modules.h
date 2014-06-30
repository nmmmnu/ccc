//Describe module object
//(actualy it is structure:)

#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include <mad/mad.h>
#include <mad/prop.h>

struct CGICoreModule{
   const char  *name;
   const char  *description;
   const char  *copyright;
   const char  *distribution;

   //Ver format: XYZ -> X.Y.Z
   //Example:    120 -> 1.2.0
   unsigned int version;
   unsigned int needVersion;

   boolean      doubleTagProcess;
   const char  *tagProcess;

   void        (*s_process)();
   void        (*e_process)();
   void        (*process)(Properties *, const char *);
};

//Dedicated to Matrix movie ;)

//This matrix describe all modules
extern CGICoreModule *module_matrix[];

//This matrix is used with XML parser,
//to avoid parsing each HTML tag.
extern const char **html_matrix;

//Modules info
extern const char *cgi_core_modules_set;
extern const char *cgi_core_modules_set_description;

//config-properties
extern Properties *cfg;

//macro-properties
extern Properties *query_macro;
extern Properties *query;
extern Properties *macro;

//Done modules
void donemodules();

#endif
