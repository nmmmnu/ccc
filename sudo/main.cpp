//gcc main.cpp -I ~/ccc/ -L ~/ccc/lib/ -lstdc++ -lmad

#include <unistd.h>
#include <stdio.h>
#include <mad/prop.h>

#define MASTER_CFG_PROP "/etc/nmmm.sudo.cfg"

int main(const int argc, const char *argv[]){
   Properties *cfg = new Properties();
   cfg->load( MASTER_CFG_PROP );

   if ( argc < 1 + 1 ){
      printf("Give at least one index, exiting...\n");
      return 1;
   }

   if ( setuid(0) != 0 ){
      printf("Can't setuid(0), exiting...\n");
      return 2;
   }

   if ( ! cfg->exist( argv[1] )){
      printf("Index not exists, exiting...\n");
      return 3;
   }

   const char *cmd = cfg->get( argv[1] );

   if ( ! cmd ){
      printf("Empty index, exiting...\n");
      return 3;
   }

   printf("Executing %s\n", cmd);

   system(cmd);

   printf("Done\n");

   return 0;
}
