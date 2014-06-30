#include <stdio.h>

#include "module.cpy.h"

#include <mad/expr.h>

void CGICoreModule__IF__Process(Properties *, const char *);

CGICoreModule CGICoreModule__IF = {
	"if",
	"IF module",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	200,
	300,
	true,
	"if",
	NULL,
	NULL,
	CGICoreModule__IF__Process
};

void CGICoreModule__IFSTR__Process(Properties *, const char *);

CGICoreModule CGICoreModule__IFSTR = {
	"ifstr",
	"IF module for strings",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	200,
	300,
	true,
	"ifstr",
	NULL,
	NULL,
	CGICoreModule__IFSTR__Process
};

/****************************************************************/

static const char  *else_html_matrix[]={"else",  NULL};
static const char    *if_html_matrix[]={"if", "ifstr", NULL};

#include "separator_helper.i.cpp"

void CGICoreModule__IF__Process(Properties *p, const char *x){
   if (!p) return;
   if (!x) return;

   const char *op = p->get("OPERATION", "0");

   Calculator *c = new Calculator(op);
   boolean result = (boolean) c->calc();

   if (c->err)
      printflog(LOG_WARNING, "Module IF: Regular expression error...\n");

   delete(c);

   do_separator(result, x, else_html_matrix, if_html_matrix);
}

void CGICoreModule__IFSTR__Process(Properties *p, const char *x){
   if (!p) return;
   if (!x) return;

   const char *op = p->get("OPERATION");

   const char *d1 = p->get("STR1");
   const char *d2 = p->get("STR2");

   boolean result;

   //`ei' & `ni': Credit to Nedret <he@nedret.com>
   if   (!stricmpn(op, "eq" )) result = strcmpn (d1, d2) == 0;
   elif (!stricmpn(op, "ne" )) result = strcmpn (d1, d2) != 0;
   elif (!stricmpn(op, "ei" )) result = stricmpn(d1, d2) == 0;
   elif (!stricmpn(op, "ni" )) result = stricmpn(d1, d2) != 0;
   else                        result = strcmpn (d1, d2) == 0;

   //printf("--%s-(%s)-%s--%d--<br>\n", d1, op, d2, result);

   do_separator(result, x, else_html_matrix, if_html_matrix);
}
