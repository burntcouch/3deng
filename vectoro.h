//
// vectoro.h
//
// class-based vector library
// (Pat Struthers)
// 8/23/18 - the basics
//

#include <iostream>
#include <float.h>
#include <string.h>

#ifndef __DVECTOR
#define __DVECTOR 1

#ifndef PI
#define PI 3.1415926535898
#endif

using namespace std;

class DVector {
	public:
			// protos for constructor and destructor
		DVector();  // initialize to null vector
		DVector(char ax);  // initialize as a unit vector (x/i, y/j, z/k, default null)
		DVector(double x, double y, double z);  // initialize to x,y,z
		~DVector();
			// protos for basic functions
		void set(double x, double y, double z);	// set
		char *show();											// display
		DVector add(DVector b);					// vector addition
		double mag();											// magnitude
		DVector mult(double s);					// scalar multiply		
		double dot(DVector b);						// dot product
		DVector cross(DVector b);				// cross product
	protected:
		double vx, vy, vz;
};

#endif
// end of vectoro.h
//
