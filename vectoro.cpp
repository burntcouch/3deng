//
// vectoro.cpp
//
//
// method definitions for vector objects
//
#include <iostream>

#ifndef __DVECTOR
#define __DVECTOR 1
#include "vectoro.h"

// method definitions

// constructors
DVector::DVector() {  // default is null vector
	if (debug) cerr << "creating a null DVector\n";

	vx = 0.0;
	vy = 0.0;
	vz = 0.0;
}

DVector::DVector(char ax) {  // unit vectors, defaults to null vector

	if (debug) {
		char sax[2] = " ";
		sax[0] = ax;
		cerr << "creating a '" << sax << "' DVector\n";
	}

	switch(ax) {
		case 'x':
		case 'X':
		case 'I':
		case 'i': vx = 1.0; vy = 0.0; vz = 0.0; break;
		case 'y':
		case 'Y':
		case 'J':
		case 'j': vx = 0.0; vy = 1.0; vz = 0.0; break;
		case 'z':
		case 'Z':
		case 'K':
		case 'k': vx = 0.0; vy = 0.0; vz = 1.0; break;
		case 'r':
		case 'R':
				vx = (((double) rand()) / RAND_MAX - 0.5);
				vy = (((double) rand()) / RAND_MAX - 0.5);
				vz = (((double) rand()) / RAND_MAX - 0.5);
				*this = this->unit();
				break;
		default:  vx = 0.0; vy = 0.0; vz = 0.0; break;
	};
}

DVector::DVector(double x, double y, double z) {  // obvious?
	if (debug) cerr << "creating a DVector\n";
	vx = x;
	vy = y;
	vz = z;
};

DVector::~DVector() {
	if (debug) cerr << "destructing a DVector\n";
};

// definitions of basic vector functions

void DVector::set(double x, double y, double z) {
	this->vx = x;
	this->vy = y;
	this->vz = z;
}

void DVector::show(char *res) {
	snprintf(res, 80, "X:[%7.2f] Y:[%7.2f] Z:[%7.2f]", vx, vy, vz);
}

DVector DVector::add(DVector b) {
	DVector s;

	s.vx = this->vx + b.vx;
	s.vy = this->vy + b.vy;
	s.vz = this->vz + b.vz;
	return(s);
}

DVector DVector::diff(DVector b) {
	DVector s;

	s.vx = this->vx - b.vx;
	s.vy = this->vy - b.vy;
	s.vz = this->vz - b.vz;
	return(s);
}

double DVector::mag() {
	return (sqrt(vx * vx + vy * vy + vz * vz));
}

DVector DVector::mult(double s) {
	DVector r;	
	
	r.vx = this->vx * s;
	r.vy = this->vy * s;
	r.vz = this->vz * s;
	return (r);
}

DVector DVector::unit() {
	double m;
	m = this->mag();
	return (this->mult(1.0 / m));
}

double DVector::dot(DVector b) {
	double res = 0.0;
	res = vx * b.vx;
	res += vy * b.vy;
	res += vz * b.vz;
	return (res);
}

DVector DVector::cross(DVector b) {
	DVector r;

  r.vx = this->vy * b.vz - this->vz * b.vy;
  r.vy = this->vz * b.vx - this->vx * b.vz;
  r.vz = this->vx * b.vy - this->vy * b.vx;
	return (r);
}


// now for more involved stuff...

// A non-quadrant ambiguous arc-cosine function.
// A and B vectors define angle and C-vect is the vector
// defining the "motion reference" for A and B.
// Be sure NOT to get A & B backwards!  12/03/02

double DV_acos(DVector avect, DVector bvect, DVector cvect)
{
	double theCos;
	DVector theVec;
	
	theCos = (avect.dot(bvect) / (avect.mag() * bvect.mag()));
	theCos = acos(theCos);
	theVec = avect.cross(bvect);
	if (theVec.dot(cvect) < 0.0) theCos = 2 * PI - theCos;
  
	return (theCos);
}


DVector DV_norm(DVector v1, DVector v2, DVector v3) {
	DVector tv1, tv2, resv;
	
	tv1 = v1.add(v2.mult(-1.0));
	tv2 = v2.add(v3.mult(-1.0));
	resv = tv1.cross(tv2);
	resv = resv.mult(1.0 / resv.mag());
	return resv;
}

DVector DV_center(DVector v1, DVector v2, DVector v3) {
	DVector tmpv;
	
	tmpv = v1.add(v2);
	tmpv = tmpv.add(v3);
	tmpv = tmpv.mult(1 / 3.0);
	return tmpv;
}

DVector DV_midpt(DVector v1, DVector v2, DVector ctr) {
	DVector tmp1, tmp2, resv;
	double rad;

	tmp1 = (v1.add(v2)).mult(0.5);
	tmp2 = (ctr.diff(tmp1)).unit();
	rad = ((v1.diff(ctr)).mag() + (v2.diff(ctr)).mag()) * 0.5;
	resv = ctr.add(tmp2.mult(rad));
	return resv;
}

// rotation methods

DVector DVector::rot(char axis, double frot){

   DVector tempvect;
   double flrot;
   double rotax[3][3]= {{1,0,0},{0,1,0},{0,0,1}};
   
   flrot = frot * PI / 180.0;
   switch (axis)
   {
      case 'x':
         rotax[1][1]=cos(flrot);
         rotax[2][1]=-sin(flrot);
         rotax[1][2]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'y':
         rotax[0][0]=cos(flrot); 
         rotax[0][2]=-sin(flrot);
         rotax[2][0]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'z':
         rotax[0][0]=cos(flrot); 
         rotax[1][0]=-sin(flrot);
         rotax[0][1]=sin(flrot); 
         rotax[1][1]=cos(flrot);
         break;
      default:
	break;
   }

   tempvect.vx =(vx*rotax[0][0] +	vy*rotax[1][0] + vz*rotax[2][0]);
   tempvect.vy =(vx*rotax[0][1] + vy*rotax[1][1] + vz*rotax[2][1]);
   tempvect.vz =(vx*rotax[0][2] + vy*rotax[1][2] + vz*rotax[2][2]);
   return tempvect;
}

DVector DVector::rot3D(DVector irv, DVector jrv, double rota) {
	char rax = 'x';
	DVector krv, rv1, rv2;

	krv = irv.cross(jrv);
	rv1.vx = this->dot(irv);
	rv1.vy = this->dot(jrv);
	rv1.vz = this->dot(krv);
	rv1 = rv1.rot(rax, rota);
	rv2.vx = rv1.vx*irv.vx + rv1.vy*jrv.vx + rv1.vz*krv.vx;
	rv2.vy = rv1.vx*irv.vy + rv1.vy*jrv.vy + rv1.vz*krv.vy;
	rv2.vz = rv1.vx*irv.vz + rv1.vy*jrv.vz + rv1.vz*krv.vz;
	return rv2;
}

#endif
// end of vectoro.cpp
//
