//Shared code for separators:
//<if>...<else>...</if>
//<db_sel>...<db_error>...</db_sel>

//I'm lazy to link it...
inline void do_separator(boolean result, const char *x,
                  const char *sep[],
                  const char *brk[]){
   //printf("\n%s\n%s\n%s\n", sep[0], brk[0], x);

   XMLParser  *xml  = new XMLParser();
   xml->loadFromString(x);

   const char *xx=xml->seekNextTag(sep, brk);

   if (result){
      parseFile(xx);
      return;
   }

   if (xml->eof()) return;
   xml->getNextTag(); //else tag him self
   if (xml->eof()) return;

   parseFile(xml->seekNextTag(NULL_XML_MATRIX));

   delete(xml);
}
