#include <string.h>

#include "../mad/mad.h"

#define CESTAR  '*'
#define CESLASH '/'
#define CEPLUS  '+'
#define CEMINUS '-'
#define CEGREAT '>'
#define CELESS  '<'
#define CEEQUAL '='

const char EDELIMITERS[] = { CESTAR, CESLASH, 
                             CEPLUS, CEMINUS, 
                             CEGREAT, CELESS, CEEQUAL,
                             '\0'};

class MathTok{
public:
   long int value;
   MathTok(const char *value){
      this->value=atoi(value);
   }
   virtual ~MathTok(){};
   virtual long int calc(long int)=0;
   virtual unsigned short int getPriority()=0;
};
class MathTok_PLUS:public MathTok{
public:
   MathTok_PLUS(const char *value):MathTok(value){};
   long int calc(long int x){ return x + value; };
   unsigned short int getPriority(){ return 10; };
};
class MathTok_MINUS:public MathTok{
public:
   MathTok_MINUS(const char *value):MathTok(value){};
   long int calc(long int x){ return x - value; };
   unsigned short int getPriority(){ return 10; };
};
class MathTok_STAR:public MathTok{
public:
   MathTok_STAR(const char *value):MathTok(value){};
   long int calc(long int x){ return x * value; };
   unsigned short int getPriority(){ return 20; };
};
class MathTok_SLASH:public MathTok{
public:
   MathTok_SLASH(const char *value):MathTok(value){};
   long int calc(long int x){ return x / value; };
   unsigned short int getPriority(){ return 19; };
};
class MathTok_GREAT:public MathTok{
public:
   MathTok_GREAT(const char *value):MathTok(value){};
   long int calc(long int x){ return x > value; };
   unsigned short int getPriority(){ return 5;  };
};
class MathTok_LESS:public MathTok{
public:
   MathTok_LESS(const char *value):MathTok(value){};
   long int calc(long int x){ return x < value; };
   unsigned short int getPriority(){ return 5;  };
};
class MathTok_EQUAL:public MathTok{
public:
   MathTok_EQUAL(const char *value):MathTok(value){};
   long int calc(long int x){ return x == value; };
   unsigned short int getPriority(){ return 5;  };
};

//#define MLEN 100;

long int calc(const char *expr){
   StringTokenizer *st = new StringTokenizer(expr, EDELIMITERS);
   
   MathTok *m[100 + 1];
   unsigned int co = 0;
   char znak = CEPLUS;

   const char *x = st->nextToken();
   while (x){
      switch  (znak){
         case CEPLUS  : m[co] = new MathTok_PLUS (x); break;
         case CEMINUS : m[co] = new MathTok_MINUS(x); break;
         case CESTAR  : m[co] = new MathTok_STAR (x); break;
         case CESLASH : m[co] = new MathTok_SLASH(x); break;
         case CEGREAT : m[co] = new MathTok_GREAT(x); break;
         case CELESS  : m[co] = new MathTok_LESS (x); break;
         case CEEQUAL : m[co] = new MathTok_EQUAL(x); break;

         default      : m[co] = new MathTok_PLUS (x); break;
      }
      co++;
      znak = st->lastdelimiter;
      x = st->nextToken();
   }

   delete(st);

   while(co > 1){
      unsigned int i;

      unsigned short int pr = m[0 + 1]->getPriority();
      for (i = 0 + 1; i < co; i++)
         if (m[i]->getPriority() > pr)
            pr = m[i]->getPriority();

      for (i = 0 + 1; i < co; i++)
         if (m[i]->getPriority() == pr){
            m[i - 1]->value = m[i]->calc( m[i - 1]->value );

            for (unsigned int j = i; j < co - 1; j++)
               m[j]=m[j + 1];

            co--;

            break;
         }
   }

   return m[0]->value;
}

int main(int argc, char *argv[]){
   printf(">>>%ld\n", calc(argv[1]) );

   return 0;
}
