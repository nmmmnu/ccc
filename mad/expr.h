#ifndef MAD_E_H
#define MAD_E_H

#include "mad.h"

#define expr_type double

class Calculator{
public:
   boolean     err;
protected:
   size_t      pos;
   const char *expr;

   expr_type atoexpr(const char *x){
      char *code[] = {NULL, NULL};
      double res=strtod(x, (char **) code);
      err = err || !isempty(code[0]);
      return res;
   };
   inline char *exprtoa(expr_type x){
      return ftoa(x);
   }
   expr_type calc1(const char *expr);
   expr_type calc_br();
public:
   Calculator(const char *expr):expr(expr){};

   expr_type calc(){ 
      pos=0;
      err = false;
      return calc_br(); 
   };
};

#endif