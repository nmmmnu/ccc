include ../Makedef

THE_TARGET= htmlconvert

all: 
	make $(THE_TARGET)

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~

$(THE_TARGET): html.o
	$(LINK_TARGET) html.o -lmad -lstdc++
	
	strip $(THE_TARGET)

html.o: html.cpp
	$(CC) html.cpp
