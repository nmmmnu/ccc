###########################
#                         #
#   CGI_Core3 - modules   #
#                         #
###########################

include ../../Makedef

all_mod=	module.cpy.o module.null.o module.shell.o module.pipe.o	\
		module.pipe2.o module.include.o module.define.o		\
		module.if.o module.db.o module.meta.o module.type.o	\
		module.for.o						\
									\
		module.demo.o module.version.o

mod_lib=	../libcgi_core_modules

all:
	make $(LIBRARYTYPE)

clean:
	rm -f *.o
	rm -f *.a
	rm -f *~

clr:
	make clean

shared:
	make $(mod_lib).so
	make static

static:
	make $(mod_lib).a

$(mod_lib).so: $(all_mod) modules_installer.o
	$(LD_SO) $@ $(all_mod) modules_installer.o
	chmod -x $@

$(mod_lib).a: $(all_mod) modules_installer.o
	$(ARC)   $@ $(all_mod) modules_installer.o

modules.h: ../modules.h ../parser.h
	touch modules.h

modules_installer.o: modules_installer.cpp
	$(CC) modules_installer.cpp

module.%.o: module.%.cpp
	$(CC) $<
