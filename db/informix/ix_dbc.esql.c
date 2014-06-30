#include <sqlhdr.h>
#include <sqlca.h>

#if !defined(__STDC__)
#define const 
#endif

#line 1 "ix_dbc.esql.ec"
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
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 31 "ix_dbc.esql.ec"

#line 32 "ix_dbc.esql.ec"
char msg[FIELD_BUFF_SIZE];
int len;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 34 "ix_dbc.esql.ec"


/*
 *    EXEC SQL get diagnostics exception 1
 *                           :msg = MESSAGE_TEXT,
 *                           :len = MESSAGE_LENGTH;
 */
#line 36 "ix_dbc.esql.ec"
  {
#line 36 "ix_dbc.esql.ec"
  static struct hostvar_struct _SQhtab[] =
    {
      { 0, 6, 100, sizeof(msg), 0 },
      { 0, 7, 102, sizeof(len), 0 },
      { 0, 0, 0, 0, 0 }
#line 38 "ix_dbc.esql.ec"
    };
  _SQhtab[0].hostaddr = (msg);
  _SQhtab[1].hostaddr = (char *)&len;
#line 38 "ix_dbc.esql.ec"
  _iqgetdiag(_SQhtab, 1);
#line 38 "ix_dbc.esql.ec"
  }

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
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 54 "ix_dbc.esql.ec"

#line 55 "ix_dbc.esql.ec"
char db_name[DB_BUFF_SIZE];
char db_user[DB_BUFF_SIZE];
char db_pass[DB_BUFF_SIZE];
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 58 "ix_dbc.esql.ec"


   //Init error function
   //The #defines will NOT help there :(((, but can help in function itself
/*
 *    EXEC SQL whenever sqlerror call ix_err;
 */
#line 62 "ix_dbc.esql.ec"


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
/*
 *       EXEC SQL connect to :db_name
 *                     USER  :db_user
 *                     USING :db_pass
 *                     WITH   CONCURRENT TRANSACTION;
 */
#line 78 "ix_dbc.esql.ec"
  {
#line 81 "ix_dbc.esql.ec"
  _iqconnect(0, db_name, (char *) 0, db_user, db_pass, 1);
#line 81 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 81 "ix_dbc.esql.ec"
    ix_err();
#line 81 "ix_dbc.esql.ec"
  }
   }else{
/*
 *       EXEC SQL connect to :db_name WITH CONCURRENT TRANSACTION;
 */
#line 83 "ix_dbc.esql.ec"
  {
#line 83 "ix_dbc.esql.ec"
  _iqconnect(0, db_name, (char *) 0, (char *) 0, (char *) 0, 1);
#line 83 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 83 "ix_dbc.esql.ec"
    ix_err();
#line 83 "ix_dbc.esql.ec"
  }
   }

   return chkerr();
};

