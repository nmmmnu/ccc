#ifndef _MAD_MACRO_H
#define _MAD_MACRO_H

#include "mad.h"
#include "prop.h"

/*
     Macro Replacer - replace 'macroses' into strings
   by Nikolay Mijaylov  (c) 1996(?), 1998, 1999, Sofia

   first realisation - Turbo Pascal, non objects

   object tree:
   
   AbstractMacroReplacer(8)=+-----MacroReplacer
*/

#define DEFAULTMACROCONTROLDELIMITER "$[]"

class AbstractMacroReplacer{
protected:
   StringBuffer *sb;
   
   char          hdelim;
   char          bdelim;
   char          edelim;
   
   short int     logic_status;
   
   void initLogic();
   char logic(char x);
   
public:
   AbstractMacroReplacer(const char *controldelim = DEFAULTMACROCONTROLDELIMITER):
                                sb(new StringBuffer()),
                                hdelim(controldelim[0]),
                                bdelim(controldelim[1]),
                                edelim(controldelim[2]){};
   virtual ~AbstractMacroReplacer(){
      delete(sb);
   };

   const char *replace(const char *source);

   virtual const char *replaceField(const char *) = 0;
};

class MacroReplacer:public AbstractMacroReplacer{
protected:
   Properties *pr;
public:
   MacroReplacer(Properties *pr, const char *controldelim = DEFAULTMACROCONTROLDELIMITER):
                                AbstractMacroReplacer(controldelim),
                                pr(pr){
   };
   inline const char *replaceField(const char *field){
      return pr ? pr->get(field) : NULLs; };
};

#endif
//_MAD_MACRO_H