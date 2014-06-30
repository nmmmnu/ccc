#ifndef setup_modules_H
#define setup_modules_H

const char *cgi_core_modules_set = 
            "HTML Preparser"
const char *cgi_core_modules_set_description = 
            "Designed for preparsing HTML files. Exclude all \"BAD\" tags."

#include "../modules.h"

#include "module.null.h"

CGICoreModule *module_matrix[] = {

	&CGICoreModule__HEAD,
	&CGICoreModule__TITLE,
	&CGICoreModule__SCRIPT,
	&CGICoreModule__STYLE,

	NULL
};

#endif
