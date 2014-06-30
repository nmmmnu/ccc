#include <stdio.h>

#include "module.demo.h"

void CGICoreModule__DEMO__Process(Properties *, const char *);

CGICoreModule CGICoreModule__DEMO = {
	"demo",
	"This is a demonstration module!",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"free",
	100,
	300,
	false,
	"demo",
	NULL,
	NULL,
	CGICoreModule__DEMO__Process
};

void CGICoreModule__DEMO__Process(Properties *, const char *){
   parseFile("--==(CGI_Core)==--");
};
