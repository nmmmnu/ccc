#include "modules.h"

//Print some modules statistix

void modulessum(){
   printf("Modules set: %s\n", cgi_core_modules_set);
   printf("Modules set description:\n%s\n", cgi_core_modules_set_description);
   printf("\n");
}

void moduleslist(){
   modulessum();

   const char *frm1 = "%2s %-15s %s\n";
   const char *frm  = "%2d %-15s %s\n";

   printf(frm1, "No", "Name", "Description");
   printf(frm1, "==", "====", "===========");

   for (unsigned int i = 0; module_matrix[i]!=NULL; i++)
      printf(frm, 
              i + 1,
              module_matrix[i]->name, 
              module_matrix[i]->description);
};

void modulesshow(unsigned int i){
   const char *frm = "%-15s : %s\n";
   const char *fr0 = "%-15s : <%s>...</%s>\n";
   const char *fr1 = "%-15s : <%s>\n";
   const char *fr2 = "%-15s : %d\n";

   printf(frm, "Name"          , module_matrix[i]->name);
   printf(frm, "Description"   , module_matrix[i]->description);

   if (module_matrix[i]->doubleTagProcess)
      printf(fr0, "HTML tag"  , module_matrix[i]->tagProcess, 
                                module_matrix[i]->tagProcess);
   else
      printf(fr1, "HTML tag"  , module_matrix[i]->tagProcess);

   printf(frm, "Copyright"     , module_matrix[i]->copyright);
   printf(frm, "Distribution"  , module_matrix[i]->distribution);
   printf(fr2, "Module Ver"    , module_matrix[i]->version);
   printf(fr2, "Core (Dep) Ver", module_matrix[i]->needVersion);

   printf("\n");
};

void modulesshow(){
  modulessum();

  for (unsigned int i = 0; module_matrix[i]!=NULL; i++)
     modulesshow(i);
};
