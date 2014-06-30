/************************************************\
*                                                *
*  REGULAR EXPRESSION PARSER EVALUATOR LANGUAGE  *
*                                                *
\************************************************/

expr_type proc_star  (_expr_fn_param){ return a * b; }
expr_type proc_slash (_expr_fn_param_err){
   if (b != 0)
      return a / b; 

   if (EModuleErr)
      *EModuleErr = true;
   return 0;
}
expr_type proc_proc  (_expr_fn_param_err){ 
   if (b != 0)
      return (expr_type) ((long int) a % (long int) b);

   if (EModuleErr)
      *EModuleErr = true;
   return 0;
}
expr_type proc_plus  (_expr_fn_param){ return a + b; }
expr_type proc_minus (_expr_fn_param){ return a - b; }
expr_type proc_and   (_expr_fn_param){ return (expr_type) ((int) a && (int) b); }
expr_type proc_or    (_expr_fn_param){ return (expr_type) ((int) a || (int) b); }
expr_type proc_great (_expr_fn_param){ return a > b; }
expr_type proc_less  (_expr_fn_param){ return a < b; }
expr_type proc_greate(_expr_fn_param){ return a >= b; }
expr_type proc_lesse (_expr_fn_param){ return a <= b; }
expr_type proc_equal (_expr_fn_param){ return a == b; }
expr_type proc_nequal(_expr_fn_param){ return a != b; }

#define E_STAR		0
#define E_SLASH		1
#define E_PROC		2
#define E_PLUS		3
#define E_MINUS		4
#define E_AND		5
#define E_OR		6
#define E_GREAT		7
#define E_LESS		8
#define E_GREATE	9
#define E_LESSE		10
#define E_EQUAL		11
#define E_NEQUAL	12

EModule mod__star   = { E_STAR  , 20, proc_star  };
EModule mod__slash  = { E_SLASH , 19, proc_slash };
EModule mod__proc   = { E_PROC  , 19, proc_proc  };
EModule mod__plus   = { E_PLUS  , 10, proc_plus  };
EModule mod__minus  = { E_MINUS , 10, proc_minus };
EModule mod__and    = { E_AND   ,  5, proc_and   };
EModule mod__or     = { E_OR    ,  4, proc_or    };
EModule mod__great  = { E_GREAT ,  7, proc_great };
EModule mod__less   = { E_LESS  ,  7, proc_less  };
EModule mod__greate = { E_GREATE,  7, proc_greate};
EModule mod__lesse  = { E_LESSE ,  7, proc_lesse };
EModule mod__equal  = { E_EQUAL ,  7, proc_equal };
EModule mod__nequal = { E_NEQUAL,  7, proc_nequal};

#define _chk_sign1(a, c)\
	if (sign == a){ return c; }

#define _chk_sign2(a, b, c)\
	if (sign == a && st->nextToEnd()){\
	if (st->nextToEnd()[0] == b){ st->index++; return c; } }

const char *EDELIMITERS	= "*/%+-&|><="	"}{#"	"!";

sign_type chk_sign(char sign, StringTokenizer *st, boolean *EModuleErr){
   //Double character operators
   _chk_sign2('>', '=', E_GREATE)
   _chk_sign2('<', '=', E_LESSE )
   _chk_sign2('<', '>', E_NEQUAL)
   _chk_sign2('!', '=', E_NEQUAL)
   _chk_sign2('=', '=', E_EQUAL )
   _chk_sign2('&', '&', E_AND   )
   _chk_sign2('|', '|', E_OR    )

   //Single character operators
   _chk_sign1('*', E_STAR  )
   _chk_sign1('/', E_SLASH )
   _chk_sign1('%', E_PROC  )
   _chk_sign1('+', E_PLUS  )
   _chk_sign1('-', E_MINUS )
   _chk_sign1('&', E_AND   )
   _chk_sign1('|', E_OR    )
   _chk_sign1('>', E_GREAT )
   _chk_sign1('<', E_LESS  )
   _chk_sign1('=', E_EQUAL )

   //Single character operators for compatibility
   _chk_sign1('}', E_GREATE)
   _chk_sign1('{', E_LESSE )
   _chk_sign1('#', E_NEQUAL)

   //Error and default operator
   if (EModuleErr && st->nextToEnd())
      *EModuleErr = true;
   return E_PLUS;
}

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

/*
#include <math.h>
expr_type proc_power (_expr_fn_param){ return pow(a, b); }
EModule mod__power = { E_POWER,  5, proc_power};
*/
