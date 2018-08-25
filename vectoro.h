//
// vectoro.h
//
//  see definitions in vectoro.cpp
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
		static int debug;  // well, self-explanatory?

			// protos for constructor and destructor
		DVector();  // initialize to null vector
		DVector(char ax);  // initialize as a unit vector (x/i, y/j, z/k, default null)
		DVector(double x, double y, double z);  // initialize to x,y,z
		~DVector();
			// protos for basic functions
		void set(double x, double y, double z);	// set
		void show(char *res);											// display
		DVector add(DVector b);					// vector addition
		DVector diff(DVector b);				// 'subtraction' (difference)
		DVector mult(double s);					// scalar multiply	
		double mag();										// magnitude
		DVector unit();									// unit vector
		double dot(DVector b);					// dot product
		DVector cross(DVector b);				// cross product
			//rotation
		DVector rot(char axis, double frot);  // about one of xyz axes
		DVector rot3D(DVector irv, DVector jrv, double rota);  // around 'i' axis of arbitrary ijk system


	protected:
		double vx, vy, vz;
};

// external vector functions
double DV_acos(DVector avect, DVector bvect, DVector cvect); //non-ambiguous arccosine function, avect -> bvect
																														 //  cvect defines 'side' that angle rotates thru from a to b
DVector DV_norm(DVector v1, DVector v2, DVector v3);   // normal based on three vertices of a face
DVector DV_center(DVector v1, DVector v2, DVector v3); // center ('average') of three vertices of a face


#endif
// end of vectoro.h
//
