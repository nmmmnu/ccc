#include <stdio.h>

#include "module.for.h"

void CGICoreModule__FOR__Process(Properties *, const char *);
void CGICoreModule__WHILE__Process(Properties *, const char *);

CGICoreModule CGICoreModule__FOR = {
	"for",
	"Pascal like FOR",
	"(c) 02.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"for",
	NULL,
	NULL,
	CGICoreModule__FOR__Process
};

CGICoreModule CGICoreModule__WHILE = {
	"while",
	"Pascal like while (beta)",
	"(c) 02.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"while",
	NULL,
	NULL,
	CGICoreModule__WHILE__Process
};

void CGICoreModule__FOR__Process(Properties *x, const char *data){
   if (!data            ) return;
   if (!x               ) return;
   if (!x->exist("FROM")) return;
   if (!x->exist("TO"  )) return;

   for (long int i = x->getAsInt("FROM"); i <= x->getAsInt("TO"); i++){
      if (x->exist("NAME"))
         macro->putAsInt(x->get("NAME"), i);
      parseFile(data);
   }
};

void CGICoreModule__WHILE__Process(Properties *x, const char *data){
   if (!x    ) return;
   if (!data ) return;

   while (x->getAsInt("NAME"))
      parseFile(data);
};
