#include <stdio.h>

#include "module.cpy.h"

void CGICoreModule__CPY__Process(Properties *, const char *);
void CGICoreModule__ASIS__Process(Properties *, const char *);

CGICoreModule CGICoreModule__CPY = {
	"cpy",
	"Obsolete CGI_Core cpy module",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"cpy",
	NULL,
	NULL,
	CGICoreModule__CPY__Process
};

CGICoreModule CGICoreModule__ASIS = {
	"asis",
	"Copy data `as is'",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"asis",
	NULL,
	NULL,
	CGICoreModule__ASIS__Process
};

void CGICoreModule__CPY__Process(Properties *, const char *x){
   parseFile(x);
}

void CGICoreModule__ASIS__Process(Properties *, const char *x){
   printout(x);
}
