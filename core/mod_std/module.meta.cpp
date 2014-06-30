#include <stdio.h>

#include "module.meta.h"

void CGICoreModule__META__Process(Properties *, const char *);

CGICoreModule CGICoreModule__META = {
	"META",
	"META-HTML control",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"htmlmeta",
	NULL,
	NULL,
	CGICoreModule__META__Process
};

//<meta http-equiv="refresh" 
//content="25;URL=index.html">

//<meta http-equiv="Set-Cookie" 
//content="cookievalue=xxx;expires=31-Dec-99;path=/">

void CGICoreModule__META__Process(Properties *x, const char *){
   if (!x) return;
   if (!x->exist("TYPE")) return;

   const char *t = x->get("TYPE");

   if    (!stricmpn(t, "refresh")){
      printout("<meta http-equiv=\"refresh\" content=\"");
      printout(x->get("TIME", "0"));
      printout(";URL=");
      printout(x->get("URL", ""));
      printout("\">\n");

   }elif (!stricmpn(t, "cookie")){
      printout("<meta http-equiv=\"Set-Cookie\" content=\"");
      printout(x->get("NAME", "X"));
      printout("=");
      printout(x->get("VALUE", ""));
      printout(";");
      if (x->exist("TIME")){
         printout("expires=");
         printout(x->get("TIME"));
         printout(";");
      }
      printout("path=");
      printout(x->get("PATH", "/"));
      printout("\">\n");

   }else
      printflog(LOG_ERROR, "Module META: Unknown meta-tag...\n");
};
