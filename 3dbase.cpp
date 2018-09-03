//
// 3dbase.cpp
//
// base objects, structs, and functions for rendering
//

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#ifndef __3DBASE
#define __3DBASE 1
#include "3dbase.h"

//
// XYCrd - convenience object for SDL rendering
//

//defs 
XYCrd::XYCrd() {
	x = 0; y = 0;
}

XYCrd::XYCrd(int ix, int iy) {
	x = ix;
	y = iy;
}

XYCrd::~XYCrd() {}

//
// *** Colora - convenience object to handle color in rendering
//

// defs
void Colora::fade(int howmuch = 10) {
	a = a - howmuch;
	if(a < 0) a = 0;
}

void Colora::bright(int howmuch = 10) {
	a = a + howmuch;
	if(a > 255) a = 255;			
}

Colora::Colora() {
	r = 255; g = 255; b = 255; a = 255;
}	

Colora::Colora(unsigned int ir, unsigned int ig, unsigned int ib, unsigned int ia) { 
	r = ir; g = ig; b = ib; a = ia;
}

Colora::~Colora() {}

// some basic colors

const Colora black(0, 0, 0, 255);
const Colora white(255, 255, 255, 255);
const Colora maxRed(255, 0, 0, 255);
const Colora maxGrn(0, 255, 0, 255);
const Colora maxBlu(0, 0, 255, 255);
const Colora maxYlw(255, 255, 0, 255);
const Colora yellow(150, 150, 0, 255);
const Colora ltGry(150, 150, 150, 255);
const Colora midGry(90, 90, 90, 255);
const Colora ltRed(100, 0, 0, 255);
const Colora ltGrn(0, 100, 0, 255);
const Colora ltBlu(0, 0, 100, 255);
const Colora dkYlw(40, 40, 0, 255);
const Colora dkGry(30, 30, 30, 255);
const Colora dkRed(50, 0, 0, 255);
const Colora dkGrn(0, 50, 0, 255);
const Colora dkBlu(0, 0, 50, 255);

void clear_to_Colora(SDL_Renderer * r, Colora c) {  // clear r to color c
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);  // set to black and full alpha
    SDL_RenderClear(r);  
}


//
// *** Trig3D - convenience object for SDL rendering
//

Trig3D::Trig3D() {
	for(int i = 0; i < 3; i++) pts[i] = XYCrd(0, 0);
}

Trig3D::Trig3D(XYCrd p1, XYCrd p2, XYCrd p3) {
	pts[0] = p1;
	pts[1] = p2;
	pts[2] = p3;
}

Trig3D::Trig3D(XYCrd p1) {
	pts[0] = p1;
	pts[1] = XYCrd(-9999, -9999);
	pts[2] = XYCrd(-9999, -9999);
}

Trig3D::~Trig3D() {}

void Trig3D::push(XYCrd apoint) {
	pts[2] = pts[1];
	pts[1] = pts[0];
	pts[0] = apoint;
}

void Trig3D::move(int x, int y) {
	for(int i = 0; i < 3; i++) {pts[i].x += x; pts[i].y += y;}
}

// a wrapper to draw this stuff in SDL

void draw_Trig3D(SDL_Renderer * r, Trig3D *trid) {
//
	filledTrigonRGBA(r, trid->pts[0].x, trid->pts[0].y, 
			trid->pts[1].x, trid->pts[1].y, 
			trid->pts[2].x, trid->pts[2].y,
			trid->color.r, trid->color.g, trid->color.b, trid->color.a);
//
}

bool Trig3D::pixel() { // is this just one pixel?
	return ( (pts[1].x == -9999 && pts[1].y == -9999 && pts[2].x == -9999 && pts[2].y == -9999) ? true : false );
}

void Trig3D::pixelize(int p) { // convert to one pixel
	if (p >= 0 && p < 3) pts[0] = pts[p];
	pts[1] = XYCrd(-9999, -9999);
	pts[2] = XYCrd(-9999, -9999);	
}

long Trig3D::area() {
	int y[3];   // indexes of sorted points
	double tempa = 0.0;

	y[0] = 0; // sort points by y-value
	if (pts[0].y > pts[1].y) { y[0] = 1; y[1] = 0; }
	else y[1] = 1;
	if (pts[2].y > pts[y[1]].y) y[2] = 2;
	else {
		if (pts[2].y < pts[y[0]].y) { y[2] = y[1]; y[1] = y[0]; y[0] = 2; }
		else { y[2] = y[1]; y[1] = 2; }
	}
		// find areas of two sub-triangles
	double invslope = (double) (pts[y[0]].x - pts[y[2]].x) / (pts[y[0]].y - pts[y[2]].y);
	double midwid = abs( pts[y[1]].x - ( pts[y[0]].x + invslope * ( pts[y[0]].y - pts[y[1]].y ) ) );
	tempa = abs(pts[y[0]].y - pts[y[1]].y) * midwid / 2;
	tempa += abs(pts[y[1]].y - pts[y[2]].y) * midwid / 2; 
	return = (long) *(tempa + 0.5);
}

#endif
// end of 3dbase.cpp
//
