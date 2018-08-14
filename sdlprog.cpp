//
// 3dsdl test program template
//

#include <math.h>
#include <float.h>
// gotcha #1 -- this is just 'iostream', NOT 'iostream.h' anymore
#include <iostream>
#include <SDL2/SDL.h>

#include "vectors.cpp"   // to test c++ reworking of vectors.c to .cpp

// gotcha #2 -- add this line to keep from having to use 'std::cout' below instead of just 'cout'
using namespace std;

int main(void);

int main(void) 
{
			// vector tests

	dVECTOR v1, v2, v3;
	double vmag;

	v2.vx = 1.0; v2.vy = 2.0; v2.vz = 3.0;
	v3.vx = -0.5; v3.vy = 1.5; v3.vz = -2.5;

	v1 = vAdd(v2, v3);
	vmag = vMag(v1);

	cout << "V2     : " << v2.vx << " " << v2.vy << " " << v2.vz << "\n\n";
	cout << "V3     : " << v3.vx << " " << v3.vy << " " << v3.vz << "\n\n";
	cout << "V2 + V3: " << v1.vx << " " << v1.vy << " " << v1.vz << "\n\n";
	cout << "vmag = " << vmag << "\n\n";
	return 0;
}
