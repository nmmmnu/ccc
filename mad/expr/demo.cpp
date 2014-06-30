#include <mad/expr.h>

int main(int, char *argv[]){
   Calculator *c = new Calculator(argv[1]);

   printf(">>>%f\n", c->calc() );
   printf(">>>%d\n", c->err );

   delete(c);

   return 0;
}
