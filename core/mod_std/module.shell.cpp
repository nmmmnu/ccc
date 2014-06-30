#include <stdio.h>
#include <stdlib.h>

#include "module.shell.h"

void CGICoreModule__SHELL__Process(Properties *, const char *);

CGICoreModule CGICoreModule__SHELL = {
	"shell",
	"Shell module (obsolete system module)",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"shell",
	NULL,
	NULL,
	CGICoreModule__SHELL__Process
};

void CGICoreModule__SHELL__Process(Properties *, const char *x){
   fflush(stdout); //prevent output bug
   char *s = getDataOut(x);
   system(s);
   free(s);
   fflush(stdout); //prevantive
};
