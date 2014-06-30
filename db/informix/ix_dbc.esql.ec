#include <mad/mad.h>
#include <mad/prop.h>

#define DBC_DATABASE		"DATABASE"
//#define DBC_DATABASESERVER	"DATABASESERVER"
#define DBC_DATABASEUSER	"DATABASEUSER"
#define DBC_DATABASEPASS	"DATABASEPASS"

#define SQL_BUFF_SIZE       1024 * 10
#define FIELD_BUFF_SIZE     1024 * 10
#define DB_BUFF_SIZE        30
#define FIELDNAME_BUFF_SIZE 40

#define SQLSTATE_ERR        "02"
//#define SQLSTATE_ERR_LEN strlen(SQLSTATE_ERROR)
#define SQLSTATE_ERR_LEN    2

#define chkerr() ( strncmp(SQLSTATE, SQLSTATE_ERR, SQLSTATE_ERR_LEN) < 0 )

//Uncomment this for extended Informix messages,
//direct from Informix server to LOG_NOTIFY_DEBUG
#define INFORMIX_DEBUG

#ifndef INFORMIX_DEBUG

#define ix_err()

#else

int ix_err(){
   EXEC SQL BEGIN DECLARE SECTION;
        char msg[FIELD_BUFF_SIZE];
        int  len; 
   EXEC SQL END DECLARE SECTION;

   EXEC SQL get diagnostics exception 1 
                          :msg = MESSAGE_TEXT,
                          :len = MESSAGE_LENGTH;

   msg[len] = '\0';
   printflog(LOG_NOTIFY_DEBUG, "Informix Error (%s) %s\n", SQLSTATE, msg);

   //exit(1); 
   return 0;
}

#endif

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

boolean IxDBConnection::connect(Properties *cfg){
   EXEC SQL BEGIN DECLARE SECTION;
        char db_name[DB_BUFF_SIZE];
        char db_user[DB_BUFF_SIZE];
        char db_pass[DB_BUFF_SIZE];
   EXEC SQL END DECLARE SECTION;

   //Init error function
   //The #defines will NOT help there :(((, but can help in function itself
   EXEC SQL whenever sqlerror call ix_err;

/*
   if (cfg->exist(DBC_DATABASESERVER)) 
      strcpy(InetLogin.InfxServer, cfg->get(DBC_DATABASESERVER));
*/

   //Connect to the db
   strncpy(db_name, cfg->get(DBC_DATABASE,     ""), DB_BUFF_SIZE );

   if (cfg->exist(DBC_DATABASEUSER))
      strncpy(db_user, cfg->get(DBC_DATABASEUSER, ""), DB_BUFF_SIZE );
   if (cfg->exist(DBC_DATABASEPASS))
      strncpy(db_pass, cfg->get(DBC_DATABASEPASS, ""), DB_BUFF_SIZE );

   if (cfg->exist(DBC_DATABASEUSER) && cfg->exist(DBC_DATABASEPASS)){
      EXEC SQL connect to :db_name 
                    USER  :db_user
                    USING :db_pass
                    WITH   CONCURRENT TRANSACTION;
   }else{
      EXEC SQL connect to :db_name WITH CONCURRENT TRANSACTION;
   }

   return chkerr();
};

