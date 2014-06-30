include ../Makedef
include ../db/Makefile.dbc

SHARED_A_DBC=drv/$(DBC_NAME)_dbc.so

all:
	$(MAKE_LIBMAD)
	$(MAKE_LIBA_DBC)

remarka:
	#make $(SHARED_A_DBC)
	#$(SHARED_A_DBC): liba_dbc.a
	
	#make shared A_DBC driver as well
	. mk.a_dbc $(SHARED_A_DBC) libmad.a liba_dbc.a $(DBC_LIB)

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *.so.*
	rm -f *~
	
	$(CLEAN_LIBMAD)
	$(CLEAN_LIBA_DBC)
