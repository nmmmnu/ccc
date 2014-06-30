#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "module.version.h"

#include "../macrocodereplacer.h"

#include <mad/expr.h>


void CGICoreModule__DEFINE__Process(Properties *, const char *);

CGICoreModule CGICoreModule__DEFINE = {
	"define",
	"C/C++ like #define(s)",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"define",
	NULL,
	NULL,
	CGICoreModule__DEFINE__Process
};

/*******************************************/

CGICoreModule CGICoreModule__DEFINEEXPR = {
	"define reg.expr",
	"regular expression #define(s)",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"expr",
	NULL,
	NULL,
	CGICoreModule__DEFINE__Process
};

/*******************************************/

CGICoreModule CGICoreModule__DEFINEENV = {
	"getenv",
	"Environment string #define(s)",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"getenv",
	NULL,
	NULL,
	CGICoreModule__DEFINE__Process
};

/*******************************************/

CGICoreModule CGICoreModule__DEFINEAREA = {
	"adv. define",
	"Advanced #define(s) with double tag",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"adefine",
	NULL,
	NULL,
	CGICoreModule__DEFINE__Process
};

/*******************************************/

CGICoreModule CGICoreModule__DEFINETIME = {
	"time",
	"Show time in format strftime()",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"time",
	NULL,
	NULL,
	CGICoreModule__DEFINE__Process
};

/*******************************************/

CGICoreModule CGICoreModule__DEFINEINDEX = {
	"indexdefine",
	"Define virtual field $[$[xxx]] fix",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"idefine",
	NULL,
	NULL,
	CGICoreModule__DEFINE__Process
};

#define TIMEBUFFER 1024

void CGICoreModule__DEFINE__Process(Properties *x, const char *areadata){
   if (!x) return;
   if (!x->exist("NAME")) return;
   
   Properties *p;
   const char *type= x->get("TYPE");

   if (!stricmpn(type,   "QUERY"))
      p=query_macro;
   elif (!stricmpn(type, "QUERYSTR"))
      p=query;
   elif (!stricmpn(type, "CFG"))
      p=cfg;
   elif (!stricmpn(type, "MACRO"))
      p=macro;
   else
      p=macro;

   if (!stricmpn(x->get(XMLTAGPARSEKEY), CGICoreModule__DEFINE.tagProcess)){
      p->put(x->get("NAME"), x->get("VALUE"));
      return;
   }

   if (!stricmpn(x->get(XMLTAGPARSEKEY), CGICoreModule__DEFINEINDEX.tagProcess)){
      p->put(x->get("NAME"), p->get(x->get("VALUE")) );
      return;
   }

   if (!stricmpn(x->get(XMLTAGPARSEKEY), CGICoreModule__DEFINEEXPR.tagProcess)){
      Calculator *ca = new Calculator(x->get("VALUE", ""));
      float fl = (float) ca->calc();
      if (ca->err)
         printflog(LOG_WARNING, "Module DEFINE_EXPR: Regular expression error...\n");
      delete(ca);

      p->put(x->get("NAME"), ftoa(fl));
      return;
   }

   if (!stricmpn(x->get(XMLTAGPARSEKEY), CGICoreModule__DEFINEENV.tagProcess)){
      p->put(x->get("NAME"), getenv(x->get("VALUE", "")));
      return;
   }

   if (!stricmpn(x->get(XMLTAGPARSEKEY), CGICoreModule__DEFINETIME.tagProcess)){
      tzset();  //Set some env?
      time_t secs_now = time(NULL);
      struct tm *time_now = localtime( &secs_now );
      char str[TIMEBUFFER];
      strftime(str, TIMEBUFFER, p->get("VALUE", "%H:%M:%S, %d-%b-%Y"), time_now);

      p->put(x->get("NAME"), str);

      return;
   }

   if (isempty(areadata)) return;

   MacroCodeReplacer *mr = new MacroCodeReplacer(macro);
   p->put(x->get("NAME"), mr->replace(areadata));
   delete(mr);
};
