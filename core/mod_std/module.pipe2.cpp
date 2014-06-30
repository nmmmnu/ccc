#include <mad/sys/dpipe.h>

#include "module.pipe2.h"

#define BIN_PERL              "perl"
#define BIN_SENDMAIL          "sendmail -t -i"

void CGICoreModule__PIPE2__Process(Properties *, const char *);
void CGICoreModule__PERL2__Process(Properties *, const char *);
void CGICoreModule__MAIL2__Process(Properties *, const char *);

CGICoreModule CGICoreModule__PIPE2 = {
	"double FIFO",
	"bi-directional POSIX FIFO (pipe) module",
	"(c) 1.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard (UNIX Version only)",
	100,
	300,
	true,
	"dpipe",
	NULL,
	NULL,
	CGICoreModule__PIPE2__Process
};

CGICoreModule CGICoreModule__PERL2 = {
	"double Perl",
	"bi-dir. FIFO to Perl interpretator. (Ability to run Perl)",
	"(c) 1.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard shortcut",
	100,
	300,
	true,
	"dperl",
	NULL,
	NULL,
	CGICoreModule__PERL2__Process
};

CGICoreModule CGICoreModule__MAIL2 = {
	"double MTA",
	"bi-dir. FIFO to UNIX MTA. (Ability to sending e-mail(s)",
	"(c) 1.2000, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard shortcut (UNIX Version only)",
	100,
	300,
	true,
	"dmail",
	NULL,
	NULL,
	CGICoreModule__MAIL2__Process
};

void CGICoreModule__PIPE2__Process(Properties *p, const char *data){
   const char *file = (p == NULL) ? BIN_SHELL : p->get("FILE", BIN_SHELL);

   if (dpopen(file) < 0){
      printflog(LOG_ERROR, "Module dFIFO: Unable dpopen()...\n");
      return;
   }

   fflush(stdout); //prevent output bug

   FILE *z = fdopen(dpopen_in , "w");
   fprintf(z, data);
   fflush(z);
   fclose(z);

   StringBuffer *sb=new StringBuffer();

   FILE *x = fdopen(dpopen_out, "r");

   while (!feof(x)){
      char c = fgetc(x);
      if (!feof(x))
         sb->add(c);
   }

   fclose(x);

   parseFile(sb->get());

   delete(sb);

   fflush(stdout); //prevent output bug
};

void CGICoreModule__PERL2__Process(Properties *p, const char *data){
   if (p){
      const char *perl = (cfg == NULL) ? BIN_PERL : p->get("PERL", BIN_PERL);
      p->put("FILE", perl);
   }

   CGICoreModule__PIPE2__Process(p, data);
};

void CGICoreModule__MAIL2__Process(Properties *p, const char *data){
   if (p){
      const char *sendmail = (cfg == NULL) ? BIN_SENDMAIL : p->get("SENDMAIL", BIN_SENDMAIL);
      p->put("FILE", sendmail);
   }

   CGICoreModule__PIPE2__Process(p, data);
};
