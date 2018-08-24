# SDL2 3D engine - see 3Dengine.doc for the real docs
#
#
Requirements:

Linux
gcc, g++, libcX-dev and libstdc++X-dev
SDL2 and SDL2-dev

I am using Debian 9.x amd64, and gcc-6 is standard there, so...

apt-get install gcc
apt-get install g++
apt-get install gcc-dev
apt-get install libc6-dev
apt-get install libstdc++-6-dev
apt-get install libstdc++-6-doc
apt-get install make
apt-get install libsdl2-gfx
apt-get install libsdl2-image
apt-get install libsdl2-mixer
apt-get install libsdl2-net
apt-get install libsdl2-ttf
apt-get install libsdl2-dev
apt-get install libsdl2-doc
apt-get install libsdl2-gfx-doc

I use 'elvis-tiny' (and alias 'vi' to it) as my cmdline editor, and SciTE as an editor in X.

Heh heh.  That will do for a start.

My core makefile is below the cutline.  I use it like this:

make dist  - make a distribution copy
make aback 3DVER=0.50x - make a backup copy and name it version 0.50x
make sdlprog - create the 'sdlprog' test binary

-------------------------------------------------------------------
# Galactic Explorer 3D engine 
#  --- working makefile
#
3DCURRVER = 0.501
SDLLIBS = -lSDL2 -lm

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
