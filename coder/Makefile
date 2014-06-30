include ../Makedef

THE_TARGET= shcode

all:
	$(MAKE_LIBMAD)
	make $(THE_TARGET)

$(THE_TARGET): code.o
	$(LINK_TARGET)	code.o -lmad -lstdc++
	strip $(THE_TARGET)

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~

code.o: code.cpp
	$(CC) code.cpp
