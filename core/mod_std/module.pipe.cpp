#include <stdio.h>
#include <stdlib.h>

#ifndef __DOS__
#include <signal.h>
#endif

#include "module.pipe.h"

#define BIN_PERL              "perl"
#define BIN_SENDMAIL          "sendmail -t -i"

#define PIPENOTIMPLEMENTED    "Non Fatal Error: Module PIPE: Sorry, "\
                              "PIPES aren't implemented"

void CGICoreModule__PIPE__Process(Properties *, const char *);
void CGICoreModule__PERL__Process(Properties *, const char *);
void CGICoreModule__MAIL__Process(Properties *, const char *);

CGICoreModule CGICoreModule__PIPE = {
	"FIFO",
	"POSIX FIFO (pipe) module",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"pipe",
	NULL,
	NULL,
	CGICoreModule__PIPE__Process
};

CGICoreModule CGICoreModule__PERL = {
	"Perl",
	"FIFO to Perl interpretator. (Ability to run Perl)",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard shortcut",
	100,
	300,
	true,
	"perl",
	NULL,
	NULL,
	CGICoreModule__PERL__Process
};

CGICoreModule CGICoreModule__MAIL = {
	"MTA",
	"FIFO to UNIX MTA. (Ability to sending e-mail(s)",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard shortcut (UNIX Version only)",
	100,
	300,
	true,
	"mail",
	NULL,
	NULL,
	CGICoreModule__MAIL__Process
};

void CGICoreModule__PIPE__Process(Properties *p, const char *data){
#ifdef _NOPOPEN
   printflog(LOG_ERROR, "%s\n", PIPENOTIMPLEMENTED);
#else

#ifndef __DOS__
   signal(SIGPIPE, SIG_IGN);
#endif

   const char *file = (p == NULL) ? BIN_SHELL : p->get("FILE", BIN_SHELL);

   fflush(stdout); //prevent output bug
   FILE *f = popen(file, PIPEWRITEMODE);

   if (f == NULL){
      printflog(LOG_WARNING, "Module PIPE: Broken pipe (%s)...\n", file);
      return;
   }

   char *sdata = getDataOut(data);

   fprintf(f, "%s", sdata);
   fflush(f);

   free(sdata);

   pclose(f);

   fflush(stdout); //prevantive

#endif
};

void CGICoreModule__PERL__Process(Properties *p, const char *data){
   if (p){
      const char *perl = (cfg == NULL) ? BIN_PERL : p->get("PERL", BIN_PERL);
      p->put("FILE", perl);
   }

   CGICoreModule__PIPE__Process(p, data);
};

void CGICoreModule__MAIL__Process(Properties *p, const char *data){
   if (p){
      const char *sendmail = (cfg == NULL) ? BIN_SENDMAIL : p->get("SENDMAIL", BIN_SENDMAIL);
      p->put("FILE", sendmail);
   }

   CGICoreModule__PIPE__Process(p, data);
};
