# Galactic Explorer 3D engine 
#  --- working makefile
#
3DCURRVER = 0.47
VGADEMOPATH = /home/patstr/vgademos
DATAPATH = /home/patstr/galex/data

ifdef 3DVER
	3DCURRVER := $(3DVER)
endif

touchall: *.c
	touch *.c

dist: *.c *.doc makefile
	tar czf ../distarch/gx3DE.$(3DCURRVER).tar.gz \
	*.c *.doc makefile README* $(VGADEMOPATH)/*test \
	$(DATAPATH)/*.mrsc $(DATAPATH)/*.hd

aback: *.c *.doc makefile
	mkdir ../v$(3DCURRVER); \
	mkdir ../v$(3DCURRVER)/back; \
	cp *.c ../v$(3DCURRVER); \
	cp *.doc ../v$(3DCURRVER); \
	cp README* ../v$(3DCURRVER); \
	cp makefile ../v$(3DCURRVER)

cback: *.c *.doc makefile
	tar czf back/gxbackup$(3DCURRVER).tar.gz *.c *.doc makefile README*

navdtest: navdtest.o
	gcc -o navdtest navdtest.o -lm 

navdtest.o: navdtest.c
	gcc -O -c navdtest.c 

solex4: solex4.o 
	gcc -o solex4 solex4.o -lvgagl -lvga -lm

solex4.o: solex4.c
	gcc -O3 -c solex4.c

solex41: solex41.o 
	gcc -o solex41 solex41.o -lvgagl -lvga -lm

solex41.o: solex41.c
	gcc -O3 -c solex41.c

solex42: solex42.o 
	gcc -o solex42 solex42.o -lvgagl -lvga -lm

solex42.o: solex42.c
	gcc -O3 -c solex42.c

solext: solext.o 
	gcc -o solext solext.o -lvgagl -lvga -lm

solext.o: solext.c
	gcc -O3 -c solext.c

waveread: waveread.o 
	gcc -o waveread waveread.o -lvgagl -lvga -lm

waveread.o: waveread.c
	gcc -O3 -c waveread.c

solex49: solex49.o 
	gcc -o solex49 solex49.o -lvgagl -lvga -lm

solex49.o: solex49.c
	gcc -O3 -c solex49.c

solex5: solex5.o 
	gcc -o solex5 solex5.o -lvgagl -lvga -lm

solex5.o: solex5.c
	gcc -O3 -c solex5.c


