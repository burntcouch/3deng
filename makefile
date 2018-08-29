# Galactic Explorer 3D engine 
#  --- working makefile
#
3DCURRVER = 0.505
SDL3DVER = 03
SDLLIBS = -lSDL2 -lSDL2_gfx -lm

ifdef 3DVER
	3DCURRVER := $(3DVER)
endif

touchall: *.c *.cpp *.h
	touch *.c; \
	touch *.cpp; \
	touch *.h

sdlbu: sdlprog.cpp sdl3d.cpp sdlprog sdl3d
	cp sdl3d.cpp ./test/sdl3d$(SDL3DVER).cpp; \
	cp sdl3d ./test/sdl3d$(SDL3DVER); \
	cp sdlprog.cpp ./test/sdlprog$(SDL3DVER).cpp; \
	cp sdlprog ./test/sdlprog$(SDL3DVER) 

dist: *.c *.cpp *.h *.rtf README* LICENSE* makefile
	tar czf ./distarch/3DEsdl.$(3DCURRVER).tar.gz \
	*.c *.cpp *.h *.rtf makefile README* LICENSE*

aback: *.c *.cpp *.h *.rtf README* makefile
	mkdir ./v$(3DCURRVER); \
	mkdir ./v$(3DCURRVER)/back; \
	cp *.c ./v$(3DCURRVER); \
	cp *.cpp ./v$(3DCURRVER); \
	cp *.h ./v$(3DCURRVER); \
	cp *.rtf ./v$(3DCURRVER); \
	cp README* ./v$(3DCURRVER); \
	cp LICENSE* ./v$(3DCURRVER); \
	cp makefile ./v$(3DCURRVER)

sdlprog: sdlprog.cpp
	g++ sdlprog.cpp -w $(SDLLIBS) -o sdlprog
	
sdl3d: sdl3d.cpp
	g++ sdl3d.cpp -w $(SDLLIBS) -o sdl3d

solex42: solex42.o 
	gcc -o solex42 solex42.o -lvgagl -lvga -lm

solex42.o: solex42.c
	gcc -O3 -c solex42.c

