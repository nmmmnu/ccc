######################################
#                                    #
#  MAD Software Standard components  #
#                                    #
######################################

include ../Makedef

all_objs=	prop.o mad.o macro.o parsers.o textfile.o coder.o \
		xml_parser.o expr.o \
		dpipe.o

all: $(LIBRARYTYPE)

static:
	rm -f $(SHAREDLIBDIR)/$(LIBMAD.SO)
	rm -f $(SHAREDLIBDIR)/$(LIBMAD.SO.VER)
	rm -f $(SHAREDLIBDIR)/$(LIBMAD.SO.NAME)
	
	make $(LIBMAD)

shared:
	#make static version as well
	make static
	
	make $(SHAREDLIBDIR)/$(LIBMAD.SO)
	cat README.SHARED

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~

$(LIBMAD): $(all_objs)
	$(ARC) $(LIBMAD) $(all_objs)

$(SHAREDLIBDIR)/$(LIBMAD.SO):	$(all_objs)
	$(LD_SO) $(SHAREDLIBDIR)/$(LIBMAD.SO) $(all_objs)
	
	ln -sf $(LIBMAD.SO) $(SHAREDLIBDIR)/$(LIBMAD.SO.VER)
	ln -sf $(LIBMAD.SO) $(SHAREDLIBDIR)/$(LIBMAD.SO.NAME)

prop.o: prop.cpp prop.h mad.h textfile.h
	$(CC) prop.cpp

mad.h: port.h
	touch mad.h

mad.o: mad.cpp mad.h
	$(CC) mad.cpp

macro.o: macro.cpp macro.h mad.h prop.h
	$(CC) macro.cpp

parsers.o: parsers.cpp parsers.h mad.h prop.h
	$(CC) parsers.cpp

textfile.o: textfile.cpp textfile.h mad.h
	$(CC) textfile.cpp

coder.o: coder.cpp coder.h mad.h
	$(CC) coder.cpp

xml_parser.o: xml_parser.h xml_parser.cpp mad.h prop.h
	$(CC) xml_parser.cpp

expr.o: expr.cpp expr.i.cpp expr.h mad.h
	$(CC) expr.cpp

dpipe.o: sys/dpipe.cpp sys/dpipe.h mad.h
	$(CC) sys/dpipe.cpp
