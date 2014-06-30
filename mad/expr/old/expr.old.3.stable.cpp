#include <string.h>
#include <ctype.h>

#include "expr.h"

const unsigned short int len = 100;

boolean EModuleErr;

struct EModule{
   char sign;
   unsigned short int priority;
   expr_type (*process)(expr_type, expr_type);
};

#define _expr_fn_param expr_type a, expr_type b

expr_type proc_star  (_expr_fn_param){ return a * b; }
expr_type proc_slash (_expr_fn_param){ 
   if (b != 0)
      return a / b; 

   EModuleErr = true;
   return 0;
}
expr_type proc_proc  (_expr_fn_param){ 
   if (b != 0)
      return (expr_type) ((long int) a % (long int) b);

   EModuleErr = true;
   return 0;
}
expr_type proc_plus  (_expr_fn_param){ return a + b; }
expr_type proc_minus (_expr_fn_param){ return a - b; }
expr_type proc_and   (_expr_fn_param){
   return (expr_type) ((int) a && (int) b); }
expr_type proc_or    (_expr_fn_param){
   return (expr_type) ((int) a || (int) b); }
expr_type proc_great (_expr_fn_param){ return a > b; }
expr_type proc_less  (_expr_fn_param){ return a < b; }
expr_type proc_greate(_expr_fn_param){ return a >= b; }
expr_type proc_lesse (_expr_fn_param){ return a <= b; }
expr_type proc_equal (_expr_fn_param){ return a == b; }
expr_type proc_nequal(_expr_fn_param){ return a != b; }

#define _chk_sign(a, b, c)\
	if (sign == a && st->nextToEnd()) if (st->nextToEnd()[0] == b){ sign = c; st->index++; }

const char *EDELIMITERS = "*/%+-&|><}{=#"	"!";

//This is table of double delimiters such as '>='
//Every of tthem must be substituted to single delimiter such '}'
#define  chk_sign()\
	_chk_sign('>', '=', '}')	\
	_chk_sign('<', '=', '{')	\
	_chk_sign('<', '>', '#')	\
	_chk_sign('!', '=', '#')	\
	_chk_sign('=', '=', '=')	\
	_chk_sign('&', '&', '&')	\
	_chk_sign('|', '|', '|')

unsigned int operatorcounter = 0;

EModule mod__star   = { EDELIMITERS[operatorcounter++], 20, proc_star  };
EModule mod__slash  = { EDELIMITERS[operatorcounter++], 19, proc_slash };
EModule mod__proc   = { EDELIMITERS[operatorcounter++], 19, proc_proc  };

const int EPLUS = operatorcounter;
EModule mod__plus   = { EDELIMITERS[operatorcounter++], 10, proc_plus  };
EModule mod__minus  = { EDELIMITERS[operatorcounter++], 10, proc_minus };

EModule mod__and    = { EDELIMITERS[operatorcounter++],  5, proc_and   };
EModule mod__or     = { EDELIMITERS[operatorcounter++],  4, proc_or    };

EModule mod__great  = { EDELIMITERS[operatorcounter++],  7, proc_great };
EModule mod__less   = { EDELIMITERS[operatorcounter++],  7, proc_less  };
EModule mod__greate = { EDELIMITERS[operatorcounter++],  7, proc_greate};
EModule mod__lesse  = { EDELIMITERS[operatorcounter++],  7, proc_lesse };
EModule mod__equal  = { EDELIMITERS[operatorcounter++],  7, proc_equal };
EModule mod__nequal = { EDELIMITERS[operatorcounter++],  7, proc_nequal};

/*
#include <math.h>
expr_type proc_power (_expr_fn_param){ return pow(a, b); }
EModule mod__power = { EDELIMITERS[operatorcounter++],  5, proc_power};
*/

EModule *e_matrix[] = {
   &mod__star,
   &mod__slash,
   &mod__proc,
   &mod__plus,
   &mod__minus,
   &mod__and,
   &mod__or,
   &mod__great,
   &mod__less,
   &mod__greate,
   &mod__lesse,
   &mod__equal,
   &mod__nequal,

   NULL
};

struct MathTok{
   expr_type          value;
   unsigned short int proc;
};

expr_type Calculator::calc1(const char *expr){
   StringTokenizer *st = new StringTokenizer(expr, EDELIMITERS);

   MathTok m[len + 1];

   size_t co = 0;

   char sign = '\0';

   const char *x = st->nextToken();

   while (x && co < len){
      m[co].value = atoexpr(x);
      m[co].proc  = EPLUS;

      for(size_t i=0; e_matrix[i]; i++)
         if (e_matrix[i]->sign == sign){
            m[co].proc = i;
            break;
         }

      //printf("%-2c %5.2f\n", e_matrix[ m[co].proc ]->sign, m[co].value);

      co++;
      sign = st->lastdelimiter;
      chk_sign();
 
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
            EModuleErr=false;
            if (e_matrix[ m[i].proc ]->process == NULL)
               err = 1;
            else
               m[i - 1].value = e_matrix[ m[i].proc ]->process(m[i - 1].value, m[i].value);
            err=err || EModuleErr;

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
