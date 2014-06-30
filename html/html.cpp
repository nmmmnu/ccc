#include <mad/mad.h>
#include <mad/xml_parser.h>

//Permited tags.
const char *XML_MATRIX[] = {
	"P",
	"H1", "H2", "H3", "H4", "H5", "H6",
	"B", "I", "U", "STRONG",
	"TABLE", "TR", "TH", "TD",
	"UL", "OL", "LI",
	"DIV", "BLOCKQUOTE", "CENTER",
	"HR",

	NULL
};

//Double tags that aren't permited
const char *dXML_MATRIX[] = {
	"HEAD",	"TITLE",
	"SCRIPT",
	"STYLE",

	NULL
};

void showhelp(){
   printf("\n");
   printf("=================================\n");
   printf("MAD Soft HTML convertor/preparser\n");
   printf("=================================\n");
   printf("Copyleft    : (c) 01.2000, Nikolay Mijaylov\n");
   printf("Distribition: GPL\n\n");
   printf("Usage\n");
   printf("\thtmlconvert [file]\n\n");
};

XMLParser *xml = new XMLParser();
Properties *p  = new Properties();

boolean permit(const char *s){
   if (!s) return false;

   p->clear();
   xml->parseTag(s, p);
   const char *tag = p->get(XMLTAGPARSEKEY);

   unsigned int i;

   /* if (!stricmpn(&tag[ tag[0] == '/' ? 1 : 0 ], XML_MATRIX[i])) */
   for (i = 0; XML_MATRIX[i]; i++)
      if (!stricmpn(&tag[ tag[0] == '/' ], XML_MATRIX[i]))
         return true;

   for (i = 0; dXML_MATRIX[i]; i++)
      if (!stricmpn(tag, dXML_MATRIX[i])){
         xml->extractDoubleTag(tag);
         return false;
      }

   return false;
}

int main(int, char *argv[]){
   if (argv[1])
      xml->loadFromFile(argv[1]);
   else{
      showhelp();
      return 1;
   }

   const char *s = "";
   while (s){
      s = xml->seekNextTag();

      if (s) printf("%s", s);

      s = xml->getNextTag();

      if (permit(s)) printf("<%s>", s);
   }

   delete(p);
   delete(xml);

   return 0;
}
