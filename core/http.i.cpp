//HTTP stuff like QUERY_STRING handing and passing header

#include <mad/coder.h>

class URLDecoderIterator:public PropertiesIterator{
private:
   Coder *coder;
protected:
   char *key;
   char *value;
public: 
   URLDecoderIterator():coder(new Coder()){
   };
   ~URLDecoderIterator(){
      delete(coder);
   };
   boolean pass(const char *key, const char *value){
      this->key   = strdupn(coder->URLDecode(key));
      this->value = strdupn(coder->URLDecode(value));

      return true;
   };
   char *getKey(){
      return key; 
   };
   char *getValue(){
      return value;
   };
};

class QueryStringParser:public StringParser{
public:
   QueryStringParser(Properties *p, const char *data):StringParser(p, data, '&', '='){};
   void parse(){
      StringParser::parse();
      p->iterate(new URLDecoderIterator(), true);
   };
};

class CookieStringParser:public StringParser{
public:
   CookieStringParser(Properties *p, const char *data):StringParser(p, data, ';', '='){};
};

void lookCookies(){
   StringParser *cp = new CookieStringParser(query, getenv("HTTP_COOKIE"));
   cp->parse();
   delete(cp);
}

#define MAXQUERYSTRINGSIZE 1024*5

void lookQueryStr(){
   char tmp[MAXQUERYSTRINGSIZE + 1];
   tmp[0]  = '\0';

   QueryStringParser *qsp;

   if ( !strcmpn( getenv("REQUEST_METHOD"), "POST" ) ){
      unsigned int size = atoid(getenv("CONTENT_LENGTH"), 0);

      if (size > MAXQUERYSTRINGSIZE)
         size =  MAXQUERYSTRINGSIZE;

      if (size > 0){
         filegets(tmp, MAXQUERYSTRINGSIZE, stdin);
         tmp[MAXQUERYSTRINGSIZE] = '\0';
      }

      qsp = new QueryStringParser(query, tmp);
   }else
      qsp = new QueryStringParser(query, getenv("QUERY_STRING") );

   qsp->parse();
   delete(qsp);
};

boolean f_placeHTTPHeader = true;

void placeHTTPHeader(const char *contenttype){
   if (f_placeHTTPHeader)
      f_placeHTTPHeader = false;
   else
      return;

   if (!want_http_header)
      return;

   printf("content-type: %s\n\n", contenttype);
};
