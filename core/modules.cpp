//Register and Install all modules

#include "modules.h"

//Do u need more?
#define MAX_MODULES 99

const char **initmodules(){
   static const char *html_matrix[MAX_MODULES];
   
   for (unsigned int i = 0; module_matrix[i]!=NULL && i <= MAX_MODULES; i++){
      html_matrix[i] = module_matrix[i]->tagProcess;

      //executing module init (and pray that module worx:)
      if (module_matrix[i]->s_process)
         module_matrix[i]->s_process();
   }

   return html_matrix;
};

const char **html_matrix = initmodules();

void donemodules(){
   //executing module done (and pray that module worx:)
   for (unsigned int i = 0; module_matrix[i]!=NULL && i <= MAX_MODULES; i++)
      if (module_matrix[i]->e_process)
         module_matrix[i]->e_process();
};
