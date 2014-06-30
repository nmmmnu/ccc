#include <stdio.h>

#include "module.version.h"

#include "../VERSION.h"

void CGICoreModule__VERSION__Process(Properties *, const char *);

CGICoreModule CGICoreModule__VERSION = {
	"version",
	"Show CGI_Core Version (demo module)",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"free",
	100,
	300,
	false,
	"version",
	NULL,
	NULL,
	CGICoreModule__VERSION__Process
};

void CGICoreModule__VERSION__Process(Properties *, const char *){
   parseFile(CGI_CORE_VERSION" ("CGI_CORE_VERSION_DATE")");
};
