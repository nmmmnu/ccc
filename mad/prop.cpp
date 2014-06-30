#include "prop.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "textfile.h"

PropertiesNode::PropertiesNode(const char *key, const char *value){
   this->key   = strdupn(key);
   this->value = strdupn(value);
};
PropertiesNode::~PropertiesNode(){
   free(key);
   free(value);
};
boolean PropertiesNode::itIs(const char *key){
   return (key ==NULL) ? false : !strcmp(key, this->key);
};

//==================================================

class PropListIterator: public PropertiesIterator{
protected:
   FILE *f;
   char delim;
public:
   PropListIterator(FILE *f, char delim):f(f),
                                         delim(delim){
   };

   boolean pass(const char *key, const char *value){
      fprintf(f, "%s%c%s\n", key, delim, value);
      return true;
   } 
};

//==================================================

void Properties::put(const char *key, const char *value){
   if (isempty(key))
      return;

   del(key);

   if (isempty(value))
      return;

   PropertiesNode *x = new PropertiesNode(key, value);

   x->next = root;
   root = x;
};
void Properties::putAsInt(const char *key, int value){
   char tmp[50];  //max 17 chars
   sprintf(tmp, "%d", value); //BUG?!?! \0
   //itoa(value, tmp, 10); //bc - ok; gcc - error ?!?!?
   put(key, tmp);
};

void Properties::putAsLong(const char *key, long value){
   char tmp[50];  //max 17 chars
   sprintf(tmp, "%ld", value); //BUG?!?! \0
   put(key, tmp);
};

void Properties::putParsedStr(const char *s, char delim){
   if (isempty(s))
      return;

   StringTokenizer *st = new StringTokenizer(s, delim);

   char *tmp1 = strdupn(st->nextToken());
   char *tmp2 = strdupn(st->nextToEnd());

   const char *name = strcut(tmp1);
   const char *value = strcut(tmp2);

   put(name, value); 

   free(tmp1);
   free(tmp2);
   delete(st);
};

void Properties::clear(){
   while (root != NULL){
      PropertiesNode *tmp = root;
      root = tmp->next;
      delete(tmp);
   }
};
void Properties::del(const char *key){
   if (root == NULL)
      return;

   PropertiesNode *tmp = root;

   if (root->itIs(key)){
      root = root->next;
      delete(tmp);
      return;
   }

   while (tmp->next != NULL){
      PropertiesNode *tmp1 = tmp->next;

      if (tmp1->itIs(key)){
         tmp->next = tmp1->next;
         delete(tmp1);
      }else
         tmp = tmp1;
   }
};
const char *Properties::get(const char *key){
   PropertiesNode *tmp = root;
   while (tmp != NULL){
      if (tmp->itIs(key)){
         return tmp->value;
      }else
         tmp = tmp->next;
   }

   if (superProperties == NULL)
      return NULL;
   else
      return superProperties->get(key);
};
const char *Properties::get(const char *key, char *value){
   return (get(key) == NULL) ? value : get(key);

};
int Properties::getAsInt(const char *key, int value){
   const char *tmp = get(key);
   if (tmp == NULL)
      return value;

   return atoi(tmp);
};
long Properties::getAsLong(const char *key, long value){
   const char *tmp = get(key);
   if (tmp == NULL)
      return value;

   return atol(tmp);
};
boolean  Properties::exist(const char *key){
   return ( get(key) != NULL );
};
void Properties::iterate(PropertiesIterator *pi , boolean destruct){
   PropertiesNode *tmp = root;
   while (tmp != NULL){
      if (!pi->pass(tmp->key, tmp->value))
         break;

      if (pi->getKey() != NULL){
         free(tmp->key);
         tmp->key = pi->getKey();
      }

      if (pi->getValue() != NULL){
         free(tmp->value);
         tmp->value = pi->getValue();
      }

      tmp = tmp->next;
   }
   if (destruct)
      delete(pi);
};
void Properties::dump(){
   printf("Dump Properties\n");
   printf("===============\n");

   iterate(new PropListIterator(stdout, '\t'), true);

   printf("===============\n");
};

void Properties::save(const char *filename, char propdelim, char remdelim){
   FILE *f;

   if ( (f = fopen(filename, "wt")) == NULL )
      return;

   fprintf(f, "%cThis is automatic generated file\n", remdelim);
   fprintf(f, "%c\n", remdelim);

   iterate(new PropListIterator(f, propdelim), true);

   fclose(f);
};

class PropTextFile:public TextFile{
protected:
   Properties *prop;
   char       propdelim;
   char       remdelim;
public:
   PropTextFile(Properties *prop, char propdelim, char remdelim):
                                               prop(prop),
                                               propdelim(propdelim),
                                               remdelim(remdelim){};
   boolean iterator(const char *b){
      if (b == NULL)
         return true;

      char *buffer = strdup(b);

      for (size_t i = 0; i < strlen(buffer); i++)
         if (buffer[i] == remdelim){
            buffer[i] = '\0';
            break;
         }

      prop->putParsedStr(buffer, propdelim);

      free(buffer);

      return true;
   };
};

void Properties::load(const char *filename, char propdelim, char remdelim){
   PropTextFile *ptf = new PropTextFile(this, propdelim, remdelim);
   ptf->loadFromFile(filename);
   ptf->iterate();
   delete(ptf);
};

class PropAsSignIterator: public PropertiesIterator{
protected:
   Properties *x;
public:
   PropAsSignIterator(Properties *x):x(x){}
   boolean pass(const char *key, const char *value){
      if (x != NULL)
         x->put(key, value);
      return (x != NULL);
   }
};

void Properties::asSign(Properties *p){
   p->iterate(new PropAsSignIterator(this), true);
};
