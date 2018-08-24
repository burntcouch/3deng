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

int main(void) 
{
			// vector tests

	DVector v1, v4;
	DVector v2(1.0, 2.0, 3.0);
	DVector v3('x');

	double dv, vmag;
	char buf[100] = "\0";

	v2.set(1.0, 2.0, 3.0);
	v1 = v2.add(v3);

	// try some on the generic vectors...
	v4 = DVector('y').add(DVector('z'));

	dv = v2.dot(v3);
	vmag = v1.mag();
	cout << dv << "\n" << vmag << "\n\n";
	v2.show(buf);
	cout << "V2      : " << buf << "\n\n";
	v3.show(buf);
	cout << "V3      : " << buf << "\n\n";
	v1.show(buf);
	cout << "V2 + V3 : " << buf << "\n\n"; 
	cout << "V2 . V3 : " << v2.dot(v3) << "\n\n";
	cout << "V1 mag  : " << v1.mag() << "\n\n";
	v4.show(buf); 
	cout << "arbitrary unit : " << buf << "\n\n";
	return 0;
}
