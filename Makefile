ROOT_FILES= Makedef Makefile THANX ch_db ch_os mad_logo

ARCH= ccc.tar.gz
DBARCH= a_dbc.tar.gz
LIBARCH= mad.tar.gz

all:
	@cat ./mad_logo
	@echo "Usage:"
	@echo "	make arc --> make archive file of all"
	@echo "	make lib --> make archive file of LIBS"
	@echo "	make adbc -> make archive file of A_DBC (and LIBS)"
	@echo "	make dsk --> copy archive to disk A:"

clean:
	rm -f *.tar.gz

arc:
	make clean
	tar -c * | gzip > $(ARCH)

dsk:
	make arc
	mcopy *.tar.gz a:

lib:
	tar -c	\
		mad	\
		$(ROOT_FILES) | gzip > $(LIBARCH)

adbc:
	tar -c	\
		mad	\
		db	\
		dbtest	\
		$(ROOT_FILES) | gzip > $(DBARCH)

