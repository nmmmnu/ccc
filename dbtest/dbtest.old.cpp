#include <db/a_dbc.h>

#include <mad/prop.h>
#include <mad/parsers.h>

//#define TODO exec
#define TODO select

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

void exec(ADBC *db, ...){
   printf(">Exec SQL...\n");
   if (!db->exec()){
      printf("Error\n");
      exit(1);
   }
   printf("Affect %ld row(s)\n", db->getAffectedCount());
}

int main(void){
#ifdef __MFC__
   if (!initMFC()){
      printf("content-type: text/plain\n\nMFC Crashed\nOh, dear!\n"); 
      exit(100);
   }
#endif

   printf("content-type: text/plain\n\n\n\n");  //for web testing

   Properties *p = new Properties();
   Properties *q = new Properties();
   
   //Seting Database properties, they are depend of database
//   p->put("DATABASEDRIVER", "./mysql_dbc.so");
//   p->put("DATABASE", "./database.db" );
   p->put("DATABASE", "./type.txt" );
//   p->put("DATABASEHOST", "photon" );
//   p->put("DATABASEUSER", "db5" );
//   p->put("DATABASEPASS", "*********" );
//   p->put("DATABASE", "proba" );
//   p->put("CONNECTIONSTRING", "DSN=access_proba" );
   

//   p->put("DATABASEHOST", "photon" );
//   p->put("DATABASEUSER", "postgres" );
//   p->put("DATABASEPASS", "-----" );
   
//   p->put("DATABASE", getenv("adbc_db") );
//   p->put("DATABASE", "/usr/interbase/examples/v4/employee.gdb" );
//   p->put("DATABASEUSER", "SYSDBA");
//   p->put("DATABASEPASS", "masterkey");
//   p->putAsInt("DATABASELIVEQUERY", 0);

   printf(">CREATE...\n");

   ADBConnection *dbcon = getADBConnection();

   printf(">CONNECT...\n");
   if (!dbcon->connect(p)){
      printf("Error\n");
      exit(1);
   }

   ADBC *db = dbcon->getADBC();

   printf(">SQL...\n");
   db->setSQL(
""
//             "name=Niki&no=1"
//             getenv("adbc_sql")
//           "select * from hora"
//           "insert into customer (fname) values ('nmmm')"
//           "delete from customer where fname = 'nmmm'"
//           "Create table hora( num integer, name char(20) )"
//           "Insert into hora values(1, 'Niki' )"
   );

   TODO(db, q);

   delete(db);
   
   printf(">Disconnect...\n");
   dbcon->disconnect();
   delete(dbcon);
   delete(p);
   delete(q);
   printf(">Exit\n");

   return 0;
}
