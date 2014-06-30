#include "coder.h"
#include <stdlib.h>
#include <ctype.h>

// About 'URLDecode'
/*
 * This appears in Core Web Programming from
 * Prentice Hall Publishers, and may be freely used
 * or adapted. 1997 Marty Hall, hall@apl.jhu.edu.
 */
// Translated is C/C++ 1998, Nikolay Mijaylov

const char *Coder::URLDecode(const char *enc){
   sb->clear();

   if (isempty(enc))
      return NULL;

   size_t i = 0;
   while (i < strlen(enc)){
      if (enc[i] == '+'){
         sb->add(' ');
         i++;
      }else if (enc[i] == '%'){
         char tmp[] = {'\0', '\0', '\0'};
         strncpy(tmp, &enc[i + 1], 2);

         int tmpi;
         sscanf(tmp, "%x", &tmpi);

         sb->add( (char) tmpi );

         i = i + 1 + 2;
      }else{
         sb->add(enc[i]);
         i++;
      }
   }

   return sb->get();
};

const char *Coder::URLEncode(const char *dec){
   sb->clear();

   if (isempty(dec))
      return NULL;
   /*
   25.08.1999
   
   OK All we know that '+' is conformed to space, but when we
   use Microsoft Internet Explorer and we try to encode a HTML
   link '<A></A>', there are a problem with this. The Netscape
   Navigator has a same problem. So Encoding is changed to '%20'
   which mean space too.
   */
   for(size_t i = 0; i < strlen(dec); i++)
      if (dec[i] == ' ')
         //sb->add('+');
	 sb->add("%20");
      else if (((dec[i] >= '0') && (dec[i] <= '9')) ||
               ((dec[i] >= 'a') && (dec[i] <= 'z')) ||
	       ((dec[i] >= 'A') && (dec[i] <= 'Z')))
	 sb->add(dec[i]);
      else{
         sb->add('%');
	 
	 char tmp[15];
	 sprintf(tmp, "%x", dec[i]);
	 if (strlen(tmp) == 1)
	    sb->add('0');

	 sb->add(tmp);
      }

   return sb->get();
}

const char *Coder::HTMLEncode(const char *enc, boolean phpn){
   sb->clear();

   if (isempty(enc))
      return NULL;
      
   for (size_t i = 0; i < strlen(enc); i++)
      switch (enc[i]){
         case '&' : sb->add("&amp;");   break;
         case '<' : sb->add("&lt;");    break;
         case '>' : sb->add("&gt;");    break;
         case '"' : sb->add("&quot;");  break;
         case '\n': sb->add( phpn ? "<Br>" : "\n" ); break;
         case ' ' : sb->add("&nbsp;");  break;
	 default  : sb->add( enc[i] );
      }

  return sb->get();
};

const char *Coder::SoftHTMLEncode(const char *enc, boolean phpn){
   sb->clear();

   if (isempty(enc))
      return NULL;

   boolean f = false;
   for (size_t i = 0; i < strlen(enc); i++)
      switch (enc[i]){
         case '&' : sb->add("&amp;");   f = false; break;
         case '<' : sb->add("&lt;");    f = false; break;
         case '>' : sb->add("&gt;");    f = false; break;
         case '"' : sb->add("&quot;");  f = false; break;
         case '\n': sb->add( phpn ? "<Br>" : "\n" ); break;
         //Old Ms Front Page alike spaces :)
         case ' ' : if (!f){ sb->add(" "); f = true; }
                    break;
	 default  : sb->add( enc[i] ); f = false;
      }

  return sb->get();
}

const char *Coder::numberEncode(const char *enc){
   if (isempty(enc))
      return "0";
      
   //long int x = atol(enc);
   sprintf(numtmp, "%ld", atol(enc));
   
   return numtmp;
}

const char *Coder::floatEncode(const char *enc, int toc){
   if (isempty(enc))
      return "0";
      
   //float x = atof(enc);

   //char format = "%0.xf"; format[4] = itoc(toc);
   //char format = {'%', '0', '.', itoc(toc), 'f'};

   char format[100];
   sprintf(format, "%%0.%df", toc);

   sprintf(numtmp, format, atof(enc));

/*   
   for (size_t i = strlen(numtmp); i > 0; i--)
      if ( isspace(numtmp[i - 1]) || numtmp[i - 1] == '0' )
         numtmp[i - 1] = '\0';
      else
         break;
*/
 
   return numtmp;
}

#define CPP_PATTERN "\'\"\\"
#define SH_PATTERN  CPP_PATTERN"`!&"

const char *Coder::CPPEncode(const char *enc, boolean sh){
   sb->clear();
   if (isempty(enc))
      return NULL;
      
   for (size_t i = 0; i < strlen(enc); i++){
      if ( strchr((sh) ? SH_PATTERN : CPP_PATTERN, enc[i]) )
         sb->add('\\');
      sb->add( enc[i] );
   }
   /*
      switch (enc[i]){
         case '\'': sb->add("\\\'");       break;
         case '\"': sb->add("\\\"");       break;
         case '\\': sb->add("\\\\");       break;
         case '`' : sb->add( (sh) ? "\\`" : "`");        break;
         case '!' : sb->add( (sh) ? "\\!" : "!");        break;
         case '&' : sb->add( (sh) ? "\\&" : "&");        break;

         default  : sb->add( enc[i] );
      }
   */
   return sb->get();
};

const char *Coder::CPPDecode(const char *enc){
   sb->clear();
   if (isempty(enc))
      return NULL;
      
   for (size_t i = 0; i < strlen(enc); i++)
      if (enc[i] == '\\')
         i++;
      else
         sb->add( enc[i] );

   return sb->get();
};

const char *Coder::sqlEncode(const char *enc){
   sb->clear();
   if (isempty(enc))
      return NULL;
      
   for (size_t i = 0; i < strlen(enc); i++)
      switch (enc[i]){
         case '\'':
         case '\"': sb->add("`");       break;

         default  : sb->add( enc[i] );
      }

   return sb->get();
};

const char *Coder::sqlNullEncode(const char *enc){
   sb->clear();
   if (isempty(enc))
      return "NULL";
      
   sb->add('\'');
   sb->add(enc);
   sb->add('\'');

   return sb->get();
};
