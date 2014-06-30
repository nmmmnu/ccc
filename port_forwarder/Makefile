include ../Makedef

THE_TARGET= portfw

all:
	$(MAKE_LIBMAD)
	make $(THE_TARGET)

$(THE_TARGET): portfw.o
	$(LINK_TARGET)	portfw.o -lmad
	strip $(THE_TARGET)

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~

portfw.o: portfw.cpp
	$(CC) portfw.cpp
