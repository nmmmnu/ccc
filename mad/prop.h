#ifndef _MAD_PROP_H
#define _MAD_PROP_H

#include "mad.h"

#include <stdlib.h>

class PropertiesNode{
public:
   PropertiesNode *next;

   char *key;
   char *value;

   PropertiesNode(const char *key, const char *value);
   virtual ~PropertiesNode();
   boolean itIs(const char *key);
};


class PropertiesIterator{
public:
   virtual ~PropertiesIterator(){};
   
   virtual boolean pass(const char *key, const char *value) = 0;
   virtual char *getKey(){
      return NULL;
   };
   virtual char *getValue(){
      return NULL;
   };
};

class Properties{
protected:
   PropertiesNode *root;

   Properties *superProperties;
public:
   Properties(Properties *superProperties = NULL):
                                   root(NULL),
                                   superProperties(superProperties){};
   virtual ~Properties(){
       clear();
   };

   void setSuperProperties(Properties *superProperties = NULL){
      this->superProperties = superProperties;
   };

   void clear();

   //added later for intuitive work:)
   inline void add(const char *key, const char *value){put(key, value);
};
   void put(const char *key, const char *value);
   void putAsInt(const char *key, int value);
   void putAsLong(const char *key, long value);
   void putParsedStr(const char *s, char delim);
   void del(const char *key);
   const char *get(const char *key);
   const char *get(const char *key, char *value);
   int  getAsInt(const char *key, int value = 0);
   long getAsLong(const char *key, long value = 0);
   boolean exist(const char *key);

   void iterate(PropertiesIterator *pi, boolean destruct = false);

   void dump();
   void save(const char *filename, char propdelim = '\t', char remdelim = '#');
   void load(const char *filename, char propdelim = '\t', char remdelim = '#');
   void asSign(Properties *p);
};

#endif
//_MAD_PROP_H
