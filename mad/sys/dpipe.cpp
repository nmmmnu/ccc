#include "dpipe.h"
#include "../mad.h"

int dpopen_in;
int dpopen_out;

#ifdef _NOPIPE

int dpopen(const char *){
   return -5;
};

#else

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define  STDIN_NO  fileno(stdin)
#define  STDOUT_NO fileno(stdout)

int dpopen(const char *command){
   //Create pipes
   int  in[2];
   if (pipe( in) < 0)
      return -1;

   int out[2];
   if (pipe(out) < 0){ 
      //close open descriptors
      close( in[0]); close( in[1]);
      return -2;
   }

   //Flush files
   //fflush(stdin);
   fflush(stdout); 
   fflush(stderr);

   //Go fork
   int fres = fork();

   if ( fres < 0  ){
      //Something BAD happen

      //close open descriptors
      close( in[0]); close( in[1]);
      close(out[0]); close(out[1]);
      return -3;

   }elif ( fres == 0 ){
      //Child process

      //We will not check for errors,
      //because this is different process

      //Close stdfiles and redirect them
      close( STDIN_NO); close( in[1]); dup2( in[0],  STDIN_NO);
      close(STDOUT_NO); close(out[0]); dup2(out[1], STDOUT_NO);

      //Replace forked process
      if ( system(command) ){
      }

      //printf("XXXXX");

      exit(0);
   }

   //parent process

   close( in[0]); dpopen_in  =  in[1];
   close(out[1]); dpopen_out = out[0];

   return 0;
}

#endif
//_NO_PIPE
