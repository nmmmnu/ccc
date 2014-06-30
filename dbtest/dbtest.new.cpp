#include <db/a_dbc.h>

#include <mad/prop.h>
#include <mad/parsers.h>

void dbcGet(ADBC *dbc, Properties *p){
   for (unsigned int i = 0; i < dbc->getFieldCount(); i++){
      p->put(dbc->getFieldName(i), dbc->getFieldData(i));
   }
}

void select(ADBC *db, Properties *q){
   printf(">OPEN...\n");
   if (!db->open()){
      printf("Error\n");
      exit(1);
   }

   printf("Database matrix (cols) x (rows): (%d) x (%ld)\n", db->getFieldCount(), db->getRecordCount() );
//   db->gotoRecordNum(3);

   while (!db->eof()){
      dbcGet(db, q);
      q->dump();
      db->next();
      //read();
   }
 
   db->close();
}

void exec(ADBC *db){
   printf(">Exec SQL...\n");
   if (!db->exec()){
      printf("Error\n");
      exit(1);
   }
   printf("Affect %ld row(s)\n", db->getAffectedCount());
}

/*
Usage:
	dbtest 0|1 "profile"
Where:
	0 : Select
	1 : Exec
Extra profile fields:
	EXEC_SQL
	SELECT_SQL
*/

#define OPERATION	(argv[1] ? argv[1][0] == '1' : 0)
#define DEFPROFILE	argv[2]
#define PROFILE		((argc > 1 && DEFPROFILE) ? DEFPROFILE : "test.dsn")
#define EXEC_SQL	"EXEC_SQL"
#define SELECT_SQL	"SQLECT_SQL"

int main(int argc, const char *argv[]){
#ifdef __MFC__
   if (!initMFC()){
      printf("content-type: text/plain\n\nMFC Crashed\nOh, dear!\n"); 
      exit(100);
   }
#endif

   printf("content-type: text/plain\n\n\n\n");  //for web testing

   Properties *p = new Properties();
   Properties *q = new Properties();

   printf("Current driver: %s\n", a_dbc_drivername);
   
   printf(">Loaded DSN: %s\n", PROFILE);
   p->load(PROFILE);
   p->dump();

   printf(">CREATE...\n");

   ADBConnection *dbcon = getADBConnection();

   printf(">CONNECT...\n");
   if (!dbcon->connect(p)){
      printf("Error\n");
      exit(1);
   }

   ADBC *db = dbcon->getADBC();

   printf(">SQL...\n");

   if (OPERATION){
      db->setSQL(p->get("EXEC_SQL"));
      exec(db);
   }else{
      db->setSQL(p->get("SELECT_SQL"));
      select(db, q);
   }

   delete(db);
   
   printf(">Disconnect...\n");
   dbcon->disconnect();
   delete(dbcon);
   delete(p);
   delete(q);
   printf(">Exit\n");

   return 0;
}
