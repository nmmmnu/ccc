#include "module.type.h"

#include "../main.h"

void CGICoreModule__TYPE__Process(Properties *, const char *);

CGICoreModule CGICoreModule__TYPE = {
	"type",
	"Change HTTP Content-type",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"content-type",
	NULL,
	NULL,
	CGICoreModule__TYPE__Process
};

void CGICoreModule__TYPE_TYPE__Process(Properties *, const char *);

CGICoreModule CGICoreModule__TYPE_XML = {
	"type.xml",
	"Change HTTP Content-type to XML",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"content-type.xml",
	NULL,
	NULL,
	CGICoreModule__TYPE_TYPE__Process
};

CGICoreModule CGICoreModule__TYPE_TEXT = {
	"type.text",
	"Change HTTP Content-type to plain text",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"content-type.text",
	NULL,
	NULL,
	CGICoreModule__TYPE_TYPE__Process
};

CGICoreModule CGICoreModule__TYPE_HDML = {
	"type.hdml",
	"Change HTTP Content-type to HDML",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"content-type.hdml",
	NULL,
	NULL,
	CGICoreModule__TYPE_TYPE__Process
};

CGICoreModule CGICoreModule__TYPE_WML = {
	"type.hdml",
	"Change HTTP Content-type to WML",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"content-type.wml",
	NULL,
	NULL,
	CGICoreModule__TYPE_TYPE__Process
};

void CGICoreModule__REDIRECT__Process(Properties *, const char *);

CGICoreModule CGICoreModule__REDIRECT = {
	"HTTP redirect",
	"HTTP redirection",
	"(c) 01.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	false,
	"redirect",
	NULL,
	NULL,
	CGICoreModule__REDIRECT__Process
};

void CGICoreModule__TYPE__Process(Properties *p, const char *){
   if (!p               ) return;
   if (!p->exist("TYPE")) return;

   placeHTTPHeader(p->get("TYPE"));
};

void CGICoreModule__TYPE_TYPE__Process(Properties *x, const char *){
   const char *xx = x->get(XMLTAGPARSEKEY); //Speedup?:)
   const char *t  = NULL;

   if   (!stricmpn(xx, CGICoreModule__TYPE_XML.tagProcess))
      t = "text/xml";
   elif (!stricmpn(xx, CGICoreModule__TYPE_HDML.tagProcess))
      t = "text/x-hdml";
   elif (!stricmpn(xx, CGICoreModule__TYPE_WML.tagProcess))
      t = "text/vnd.wap.wml";
   elif (!stricmpn(xx, CGICoreModule__TYPE_TEXT.tagProcess))
      t = "text/plain";

   if (t)
      placeHTTPHeader(t);
}

void CGICoreModule__REDIRECT__Process(Properties *p, const char *){
   if (!p                   ) return;
   if (!p->exist("LOCATION")) return;

   if (f_placeHTTPHeader)
      printf("location: %s\n\n", p->get("LOCATION"));
}