void IxDBConnection::disconnect(){
   EXEC SQL disconnect current;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void IxDBC::setSQL(const char *sql){
   EXEC SQL BEGIN DECLARE SECTION;
        char  sql1[SQL_BUFF_SIZE];
        char *syssel = IX_SEL;
   EXEC SQL END DECLARE SECTION;

   //Set SQL
   strncpy(sql1, sql, SQL_BUFF_SIZE);
   EXEC SQL prepare :syssel from :sql1;

   sqlstate = chkerr();
};

boolean IxDBC::exec(){
   EXEC SQL BEGIN DECLARE SECTION;
        char *syssel = IX_SEL;
   EXEC SQL END DECLARE SECTION;

   EXEC SQL execute :syssel;

   calcAffectedCount();
   return chkerr();
};

boolean IxDBC::open(){
   if (!sqlstate)
      return false;

   EXEC SQL BEGIN DECLARE SECTION;
        char *sysdesc = IX_DESCR;
        char *syssel  = IX_SEL;
        char *syscurs = IX_CURS;
   EXEC SQL END DECLARE SECTION;

   EXEC SQL declare :syscurs cursor for :syssel;  
   EXEC SQL allocate descriptor :sysdesc;
   EXEC SQL describe :syssel using sql descriptor :sysdesc;

   if (!chkerr())
      return false;

   EXEC SQL open :syscurs;

   _eof=false;

   next();

   calcAffectedCount(); //It must be aftter fetch row

   return chkerr();
};

void IxDBC::close(){
   EXEC SQL BEGIN DECLARE SECTION;
        char *sysdesc = IX_DESCR;
        char *syssel  = IX_SEL;
        char *syscurs = IX_CURS;
   EXEC SQL END DECLARE SECTION;
    
   EXEC SQL free :syssel;                   /* free resources for statement */
   EXEC SQL free :syscurs;                  /* free resources for cursor */
   EXEC SQL deallocate descriptor :sysdesc; /* free system descriptor area */
};

void IxDBC::next(){
   EXEC SQL BEGIN DECLARE SECTION;
        char *sysdesc = IX_DESCR;
        char *syscurs = IX_CURS;
   EXEC SQL END DECLARE SECTION;

   EXEC SQL fetch :syscurs using sql descriptor :sysdesc;

   if (!chkerr())
      _eof=true;
   else
      _eof=strncmp(SQLSTATE, "02", 2) == 0;
};

unsigned int IxDBC::getFieldCount(){
   EXEC SQL BEGIN DECLARE SECTION;
        int sels;
   EXEC SQL END DECLARE SECTION;

   EXEC SQL BEGIN DECLARE SECTION;
        char *sysdesc = IX_DESCR;
   EXEC SQL END DECLARE SECTION;

   EXEC SQL get descriptor :sysdesc :sels = COUNT;

   return (unsigned int) sels;
};

void IxDBC::trim_r(char a[], size_t len){
   for(size_t i = len - 1 - 1; i > 0; i--)
      if (a[i] == ' ')
         a[i] = '\0';
      else
         return;
}

const char *IxDBC::getFieldData(unsigned int no){
   EXEC SQL BEGIN DECLARE SECTION;
        int   no1;
        static char  data[FIELD_BUFF_SIZE];
	int   type;
       	int   len;
        short ind;
   EXEC SQL END DECLARE SECTION;

   EXEC SQL BEGIN DECLARE SECTION;
      char *sysdesc = IX_DESCR;
   EXEC SQL END DECLARE SECTION;

   /* ESQL hack :) */
   no1=(unsigned int) no + 1;

   EXEC SQL get descriptor :sysdesc VALUE	:no1
						:type = TYPE,
						:len = LENGTH,
						:ind = INDICATOR,
						:data = DATA;

   if (ind == -1)
      return NULL;

   trim_r(data, FIELD_BUFF_SIZE);
   return data;
};

const char *IxDBC::getFieldName(unsigned int no){
   EXEC SQL BEGIN DECLARE SECTION;
        int   no1;
        static char  name[FIELDNAME_BUFF_SIZE];
	int   type;
       	int   len;
        short ind;
   EXEC SQL END DECLARE SECTION;

   EXEC SQL BEGIN DECLARE SECTION;
      char *sysdesc = IX_DESCR;
   EXEC SQL END DECLARE SECTION;

   /* ESQL hack :) */
   no1=(unsigned int) no + 1;
   //printf("(((%d)))\n", no);
   EXEC SQL get descriptor :sysdesc VALUE	:no1						:name = NAME,
						:name = NAME,
						:type = TYPE,
						:len = LENGTH,
						:ind = INDICATOR;

   trim_r(name, FIELDNAME_BUFF_SIZE);

   //printf(">%2i | %-10s | %3d | %3d | %3d | %s<\n", no, name, type, len, ind, (ind == -1) ? NULL : data);
   return name;
};

void IxDBC::calcAffectedCount(){
   EXEC SQL BEGIN DECLARE SECTION;
        int  count; 
   EXEC SQL END DECLARE SECTION;

   EXEC SQL get diagnostics :count = ROW_COUNT;

   affectedcount = count;
};
