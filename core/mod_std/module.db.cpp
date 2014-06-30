#include <stdio.h>
#include <math.h> //for ceil

#include "module.db.h"

#include <db/a_dbc.h>

void CGICoreModule__DBEXE__Process(Properties *, const char *);
void CGICoreModule__DBSEL__Process(Properties *, const char *);

CGICoreModule CGICoreModule__DBEXE = {
	"db exec",
	"A_DBC SQL exec",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"db_exe",
	NULL,
	NULL,
	CGICoreModule__DBEXE__Process
};

CGICoreModule CGICoreModule__DBSEL = {
	"db select",
	"A_DBC SQL select",
	"(c) 11.1999, \"Nikolay Mijaylov\" <nmmm@nmmm.nu>",
	"standard",
	100,
	300,
	true,
	"db_sel",
	NULL,
	NULL,
	CGICoreModule__DBSEL__Process
};

static const char   *err_html_matrix[]={"db_error", NULL};
static const char *dbsel_html_matrix[]={"db_sel",   NULL};
static const char *dbexe_html_matrix[]={"db_exe",   NULL};

#include "separator_helper.i.cpp"


unsigned long int initdbcount = 0;
ADBConnection *dbcon = NULL;

ADBC *initdb(Properties *p){
   ADBC *dbc = NULL;

   if (initdbcount == 0){
      dbcon = getADBConnection();
      if (dbcon->connect(cfg)) 
         initdbcount++;

      printflog(LOG_NOTIFY_DEBUG, "Module DB: Connection to the database (%ld)...\n", initdbcount);
   }else{
      initdbcount++;
      printflog(LOG_NOTIFY_DEBUG, "Module DB: Already Connected to the database (%ld)...\n", initdbcount);
   }

   if (initdbcount > 0){
      dbc = dbcon->getADBC();
      dbc->setSQL(p->get("SQL", "i know it will fail :(") );
   }

   return dbc;
}

void donedb(ADBC *dbc){

   if (initdbcount > 0){
      delete(dbc);
      initdbcount--;
   }

   if (initdbcount == 0){
      dbcon->disconnect();
      delete(dbcon);
      printflog(LOG_NOTIFY_DEBUG, "Module DB: Disconnection from the database (%ld)...\n", initdbcount + 1);
   }else
      printflog(LOG_NOTIFY_DEBUG, "Module DB: Keep database connection (%ld)...\n", initdbcount + 1);
}

void CGICoreModule__DBEXE__Process(Properties *p, const char *x){
   ADBC *dbc = initdb(p);
   boolean isOK = (dbc != NULL);

   if (isOK)
      isOK = dbc->exec();

   //All is finito, NOW lets check the result
   macro->putAsInt("DBERROR", !isOK);
   macro->putAsInt("DBAFFECT", isOK ? dbc->getAffectedCount() : 0 );

   do_separator(isOK, x, err_html_matrix, dbexe_html_matrix);

   donedb(dbc);
}

void dbcGet(ADBC *dbc, Properties *p){
   for (unsigned int i=0; i < dbc->getFieldCount(); i++)
      p->put(dbc->getFieldName(i),
             dbc->getFieldData(i));
};

#define getno(page) page_rows * (page - 1)
#define do_sep(a) do_separator(a, x, err_html_matrix, dbsel_html_matrix)

void CGICoreModule__DBSEL__Process(Properties *p, const char *x){
   macro->putAsInt("DBERROR", 0);

   ADBC *dbc = initdb(p);
   boolean isOK = (dbc != NULL);

   if (!isOK){
      do_sep(false);
      macro->putAsInt("DBERROR", 1);
      donedb(dbc);
      return;
   }

   isOK = dbc->open();

   if (!isOK){
      do_sep(false);
      macro->putAsInt("DBERROR", 1);
      donedb(dbc);

      return;
   }

   macro->putAsInt("DBTOTALRECORDS", dbc->getRecordCount());

   long int page_rows = p->getAsInt("PAGEROWS", 0);
   long int recordnum = 0;
   long int this_page = 0;

   if ((page_rows > 0            ) &&
       (dbc->getRecordCount() > 0)){
      long int total_pages = (long int) ceil((float) dbc->getRecordCount() / page_rows);

      this_page = p->getAsInt("PAGE", 1);
      this_page = this_page >= 1           ? this_page : 1;
      this_page = this_page <= total_pages ? this_page : total_pages;

      macro->putAsInt("DBTOTALPAGES", (int) total_pages);
      macro->putAsInt("DBPAGE",       (int) this_page);
      macro->putAsInt("DBNEXTPAGE",   (int) this_page + 1);
      macro->putAsInt("DBPREVPAGE",   (int) this_page - 1);
      macro->putAsInt("DBPAGEROWS",   (int) page_rows);

      dbc->gotoRecordNum( getno(this_page) );
      recordnum = getno(this_page);
   }
   
   while (!dbc->eof()){
      recordnum++;
      macro->putAsInt("DBRECORD", (int) recordnum);

      dbcGet(dbc, macro);
      do_sep(true);

      dbc->next();

      if ((page_rows > 0                     ) &&
          (recordnum >= getno(this_page + 1) ))
         break;
   }

   if (p->getAsInt("NULLERR", 0))
      if (dbc->getRecordCount() == 0)
         do_sep(false);

   dbc->close();

   donedb(dbc);
}
