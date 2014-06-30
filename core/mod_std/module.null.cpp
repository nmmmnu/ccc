#include <stdio.h>

#include "module.null.h"

#include "../parser.h"

CGICoreModule CGICoreModule__NULL = {
	"NULL",
	"NULL (empty, NOOP) module",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"null",
	NULL,
	NULL,
	NULL
};

void CGICoreModule__ECHO__Process(Properties *, const char *);

CGICoreModule CGICoreModule__ECHO = {
	"echo",
	"Control over output",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"echo",
	NULL,
	NULL,
	CGICoreModule__ECHO__Process
};

void CGICoreModule__ECHO__Process(Properties *p, const char *){
   if (!p){
      echo_control = false;
      return;
   }

   echo_control = p->getAsInt("MODE") != 0;
}
