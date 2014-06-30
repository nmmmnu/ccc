ESQL= ${INFORMIXDIR}/bin/esql

all: ix_dbc.esql.c

ix_dbc.esql.c: ix_dbc.esql.ec
	$(ESQL) -e ix_dbc.esql.ec
	touch ix_dbc.cpp

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~
	rm -f ix_dbc.esql.c

	rm -f esql_demo.c
	rm -f a.out
