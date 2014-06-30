//Put CFG here
#ifndef CGI_CORE_CONFIG_H
#define CGI_CORE_CONFIG_H

#include "VERSION.h"

//Directory holds prop files
#ifdef __DOS__
//Are you think `c:\windows\system' is better?
#define MASTER_PROP_DIR "c:\\etc\\"
#else
#define MASTER_PROP_DIR "/etc/"
#endif

//Prop files in general

//The MASTER CFG file
#define dMASTER_CFG_PROP   MASTER_PROP_DIR"cgi_core."CGI_CORE_VERSION_BIG".cfg"
//The MASTER MACRO file
#define dMASTER_MACRO_PROP MASTER_PROP_DIR"cgi_core."CGI_CORE_VERSION_BIG".macro"

//The MASTER CFG file
#define MASTER_CFG_PROP   getenvd("MASTER_CFG_PROP",   dMASTER_CFG_PROP  )
//The MASTER MACRO file
#define MASTER_MACRO_PROP getenvd("MASTER_MACRO_PROP", dMASTER_MACRO_PROP)

//A_DBC info requied A_DBC included,
//remark it if you won't use it!
#include <db/a_dbc.h>

//You are security maniac?
//Define this for paranoic security!
#define PARANOIDSECURITY

#endif
