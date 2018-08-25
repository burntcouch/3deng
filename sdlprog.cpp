//
// 3dsdl test program template
//

#include <math.h>
#include <float.h>
// gotcha #1 -- this is just 'iostream', NOT 'iostream.h' anymore
#include <iostream>
#include <SDL2/SDL.h>

#include "vectoro.cpp"   // to test c++ reworking of vectors.c to .cpp

// gotcha #2 -- add this line to keep from having to use 'std::cout' below instead of just 'cout'
using namespace std;

int main(void);

void vdump(char *caption, DVector *v);
DVector *vrand(void);

int DVector::debug = 0;  // set the debug flag  NOTICE THAT THIS IS OUTSIDE main() !!

int main(void) 
{
			// vectoro tests

	DVector v1, v4;
	DVector v2(1.0, 2.0, 3.0);
	DVector v3('x');
	DVector *pv5;

	double dv, vmag;
	char buf[100] = "\0";

	srand(time(NULL));  // yes, there are better ways to do this, but...

	v2.set(1.0, 2.0, 3.0);
	v1 = v2.add(v3);

	// try some on the generic vectors...
	v4 = DVector('r').add(DVector('z'));

	dv = v2.dot(v3);
	vmag = v1.mag();
	cout << dv << "\n" << vmag << "\n\n";
	vdump("V2      : ", &v2);
	vdump("V3      : ", &v3);
	vdump("V2 + V3 : ", &v1); 
	cout << "\nV2 . V3 : " << v2.dot(v3) << "\n";
	cout << "V1 mag  : " << v1.mag() << "\n";
	vdump("\narbitrary unit : ", &v4);
	cout << "\n\n";
	
	pv5 = vrand();
	vdump("V5 random: ", pv5);
	cout << "\n\n";

	// rotation tests
	for (double theta = 0.0; theta < 360.0; theta += 30.0) {
		char zstr[30];
		DVector *pz = new DVector();

		*pz = pv5->rot('x', theta);
		sprintf(zstr, "Rot %4.1f deg abt. x of:", theta);
		vdump(zstr, pz);
	}
	for (double theta = 0.0; theta < 360.0; theta += 30.0) {
		char zstr[30];
		DVector *pz = new DVector();

		*pz = pv5->rot('y', theta);
		sprintf(zstr, "Rot %4.1f deg abt. y of:", theta);
		vdump(zstr, pz);
	}
	for (double theta = 0.0; theta < 360.0; theta += 30.0) {
		char zstr[30];
		DVector *pz = new DVector();

		*pz = pv5->rot('z', theta);
		sprintf(zstr, "Rot %4.1f deg abt. z of:", theta);
		vdump(zstr, pz);
	}

	return 0;
}

DVector *vrand(void) {
	double x, y, z;
	DVector *pv;

	x = (((double) rand()) / RAND_MAX - 0.5);
	y = (((double) rand()) / RAND_MAX - 0.5);
	z = (((double) rand()) / RAND_MAX - 0.5);

	pv = new DVector(x, y , z);
	*pv = pv->unit();

	return pv;
}

void vdump(char *caption, DVector *v) {
	char buf[100] = "\0";
	
	v->show(buf);
	cout << caption << buf << "\n";
}

