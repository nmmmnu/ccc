##########################
#                        #
#      Mad Software      #
#  Data Base Connectors  #
#                        #
##########################

include ../Makedef
include Makefile.dbc

all:
	$(MAKE_LIBMAD)
	$(DATABASE_SPECIFIC_CMD)
	make $(LIBA_DBC)

$(LIBA_DBC): a_dbc.o a_dbc_coder.o a_dbc_coder_null.o
	$(ARC) $(LIBA_DBC) a_dbc.o a_dbc_coder.o a_dbc_coder_null.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~

a_dbc.o: a_dbc.h a_dbc.cpp
	$(CC) a_dbc.cpp $(DBC_INC_DIR)

a_dbc_coder.o: a_dbc_coder.h a_dbc_coder.cpp
	$(CC) a_dbc_coder.cpp $(DBC_INC_DIR)

a_dbc_coder_null.o: a_dbc_coder.h a_dbc_coder_null.cpp
	$(CC) a_dbc_coder_null.cpp $(DBC_INC_DIR)
