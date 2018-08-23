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

	DVector v1, v2, v3;

	v2.set(1.0, 2.0, 3.0);
	v3.set(-0.5, 1.5, -2.5);

	v1 = v2.add(v3);

	cout << "V2      : " << v2.show() << "\n\n";
	cout << "V3      : " << v3.show() << "\n\n";
	cout << "V2 + V3 : " << v1.show() << "\n\n";
	cout << "V1 mag  : " << v1.mag() << "\n\n";
	return 0;
}
