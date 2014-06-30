#include <string.h>

#include "../mad/mad.h"

#define EPLUS  "+"
#define EMINUS "-"
#define ESTAR  "*"
#define ESLASH "/"

#define EDELIMITERS  EPLUS""EMINUS""ESTAR""ESLASH

class MathToken{
private:
   float      value;
   MathToken *next;
public:
   MathToken(long float value):
                         value(value), 
                         next(NULL){};
   virtual ~MathToken(){}

   inline void setNext(MathToken *mt){
      if (next)
         delete(next);
      next=mt;
   }

   virtual float calculate(float a, float b)=0;
   virtual unsigned short int getClass()=0;
   
   void process(){
      MathToken mt = next;
      value = calculate(value, mt->value);
      next  = mt->next;
      delete(mt);
   }
}

long int calc(const char *expr){
   StringTokenizer *st = new StringTokenizer(expr, EDELIMITERS);

   MathToken *root;

   MathToken *mt1 = NULL;
   const char *x = st->nextToken();
   while (x){
      MathToken *mt;
      case (st->lastdelimiter){
         EPLUS : mt = new MathToken(x);
         EMINUS: mt = new MathToken(x);
         ESTAR : mt = new MathToken(x);
         ESLASH: mt = new MathToken(x);
      }
      x = st->nextToken();

      if (mt1)
         mt1->next=mt;
      else
         root=mt
      
      mt1=mt;
   }

   delete(st);
   
   for (unsigned short int i = 255; i >= 0; i--)
      for (MathToken *mt=root; mt; mt=mt->next)
         if (mt->getClass() >= i)
            mt->process();

   return 0;
}

int main(int argc, char *argv[]){
   printf(">>>%ld\n", calc("2+2*56+7/2"));

   return 0;
}
