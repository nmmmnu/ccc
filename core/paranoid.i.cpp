/*
 * PARANOID SECURITY OVERVIEW
 * --------------------------
 * Look ater security.
 *    Some ppl, including me thinking that it isn't
 * nesecery to pass all these tests, other think they 
 * need. Enable/Disable them with "config.h" file.
 */

#ifndef PARANOIDSECURITY
//fake function
//inline void lookParanoidSecure(){};
//Now a trick with define
#define lookParanoidSecure()
#else

//Hope any UNIX have it
#ifndef __DOS__

//1:30 min REAL livetime, then kill...
//Livetime cannot be configured to be less...
#define CGI_LIVETIME 90

//MAXPATHLEN
//about realpath(), man page "say" it's in <sys/param.h>,
//but I found it in <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#endif 
//__DOS__

#include CHDIR_LIB

void lookDir(){
   if (cfg->exist("dir"))
      chdir(cfg->get("dir"));
};

#if defined(__DOS__) || defined(__BEOS__)
#define setautokill()
#else

void signal_SIGALRM_catch(int){
   printf("Time limit expire, process halted\n");
   printflog(LOG_FATAL_ERROR,
                     "Paranoid: Time limit expire, file %s\n", the_file);

   raise(SIGTERM); //Terminate it!
}

void setautokill(){
   if (signal(SIGALRM, signal_SIGALRM_catch) == SIG_ERR){
      printflog(LOG_WARNING, "Paranoid: Can't signal()\n");
      return;
   }

   itimerval t;
   t.it_value.tv_sec  = MAX(cfg->getAsInt("livetime"), CGI_LIVETIME);
   t.it_value.tv_usec = t.it_interval.tv_sec = t.it_interval.tv_usec = 0;

   setitimer(ITIMER_REAL, &t, NULL);
}

#endif 

#if defined(__DOS__) || defined(__BEOS__)
#define lookPath() true
#else
boolean lookPath(){
   const char *pararoot=getenv("DOCUMENT_ROOT");
   if (!the_file || !pararoot)
      return true;

   char  rn1b[MAXPATHLEN];
   const char *rn1 = realpath(the_file, rn1b);

   char  rn2b[MAXPATHLEN];
   const char *rn2 = realpath(pararoot, rn2b);

   if (!rn1 || !rn2)
      return true;

   if (strncmp(rn1, rn2, strlen(rn2) )){
      printflog(LOG_FATAL_ERROR, "Paranoid: DOCUMENT_ROOT not match!\n");
      return false;
   }

   if (rn2[strlen(rn2) - 1] != PATH_DELIMITER){
      /*
        Now lets check old Octarion's bug with missing slash:
           1. /www            - document_root
           2. /www_anytxthere - "simillar" directory
        Request:
           core --file /www_any_text_here/file.chtm
       */
      if (rn1[strlen(rn2) - 1 + 1]  != PATH_DELIMITER){
         printflog(LOG_FATAL_ERROR, "Paranoid: DOCUMENT_ROOT not match, "\
                                    "Accessing \"simillar\" directory!\n");
         return false;
      }
   }
   return true;
}
#endif

//BeOS has only one user named 'Baron' with UID/GID 0...
#if defined(__DOS__) || defined(__BEOS__)
#define lookUID() true
#else
boolean lookUID(){
   if (cfg->exist("setgid"))
      if (setgid(cfg->getAsInt("setgid")) == -1)
         printflog(LOG_NON_FATAL_ERROR, "Paranoid: Unable setgid()\n");
      else
         printflog(LOG_NOTIFY, "Paranoid: setgid()\n");

   if (cfg->exist("setuid"))
      if (setuid(cfg->getAsInt("setuid")) == -1)
         printflog(LOG_NON_FATAL_ERROR, "Paranoid: Unable setuid()\n");
      else
         printflog(LOG_NOTIFY, "Paranoid: setuid()\n");

   uid_t u = getuid() && geteuid() && getgid() && getegid();
   if (!u)
      printflog(LOG_FATAL_ERROR, "Paranoid: Running as UID 0 (root)\n");

   return (boolean) u;
}
#endif

#ifdef __DOS__
#define lookStat() true
#else
boolean _lookStat(const char *filename){
   if (!filename)
      return true;

   struct stat st;
   if (stat(filename, &st) < 0){
      printflog(LOG_NON_FATAL_ERROR, 
                "Paranoid: Unable stat() on file \"%s\"\n", filename);
      return true;
   }
   if (st.st_mode & S_IWOTH){
      printflog(LOG_FATAL_ERROR,
                "Paranoid: Dangerous stat() on file \"%s\"\n", filename);
      return false;
   }
   return true;
}

inline boolean lookStat(){
   return _lookStat(the_file          ) &&
          _lookStat(MASTER_CFG_PROP   ) &&
          _lookStat(MASTER_MACRO_PROP );
}
#endif

inline void lookParanoidSecure(){
   lookDir();

   setautokill();

   if ((!lookPath()) ||
       (!lookUID() ) ||
       (!lookStat()) )
      the_file=NULL;
}

#endif
//PARANOIDSECURITY
