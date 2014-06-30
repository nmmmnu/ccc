#include "stat.i.cpp"

#define PARAM_HELP    "help"
#define PARAM_FILE    "file"
#define PARAM_HEAD    "no-head"
#define PARAM_VERSION "version"
#define PARAM_DB      "a_dbc"
#define PARAM_MOD     "modules"
#define PARAM_DET_MOD "detail-modules"

void printhelp(){
   const char *frm = "--%-15s - %s\n";
   printf(frm, PARAM_HELP,    "this help screen");
   printf(frm, PARAM_HEAD,    "do not output HTTP header");
   printf(frm, PARAM_FILE" <file>",    "use <file> instead PATH_TRANSLATED");
   printf(frm, PARAM_VERSION, "show version");
   printf(frm, PARAM_DB,      "show installed A_DBC");
   printf(frm, PARAM_MOD,     "show summary information about installed modules");
   printf(frm, PARAM_DET_MOD, "show detail information for each installed module");

   printf("\nImpotant Remark:\n");
   printf("\tThe A_DBC project is no longer integrated in CGI_Core.\n");
   printf("\tIt is part of CGI_Core modules.\n");
   printf("\tYou can found list of supported databases, online at:\n");
   printf("\t\thttp://www.nmmm.nu/linux/a_dbc/\n\n");
}

void printversion(){
   printf("\nCGI Core (TM) Version %s\n", CGI_CORE_VERSION);
   printf("\tInternal number: %d\n", CGI_CORE_VERSION_NUM);
#ifdef __USER__
   printf("\tCompiled: %s, %s from %s\n", __DATE__, __TIME__, __USER__);
#else
   printf("\tCompiled: %s, %s\n", __DATE__, __TIME__);
#endif

   printf("\tModules set: %s\n", cgi_core_modules_set);

#ifdef PARANOIDSECURITY
   printf("\tSecurity: PARANOID\n");
#endif
#ifdef _A_DBC_H
   printf("\tA_DBC: Installed\n");
#endif
   printf("(c) 1998 - %s, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>\n", CGI_CORE_VERSION_DATE);
   printf("\tAll rights reserved.\n\n");
}

void printdb(){
#ifdef _A_DBC_H
   // Ok this mean we aleady have included A_DBC
   const char *frm = "%-15s - %s\n";

   printf(frm, "A_DBC version", a_dbc_version);
   printf(frm, "A_DBC home",  "http://www.nmmm.nu/linux/a_dbc/");
   printf(frm, "Driver",       a_dbc_drivername);
   printf(frm, "Copyright",    a_dbc_drivercopyright);
   printf(frm, "Distribution", a_dbc_driverdistribution);
#else
   printf("No A_DBC support...\n");
   printf("Find info online at\n");
   printf("\thttp://www.nmmm.nu/linux/a_dbc/\n");
#endif
}

#define look_arg(a) ( !strcmp(opt, "--"a) || !strcmp(opt, "-"a) )
#define proc_arg(a, b) if (look_arg(a)){ printversion(); b; exit(0); }

void lookUpArg1(char *opt, char *opt1){
   proc_arg(PARAM_HELP,    printhelp()   );
   proc_arg(PARAM_VERSION, NOOP          );
   proc_arg(PARAM_MOD,     moduleslist() );
   proc_arg(PARAM_DET_MOD, modulesshow() );
   proc_arg(PARAM_DB,      printdb()     );

   if (look_arg(PARAM_HEAD))
      want_http_header=false;

   if (look_arg(PARAM_FILE)){
      if (!opt1){
         printferr("Filename missed!\n");
         exit(0);
      }
      the_file=opt1;
   }
}

void lookUpArg(int argc, char *argv[]){
   for (int i = 0; i < argc; i++)
      lookUpArg1(argv[i], i + 1 < argc ? argv[i + 1] : NULL);
};