void IxDBConnection::disconnect(){
/*
 *    EXEC SQL disconnect current;
 */
#line 90 "ix_dbc.esql.ec"
  {
#line 90 "ix_dbc.esql.ec"
  _iqdisconnect(3, (char *) 0, 0, 0);
#line 90 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 90 "ix_dbc.esql.ec"
    ix_err();
#line 90 "ix_dbc.esql.ec"
  }
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void IxDBC::setSQL(const char *sql){
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 98 "ix_dbc.esql.ec"

#line 99 "ix_dbc.esql.ec"
char sql1[SQL_BUFF_SIZE];
char *syssel =  IX_SEL;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 101 "ix_dbc.esql.ec"


   //Set SQL
   strncpy(sql1, sql, SQL_BUFF_SIZE);
/*
 *    EXEC SQL prepare :syssel from :sql1;
 */
#line 105 "ix_dbc.esql.ec"
  {
#line 105 "ix_dbc.esql.ec"
#line 105 "ix_dbc.esql.ec"
  _iqnprep((char *) syssel, sql1);
#line 105 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 105 "ix_dbc.esql.ec"
    ix_err();
#line 105 "ix_dbc.esql.ec"
  }

   sqlstate = chkerr();
};

boolean IxDBC::exec(){
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 111 "ix_dbc.esql.ec"

#line 112 "ix_dbc.esql.ec"
char *syssel =  IX_SEL;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 113 "ix_dbc.esql.ec"


/*
 *    EXEC SQL execute :syssel;
 */
#line 115 "ix_dbc.esql.ec"
  {
#line 115 "ix_dbc.esql.ec"
#line 115 "ix_dbc.esql.ec"
  _iqexecute(_iqlocate_cursor((char *) syssel, 101), (struct sqlda *) 0, (char *) 0, (struct value *) 0, (struct sqlda *) 0, (char *) 0, (struct value *) 0, 0);
#line 115 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 115 "ix_dbc.esql.ec"
    ix_err();
#line 115 "ix_dbc.esql.ec"
  }

   calcAffectedCount();
   return chkerr();
};

boolean IxDBC::open(){
   if (!sqlstate)
      return false;

/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 125 "ix_dbc.esql.ec"

#line 126 "ix_dbc.esql.ec"
char *sysdesc =  IX_DESCR;
char *syssel =  IX_SEL;
char *syscurs =  IX_CURS;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 129 "ix_dbc.esql.ec"


/*
 *    EXEC SQL declare :syscurs cursor for :syssel;
 */
#line 131 "ix_dbc.esql.ec"
  {
#line 131 "ix_dbc.esql.ec"
#line 131 "ix_dbc.esql.ec"
  _iqcddcl(_iqlocate_cursor((char *) syscurs, 0), (char *) syscurs, _iqlocate_cursor((char *) syssel, 1), 0);
#line 131 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 131 "ix_dbc.esql.ec"
    ix_err();
#line 131 "ix_dbc.esql.ec"
  }
/*
 *    EXEC SQL allocate descriptor :sysdesc;
 */
#line 132 "ix_dbc.esql.ec"
  {
#line 132 "ix_dbc.esql.ec"
  _iqalloc(sysdesc, 100);
#line 132 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 132 "ix_dbc.esql.ec"
    ix_err();
#line 132 "ix_dbc.esql.ec"
  }
/*
 *    EXEC SQL describe :syssel using sql descriptor :sysdesc;
 */
#line 133 "ix_dbc.esql.ec"
  {
#line 133 "ix_dbc.esql.ec"
#line 133 "ix_dbc.esql.ec"
  _iqdescribe(_iqlocate_cursor((char *) syssel, 101), (struct sqlda **) 0, sysdesc);
#line 133 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 133 "ix_dbc.esql.ec"
    ix_err();
#line 133 "ix_dbc.esql.ec"
  }

   if (!chkerr())
      return false;

/*
 *    EXEC SQL open :syscurs;
 */
#line 138 "ix_dbc.esql.ec"
  {
#line 138 "ix_dbc.esql.ec"
#line 138 "ix_dbc.esql.ec"
  _iqdcopen(_iqlocate_cursor((char *) syscurs, 100), (struct sqlda *) 0, (char *) 0, (struct value *) 0, 0, 0);
#line 138 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 138 "ix_dbc.esql.ec"
    ix_err();
#line 138 "ix_dbc.esql.ec"
  }

   _eof=false;

   next();

   calcAffectedCount(); //It must be aftter fetch row

   return chkerr();
};

void IxDBC::close(){
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 150 "ix_dbc.esql.ec"

#line 151 "ix_dbc.esql.ec"
char *sysdesc =  IX_DESCR;
char *syssel =  IX_SEL;
char *syscurs =  IX_CURS;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 154 "ix_dbc.esql.ec"


/*
 *    EXEC SQL free :syssel;                   ** free resources for statement **
 */
#line 156 "ix_dbc.esql.ec"
  {
#line 156 "ix_dbc.esql.ec"
#line 156 "ix_dbc.esql.ec"
  _iqfree(_iqlocate_cursor((char *) syssel, 102));
#line 156 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 156 "ix_dbc.esql.ec"
    ix_err();
#line 156 "ix_dbc.esql.ec"
  }
/*
 *    EXEC SQL free :syscurs;                  ** free resources for cursor **
 */
#line 157 "ix_dbc.esql.ec"
  {
#line 157 "ix_dbc.esql.ec"
#line 157 "ix_dbc.esql.ec"
  _iqfree(_iqlocate_cursor((char *) syscurs, 102));
#line 157 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 157 "ix_dbc.esql.ec"
    ix_err();
#line 157 "ix_dbc.esql.ec"
  }
/*
 *    EXEC SQL deallocate descriptor :sysdesc; ** free system descriptor area **
 */
#line 158 "ix_dbc.esql.ec"
  {
#line 158 "ix_dbc.esql.ec"
  _iqdealloc(sysdesc);
#line 158 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 158 "ix_dbc.esql.ec"
    ix_err();
#line 158 "ix_dbc.esql.ec"
  }
};

void IxDBC::next(){
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 162 "ix_dbc.esql.ec"

#line 163 "ix_dbc.esql.ec"
char *sysdesc =  IX_DESCR;
char *syscurs =  IX_CURS;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 165 "ix_dbc.esql.ec"


/*
 *    EXEC SQL fetch :syscurs using sql descriptor :sysdesc;
 */
#line 167 "ix_dbc.esql.ec"
  {
#line 167 "ix_dbc.esql.ec"
  static _FetchSpec _FS0 = { 0, 1, 0 };
#line 167 "ix_dbc.esql.ec"
  _iqcftch(_iqlocate_cursor((char *) syscurs, 100), (struct sqlda *) 0, (struct sqlda *) 0, sysdesc, &_FS0);
#line 167 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 167 "ix_dbc.esql.ec"
    ix_err();
#line 167 "ix_dbc.esql.ec"
  }

   if (!chkerr())
      _eof=true;
   else
      _eof=strncmp(SQLSTATE, "02", 2) == 0;
};

unsigned int IxDBC::getFieldCount(){
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 176 "ix_dbc.esql.ec"

#line 177 "ix_dbc.esql.ec"
int sels;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 178 "ix_dbc.esql.ec"


/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 180 "ix_dbc.esql.ec"

#line 181 "ix_dbc.esql.ec"
char *sysdesc =  IX_DESCR;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 182 "ix_dbc.esql.ec"


/*
 *    EXEC SQL get descriptor :sysdesc :sels = COUNT;
 */
#line 184 "ix_dbc.esql.ec"
  {
#line 184 "ix_dbc.esql.ec"
  static struct hostvar_struct _SQhtab[] =
    {
      { 0, 0, 102, sizeof(sels), 0 },
      { 0, 0, 0, 0, 0 }
#line 184 "ix_dbc.esql.ec"
    };
  _SQhtab[0].hostaddr = (char *)&sels;
#line 184 "ix_dbc.esql.ec"
  _iqgetdesc(sysdesc, -1, _SQhtab, 0);
#line 184 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 184 "ix_dbc.esql.ec"
    ix_err();
#line 184 "ix_dbc.esql.ec"
  }

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
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 198 "ix_dbc.esql.ec"

#line 199 "ix_dbc.esql.ec"
int no1;
static char data[FIELD_BUFF_SIZE];
int type;
int len;
short ind;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 204 "ix_dbc.esql.ec"


/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 206 "ix_dbc.esql.ec"

#line 207 "ix_dbc.esql.ec"
char *sysdesc =  IX_DESCR;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 208 "ix_dbc.esql.ec"


   /* ESQL hack :) */
   no1=(unsigned int) no + 1;

/*
 *    EXEC SQL get descriptor :sysdesc VALUE	:no1
 * 						:type = TYPE,
 * 						:len = LENGTH,
 * 						:ind = INDICATOR,
 * 						:data = DATA;
 */
#line 213 "ix_dbc.esql.ec"
  {
#line 213 "ix_dbc.esql.ec"
  static struct hostvar_struct _SQhtab[] =
    {
      { 0, 1, 102, sizeof(type), 0 },
      { 0, 2, 102, sizeof(len), 0 },
      { 0, 5, 101, sizeof(ind), 0 },
      { 0, 6, 100, sizeof(data), 0 },
      { 0, 0, 0, 0, 0 }
#line 217 "ix_dbc.esql.ec"
    };
  _SQhtab[0].hostaddr = (char *)&type;
  _SQhtab[1].hostaddr = (char *)&len;
  _SQhtab[2].hostaddr = (char *)&ind;
  _SQhtab[3].hostaddr = (data);
#line 217 "ix_dbc.esql.ec"
  _iqgetdesc(sysdesc, no1, _SQhtab, 0);
#line 217 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 217 "ix_dbc.esql.ec"
    ix_err();
#line 217 "ix_dbc.esql.ec"
  }

   if (ind == -1)
      return NULL;

   trim_r(data, FIELD_BUFF_SIZE);
   return data;
};

const char *IxDBC::getFieldName(unsigned int no){
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 227 "ix_dbc.esql.ec"

#line 228 "ix_dbc.esql.ec"
int no1;
static char name[FIELDNAME_BUFF_SIZE];
int type;
int len;
short ind;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 233 "ix_dbc.esql.ec"


/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 235 "ix_dbc.esql.ec"

#line 236 "ix_dbc.esql.ec"
char *sysdesc =  IX_DESCR;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 237 "ix_dbc.esql.ec"


   /* ESQL hack :) */
   no1=(unsigned int) no + 1;
   //printf("(((%d)))\n", no);
/*
 *    EXEC SQL get descriptor :sysdesc VALUE	:no1						:name = NAME,
 * 						:name = NAME,
 * 						:type = TYPE,
 * 						:len = LENGTH,
 * 						:ind = INDICATOR;
 */
#line 242 "ix_dbc.esql.ec"
  {
#line 242 "ix_dbc.esql.ec"
  static struct hostvar_struct _SQhtab[] =
    {
      { 0, 7, 100, sizeof(name), 0 },
      { 0, 7, 100, sizeof(name), 0 },
      { 0, 1, 102, sizeof(type), 0 },
      { 0, 2, 102, sizeof(len), 0 },
      { 0, 5, 101, sizeof(ind), 0 },
      { 0, 0, 0, 0, 0 }
#line 246 "ix_dbc.esql.ec"
    };
  _SQhtab[0].hostaddr = (name);
  _SQhtab[1].hostaddr = (name);
  _SQhtab[2].hostaddr = (char *)&type;
  _SQhtab[3].hostaddr = (char *)&len;
  _SQhtab[4].hostaddr = (char *)&ind;
#line 246 "ix_dbc.esql.ec"
  _iqgetdesc(sysdesc, no1, _SQhtab, 0);
#line 246 "ix_dbc.esql.ec"
  if (SQLCODE < 0)
#line 246 "ix_dbc.esql.ec"
    ix_err();
#line 246 "ix_dbc.esql.ec"
  }

   trim_r(name, FIELDNAME_BUFF_SIZE);

   //printf(">%2i | %-10s | %3d | %3d | %3d | %s<\n", no, name, type, len, ind, (ind == -1) ? NULL : data);
   return name;
};

void IxDBC::calcAffectedCount(){
/*
 *    EXEC SQL BEGIN DECLARE SECTION;
 */
#line 255 "ix_dbc.esql.ec"

#line 256 "ix_dbc.esql.ec"
int count;
/*
 *    EXEC SQL END DECLARE SECTION;
 */
#line 257 "ix_dbc.esql.ec"


/*
 *    EXEC SQL get diagnostics :count = ROW_COUNT;
 */
#line 259 "ix_dbc.esql.ec"
  {
#line 259 "ix_dbc.esql.ec"
  static struct hostvar_struct _SQhtab[] =
    {
      { 0, 8, 102, sizeof(count), 0 },
      { 0, 0, 0, 0, 0 }
#line 259 "ix_dbc.esql.ec"
    };
  _SQhtab[0].hostaddr = (char *)&count;
#line 259 "ix_dbc.esql.ec"
  _iqgetdiag(_SQhtab, -1);
#line 259 "ix_dbc.esql.ec"
  }

   affectedcount = count;
};

#line 262 "ix_dbc.esql.ec"
