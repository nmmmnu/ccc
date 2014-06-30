#####################################
#                                   #
#    MAD Software XML components    #
#                                   #
#####################################

include ../Makedef

THE_TARGET= demo

#all: $(LIBXML)
#	$(MAKE_LIBMAD)

demo: 
	make all;
	$(CC) demo.cpp
	$(LINK_TARGET)	demo.o -lxml -lmad

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~

#$(LIBXML): xml_parser.o
#	$(ARC) $(LIBXML) xml_parser.o
