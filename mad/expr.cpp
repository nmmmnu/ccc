#include <string.h>
#include <ctype.h>

#include "expr.h"

const unsigned short int len = 100;

#define sign_type short int

#define _expr_fn_param_err expr_type a, expr_type b, boolean *EModuleErr
#define _expr_fn_param     expr_type a, expr_type b, boolean *
// = NULL

struct EModule{
   sign_type sign;
   unsigned short int priority;
   expr_type (*process)(_expr_fn_param);
};

#include "expr.i.cpp"

struct MathTok{
   expr_type value;
   sign_type proc;
};

expr_type Calculator::calc1(const char *expr){
   StringTokenizer *st = new StringTokenizer(expr, EDELIMITERS);

   MathTok m[len + 1];

   size_t co = 0;

   sign_type sign = E_PLUS;

   const char *x = st->nextToken();

   while (x && co < len){
      m[co].value = atoexpr(x);
      m[co].proc  = sign;

      //printf("%-3d %5.2f %d %s\n", e_matrix[ m[co].proc ]->sign, m[co].value, err, st->nextToEnd());

      co++;
      sign = chk_sign(st->lastdelimiter, st, &err);

      x = st->nextToken();
   }

   delete(st);

   while(co > 1){
      size_t i;

      unsigned short int pr  = e_matrix[ m[0 + 1].proc ]->priority;

      for (i = 0 + 1; i < co; i++){
         unsigned short int pr1 = e_matrix[ m[i].proc ]->priority;
         if (pr1 > pr)
            pr = pr1;
      }

      for (i = 0 + 1; i < co; i++)
         if (e_matrix[ m[i].proc ]->priority == pr){
            if (e_matrix[ m[i].proc ]->process == NULL)
               err = true;
            else
               m[i - 1].value = e_matrix[ m[i].proc ]->process(m[i - 1].value, m[i].value, &err);

            for (size_t j = i; j < co - 1; j++)
               m[j]=m[j + 1];

            co--;

            break;
         }
   }

   return m[0].value;
}

expr_type Calculator::calc_br(){
   StringBuffer *sb = new StringBuffer();

   for (;pos < strlen(expr); pos++){
      //printf(">>>%c %5d\n", expr[pos], pos);

      if   (expr[pos] == '('){
         pos++;
         sb->add( exprtoa(calc_br()) );
      }elif (expr[pos] == ')')
         break;
      else
         if (!isspace(expr[pos]))
            sb->add(expr[pos]);
   }

   expr_type tmp = calc1( sb->get() );
   delete(sb);
   return tmp;
}
