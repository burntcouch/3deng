//
// vectoro.cpp
//
//
// method definitions for vector objects
//
#include <iostream>
#include "vectoro.h"

// method definitions

// constructors
DVector::DVector() {  // default is null vector
	cerr << "creating a null DVector\n";

	vx = 0.0;
	vy = 0.0;
	vz = 0.0;
}

DVector::DVector(char ax) {  // unit vectors, defaults to null vector
	cerr << "creating a " << ax << " DVector\n";

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
		default:  vx = 0.0; vy = 0.0; vz = 0.0; break;
	};
}

DVector::DVector(double x, double y, double z) {  // obvious?
	cerr << "creating a DVector\n";
	vx = x;
	vy = y;
	vz = z;
};

DVector::~DVector() {
	cerr << "destructing a DVector\n";
};

// definitions of basic vector functions

void DVector::set(double x, double y, double z) {
	this->vx = x;
	this->vy = y;
	this->vz = z;
}

char *DVector::show() {
	char *res;
	sprintf(res, "X: %7.2f - Y: %7.2f - Z: %7.2f", this->vx, this->vy, this->vz);
	return (res);
}

DVector DVector::add(DVector b) {
	DVector s;

	s.vx = this->vx + b.vx;
	s.vy = this->vy + b.vy;
	s.vz = this->vz + b.vz;
	return(s);
}

double DVector::mag() {
	return (sqrt(this->vx * this->vx + this->vy * this->vy + this->vz * this->vz));
}

DVector DVector::mult(double s) {
	DVector r;	
	
	r.vx = this->vx * s;
	r.vy = this->vy * s;
	r.vz = this->vz * s;
	return (r);
}

double DVector::dot(DVector bvect) {
	return (this->vx * bvect.vx + this->vy * bvect.vy + this->vz * bvect.vz);
}

DVector DVector::cross(DVector bvect) {
	DVector r;

  r.vx = this->vy * bvect.vz - this->vz * bvect.vy;
  r.vy = this->vz * bvect.vx - this->vx * bvect.vz;
  r.vz = this->vx * bvect.vy - this->vy * bvect.vx;
	return (r);
}

// end of vectoro.cpp
//
