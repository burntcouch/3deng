//
// quicksort test
//

#include <iostream>
#include <vector>

#define ZASIZE 25

class Stuff;
typedef Stuff *pStuff;

class Stuff {
	public:
		Stuff();
		~Stuff();

		double z;
		double d;
};

Stuff::Stuff() { d = (double) (rand() / RAND_MAX); }
Stuff::~Stuff() {}


int main(void);
int ZComp(const void *a, const void *b);

int ZComp(const void *a, const void *b) {
	double fa, fb;
	int res;

	fa = (*(pStuff *)a)->z;
	fb = (*(pStuff *)b)->z;
	res = ((fa - fb) > 0) ? 1 : ((fa == fb) ? 0 : -1);

	return res;
}

int main(void) {
	int howmany = ZASIZE;

	std::vector<pStuff> s(howmany);

	std::cout << "\nBefore Sort\n";
	for(long i = 0; i < howmany; i++) {
		s[i] = new Stuff;  									// or could just point at some existing Stuff
		s[i]->z = (double) (1000.0 * rand() / RAND_MAX);  // pretend this is the distance
		s[i]->d =  ((double) rand() / RAND_MAX);  // this is data carreied along with it...

		std::cout << "i: " << i << "  Stuff: d:" << s[i]->d << " z:" << s[i]->z << "\n";
	}

	qsort(s.data(), howmany, sizeof(pStuff), ZComp);  // do it, do it...

	std::cout << "\n\nAfter Sort\n";
	for(long i = 0; i < howmany; i++) {
		std::cout << "i: " << i << "  Stuff: d:" << s[i]->d << " z:" << s[i]->z << "\n";
	}

	return 0;
}
