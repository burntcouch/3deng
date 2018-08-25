# Galactic Explorer 3D engine 
#  --- working makefile
#
3DCURRVER = 0.501
SDLLIBS = -lSDL2 -lSDL2_gfx -lm

ifdef 3DVER
	3DCURRVER := $(3DVER)
endif

touchall: *.c *.cpp *.h
	touch *.c; \
	touch *.cpp; \
	touch *.h

dist: *.c *.cpp *.h *.doc README* makefile
	tar czf ./distarch/3DEsdl.$(3DCURRVER).tar.gz \
	*.c *.cpp *.h *.doc makefile README*

aback: *.c *.cpp *.h *.doc README* makefile
	mkdir ./v$(3DCURRVER); \
	mkdir ./v$(3DCURRVER)/back; \
	cp *.c ./v$(3DCURRVER); \
	cp *.cpp ./v$(3DCURRVER); \
	cp *.h ./v$(3DCURRVER); \
	cp *.doc ./v$(3DCURRVER); \
	cp README* ./v$(3DCURRVER); \
	cp makefile ./v$(3DCURRVER)

sdlprog: sdlprog.cpp
	g++ sdlprog.cpp -w $(SDLLIBS) -o sdlprog
	
solex42: solex42.o 
	gcc -o solex42 solex42.o -lvgagl -lvga -lm

solex42.o: solex42.c
	gcc -O3 -c solex42.c

