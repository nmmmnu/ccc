#ifndef macrocodereplacerH
#define macrocodereplacerH

#define MACROS_NO         '-'
#define MACROS_HTML       'H'
#define MACROS_SOFT_HTML  'h'
#define MACROS_PHTML      'P'
#define MACROS_SOFT_PHTML 'p'
#define MACROS_URL        'U'
#define MACROS_NUMBER     'N'
#define MACROS_FLOAT      'F'
#define MACROS_SQL        'S'
#define MACROS_SQLNULL    'Q'

//Need!!!
#define MACROS_NONE       ' '

#include <mad/macro.h>
#include <mad/coder.h>

class MacroCodeReplacer:public MacroReplacer{
protected:
   Coder *coder;
public:
   MacroCodeReplacer(Properties *pr, const char *controldelim = DEFAULTMACROCONTROLDELIMITER):
                                     MacroReplacer(pr, controldelim),
                                     coder(new Coder()){};
   ~MacroCodeReplacer(){
      delete(coder);
   };
   const char *replaceField(const char *field);
};

#endif
