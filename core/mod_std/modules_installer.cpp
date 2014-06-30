#ifndef setup_modules_H
#define setup_modules_H

const char *cgi_core_modules_set = 
      "Standard";
const char *cgi_core_modules_set_description = 
      "This moduleset is designed for easy implementing "
      "applications that runs on top of HTTPd";

#include "../modules.h"

//#include "module.demo.h"
//#include "module.version.h"
#include "module.null.h"
#include "module.cpy.h"
#include "module.shell.h"
#include "module.pipe.h"
//#include "module.pipe2.h"
#include "module.include.h"
#include "module.define.h"
#include "module.if.h"
#include "module.db.h"
#include "module.meta.h"
#include "module.type.h"
#include "module.for.h"

CGICoreModule *module_matrix[] = {

	//&CGICoreModule__DEMO,
	//&CGICoreModule__VERSION,
	&CGICoreModule__NULL,
	&CGICoreModule__ECHO,
	&CGICoreModule__CPY,
	&CGICoreModule__ASIS,
	&CGICoreModule__SHELL,
	//&CGICoreModule__PIPE2,
	//&CGICoreModule__PERL2,
	//&CGICoreModule__MAIL2,
	&CGICoreModule__PIPE,
	&CGICoreModule__PERL,
	&CGICoreModule__MAIL,
	&CGICoreModule__INCLUDE,
	&CGICoreModule__DEFINE,
	&CGICoreModule__DEFINEEXPR,
	&CGICoreModule__DEFINEENV,
	&CGICoreModule__DEFINETIME,
	&CGICoreModule__DEFINEAREA,
	&CGICoreModule__DEFINEINDEX,
	&CGICoreModule__IF,
	&CGICoreModule__IFSTR,
	&CGICoreModule__DBEXE,
	&CGICoreModule__DBSEL,
	&CGICoreModule__META,
	&CGICoreModule__TYPE,
	&CGICoreModule__TYPE_TEXT,
	&CGICoreModule__TYPE_XML,
	&CGICoreModule__TYPE_HDML,
	&CGICoreModule__TYPE_WML,
	&CGICoreModule__REDIRECT,
	&CGICoreModule__FOR,
	&CGICoreModule__WHILE,

	NULL
};

#endif
