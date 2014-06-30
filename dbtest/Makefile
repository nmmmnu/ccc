################################
#                              #
# Abstract Data Base Connector #
#   DB Test program Makefile   #
#                              #
################################

THE_TARGET= dbtest
include ../Makedef
include ../db/Makefile.dbc

all:
	#$(MAKE_LIBMAD)
	#$(MAKE_LIBA_DBC)
	make $(THE_TARGET)

$(THE_TARGET):	dbtest.o
	$(LINK_TARGET)	dbtest.o -la_dbc -lmad $(DBC_LIB) -lc -lstdc++ -lm

dbtest.o: dbtest.cpp ../db/a_dbc.h
	$(CC) dbtest.cpp

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~
