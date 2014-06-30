/****************
*               *
* CGI_Meta_Core *
*               *
****************/

#include <mad/mad.h>
#include <mad/prop.h>
#include <mad/parsers.h>

#include "main.h"

#include "VERSION.h"
#include "config.h"
#include "parser.h"
#include "modules.h"

Properties *cfg         = new Properties();

Properties *query_macro = new Properties();             //query default
Properties *query       = new Properties(query_macro);  //query string
Properties *macro       = new Properties(query);

inline void init(){
#ifdef __MFC__
   if (!initMFC()){
      printf("content-type: text/html\n\n"
         "<html>\n<body>\n"
         "<h1>MFC Crashed...</h1>\n"
         "<h2>Oh, dear!</h2>\n"
         "</body>\n</html>\n");

      exit(100);
   }
#endif

     cfg->load( MASTER_CFG_PROP   );
   macro->load( MASTER_MACRO_PROP );

   stdloglevel=cfg->getAsInt("loglevel", LOG_ERROR);

   if (cfg->exist("logfile")){
      stdlog=fopen(cfg->get("logfile"), "a");
      if (!stdlog){
         stdlog=stderr;
         printflog(LOG_WARNING, "main:Heeeeeeeelp cannot open stdlog!!!!!\n");
      }
   }
}

void done(){
   donemodules();
   if (cfg->exist("logfile"))
      fclose(stdlog);

   delete(macro);
   delete(query);
   delete(query_macro);

   delete(cfg);
}

const char *the_file         = getenv("PATH_TRANSLATED");
boolean     want_http_header = true;

#include "arg.i.cpp"
#include "http.i.cpp"
#include "paranoid.i.cpp"

int main(int argc, char *argv[]){
   lookUpArg(argc, argv);

   init();

   atexit(done);

   lookCookies();
   lookQueryStr();
   lookParanoidSecure();

   parseFromFile(the_file);

   //Prevantive, if file is empty
   placeHTTPHeader();

   return 0;
}
