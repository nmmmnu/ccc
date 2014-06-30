#include <stdio.h>
#include <afxdb.h>

void doit(){
   CDatabase db;
   try{
      db.OpenEx( NULL, CDatabase::forceOdbcDialog );

	  char *format = "%-15s : %s\n";

	  printf(format, "Database",     db.GetDatabaseName());
	  printf(format, "ODBC String",  db.GetConnect());
	  printf(format, "Transactions", db.CanTransact() ? "Yes" : "Nope");

      db.Close();
   }catch (CException *){
	   printf("Error during connecting...\n");
	   exit(2);
   };
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

inline BOOL initMFC(){
   // initialize MFC and print and error on failure
	return AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
}

int main(){
   if (!initMFC()){
      printf("MFC initialization failed\n");
      return 1;
   }

	
   printf("=============================\n");
   printf("ODBC Param Helper Version 1.0\n");
   printf("=============================\n");
   printf("\tCopyleft 01.2000, Nikolay Mijaylov\n");
   printf("\t\"http://www.nmmm.nu/\" <nmmm@nmmm.nu>\n");
   printf("\tDistribution: GPL\n");
   printf("\n");

   doit();

   printf("[cr]\n"); char c = 0; scanf("%c", &c);

   return 0;
}
