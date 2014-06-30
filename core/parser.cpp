//Provide HTML parsing
#include "parser.h"

#include "main.h"

#include "macrocodereplacer.h"
#include "modules.h"
#include "VERSION.h"

//Rakursive counter, 10 is enought
#define RAKURSE_DEEP 10
static int parserak = 0;

#define FILEMAXLEN   64 * 1024

char *getDataOut(const char *s){
   MacroCodeReplacer *mr = new MacroCodeReplacer(macro);

   char *maktmp = (isempty(s)) ? NULL : strdupn(mr->replace(s));

   delete(mr);

   return maktmp;
}

boolean echo_control = true;

void printout(const char *s){
   if (isempty(s))
      return;

   if (!echo_control)
      return;

   //the funnction take care of all...
   placeHTTPHeader();

   char *tmp = getDataOut(s);
   printf("%s", tmp);
   free(tmp);
}

class PropMacroIterator: public PropertiesIterator{
protected:
   char *x;
public:
   boolean pass(const char *, const char *value){
      MacroCodeReplacer *mr = new MacroCodeReplacer(macro);
   
      const char *tmp = mr->replace(value);

      x = strdup( (tmp) ? tmp : "" );

      //Iterator must return not NULL value

      delete(mr);
      return true;
   }
   char *getValue(){
      return x;
   }
};

void parseFile(const char *file){
   if (parserak > RAKURSE_DEEP){
      printflog(LOG_ERROR, "Parser: Racurse problem\n");
      return; //Racurse problem
   }

   parserak++;

   XMLParser  *xml  = new XMLParser();
   Properties *tagp = new Properties();

   xml->loadFromString(file);

   while(!xml->eof()){
      printout(xml->seekNextTag(html_matrix));

      if (xml->eof()) break; //prevantive

      tagp->clear();
      xml->parseTag(xml->getNextTag(), tagp);
      tagp->iterate(new PropMacroIterator(), true);
      const char *tag = tagp->get(XMLTAGPARSEKEY);

      for (unsigned int i = 0; module_matrix[i]!=NULL; i++)
         if (!stricmpn(module_matrix[i]->tagProcess, tag)){
            //executing module!
            //=================

            //extracting sub text (if double tag <p>Hello</p>)
            const char *subfile = (module_matrix[i]->doubleTagProcess) ?
                                   xml->extractDoubleTag(tag, true) : NULLs;

            //Look after version
            if (module_matrix[i]->needVersion > CGI_CORE_VERSION_NUM){
               printflog(LOG_FATAL_ERROR, "Parser: Module %d require version %d\n", i, module_matrix[i]->needVersion);
               break;
            }
				       
            //executing module (and pray that module worx:)
            if (module_matrix[i]->process)
               module_matrix[i]->process(tagp, subfile);

            //we do not need this cicle anymore
            break;
         }
   }

   delete(tagp);
   delete(xml);

   parserak--;
};

void parseFromFile(const char *filename){
   char data[FILEMAXLEN + 1];
   size_t len = file2char(filename, data, FILEMAXLEN);

   if (len <= 0)
      return;

   size_t i = 0;
   //Fixing #!cmd

   if (len > 2)
      if (data[0] == '#' && data[1] == '!')
         for (i = 0; data[i] != '\0' && data[i - 1] != '\n'; i++);

   parseFile(&data[i]);
};
