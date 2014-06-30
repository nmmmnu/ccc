#include <stdio.h>
#include <ctype.h>

/*
EXEC SQL include sqltypes;
EXEC SQL include locator;
EXEC SQL include datetime;
EXEC SQL include decimal;
*/
#define SQLBUFFSZ 256

/*
EXEC SQL BEGIN DECLARE SECTION;
    loc_t lcat_descr;
    loc_t lcat_picture; 
EXEC SQL END DECLARE SECTION; 
*/

int ix_err(); /* Error function */

long getrow(char *sysdesc);

int main(){
   long ret;
   short data_found = 0;

   EXEC SQL BEGIN DECLARE SECTION;
        char sql[SQLBUFFSZ], db_name[30];
        char name[40];
        int sel_cnt, i;
        short type;
        char *user   = "postgres";
        char *passwd = "ContactS";
   EXEC SQL END DECLARE SECTION;

   EXEC SQL whenever sqlerror call ix_err;

printf("Will\n");
putenv("DBPATH=/usr/local/informix/data/");
   strcpy(db_name, "addr");
   EXEC SQL CONNECT TO  'addr@crypthon_se'
                  USER  :user
                  USING :passwd
                  WITH   CONCURRENT TRANSACTION;

printf("Connected! \n");

   strcpy(sql, "select * from abc");
   EXEC SQL prepare sel_id from :sql;

   EXEC SQL declare sel_curs cursor for sel_id;  
   EXEC SQL allocate descriptor 'selcat';
   EXEC SQL describe sel_id using sql descriptor 'selcat';

   if(SQLCODE != 0){
      printf("** Statement is not a SELECT.\n");
      free_stuff();
      EXEC SQL disconnect current;
      exit(0);
   }

   /* Determine the number of columns in the select list */

   EXEC SQL get descriptor 'selcat' :sel_cnt = COUNT;
   printf("There are %d columns\n", sel_cnt);

   EXEC SQL open sel_curs;

   /* fetch a rows */ 
   while(ret = getrow("selcat")){
      if(ret < 0){
         EXEC SQL disconnect current;
 	 exit(0);
      }
      disp_data(sel_cnt, "selcat"); /* display the data */
   }
   printf("** EOF **\n");
   free_stuff();
   return 0;
}

long getrow(sysdesc)
EXEC SQL BEGIN DECLARE SECTION;
    PARAMETER char *sysdesc;
EXEC SQL END DECLARE SECTION;
{
    EXEC SQL fetch sel_curs using sql descriptor :sysdesc;

    return ( strncmp(SQLSTATE, "02", 2) != 0 );
}

#define MAX_FIELD_LEN 4096

disp_data(col_cnt, sysdesc)
int col_cnt;
EXEC SQL BEGIN DECLARE SECTION;
    PARAMETER char * sysdesc;
EXEC SQL END DECLARE SECTION;
{
    EXEC SQL BEGIN DECLARE SECTION;
        int   i;
        short ind;
        char  name[MAX_FIELD_LEN];
        char  data[MAX_FIELD_LEN];
	int   type;
       	int   len;
    EXEC SQL END DECLARE SECTION;
    unsigned int x;

    for(i = 1; i <= col_cnt; i++)
	{
        EXEC SQL get descriptor :sysdesc VALUE	:i
						:name = NAME,
						:type = TYPE,
						:len = LENGTH,
						:ind = INDICATOR,
						:data = DATA;

        for(x=MAX_FIELD_LEN - 1 - 1; x > 0; x--)
           if (data[x]==' ')
              data[x]='\0';
           else
              break;

        for(x=MAX_FIELD_LEN - 1 - 1; x > 0; x--)
           if (name[x]==' ')
              name[x]='\0';
           else
              break;

        printf(">%2i | %-5s | %3d | %3d | %3d | %s<\n", i, name, type, len, ind, (ind == -1) ? NULL : data);

        }
} 

free_stuff()
{

    EXEC SQL free sel_id;	/* free resources for statement */
    EXEC SQL free sel_curs;	/* free resources for cursor */

    /* free system descriptor area */
    EXEC SQL deallocate descriptor 'selcat'; 
}

ix_err()
{
   EXEC SQL BEGIN DECLARE SECTION;
        char msg[1024];
        int  len; 
   EXEC SQL END DECLARE SECTION;

   EXEC SQL get diagnostics exception 1 
                          :msg = MESSAGE_TEXT,
                          :len = MESSAGE_LENGTH;

   msg[len] = '\0';
   printf("Informix Error (%s) %s\n", SQLSTATE, msg);

   exit(1);
}
