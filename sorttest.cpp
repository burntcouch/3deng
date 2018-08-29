//
// quicksort test
//


#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define ZASIZE 25

class ZD;
typedef ZD pZD;

class Stuff;
typedef Stuff pStuff;

class ZD {
	public:
		ZD();
		~ZD();

		double z;
		void *ix;
};

ZD::ZD() { z = 0.0; ix = NULL;}
ZD::~ZD() {}

class Stuff {
	public:
		Stuff();
		~Stuff();

		double z;
		double d;
};

Stuff::Stuff() { d = (double) (rand() / RAND_MAX); }
Stuff::~Stuff() {}


ZD *zarr;

int main(void);
int ZComp(const void *a, const void *b);

int ZComp(const void *a, const void *b) {
	long ia, ib;
	double fa, fb;
	int res;

	ia = *(long *)a;
	ib = *(long *)b;
	fa = zarr[ia].z;
	fb = zarr[ib].z;
	if ((fa - fb) > 0) res = 1;
	else res = - 1;
	if (fa == fb) res = 0; 
	return res;
}

int main(void) {
	long *ZX;
	int howmany = 20;

	Stuff *s;

	ZX = new long[howmany];
	zarr = new ZD[howmany];
	s = new Stuff[howmany];

	for(long i = 0; i < howmany; i++) {
		s[i].z = (double) (1000.0 * rand() / RAND_MAX);
		s[i].d =  ((double) rand() / RAND_MAX);
		zarr[i].z = s[i].z;
		zarr[i].ix = &s[i];

		ZX[i] = i;
		std::cout << "i: " << i << "  Z: " << zarr[i].z << " Stuff: " << s[i].d << "\n";
	}

	std::cout << "\n\n\n";

	qsort(ZX, howmany, sizeof(long), ZComp);

	for(long i = 0; i < howmany; i++) {
		Stuff *somestuff = (Stuff *) zarr[ZX[i]].ix;

		printf( "i:%i  newi: %i  ZD: %5.1f newZD: %5.1f  Stuff: %5.4f\n", i, ZX[i], zarr[i].z, 
			zarr[ZX[i]].z, somestuff->d );
	}

	// clean up
	delete[] s;
	delete[] zarr;
	delete[] ZX;

	return 0;
}
