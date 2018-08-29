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

Trig3D::~Trig3D() {}

void Trig3D::push(XYCrd * apoint) {
	pts[2] = pts[1];
	pts[1] = pts[0];
	pts[0] = *apoint;
}

void Trig3D::move(int x, int y) {
	for(int i = 0; i < 3; i++) {pts[i].x += x; pts[i].y += y;}
}

// a wrapper to draw this stuff easier

void draw_Trig3D(SDL_Renderer * r, Trig3D *trid) {

	filledTrigonRGBA(r, trid->pts[0].x, trid->pts[0].y, 
			trid->pts[1].x, trid->pts[1].y, 
			trid->pts[2].x, trid->pts[2].y,
			trid->color.r, trid->color.g, trid->color.b, trid->color.a);
}

// qsort compare functions for z-sorts (protos and structs in 3dbase.h)
//
//  notice that this DEPENDS on the declaration of "ZS3D *Zarr;" in the header file
//
int ZSCompS(const void * a, const void * b) {
  long ia, ib;
	double fa, fb;
	int res;

	ia = *(long *)a;
	ib = *(long *)b;
	fa = Zarr[ia].z;
	fb = Zarr[ia].z;
	if ((fa - fb) > 0) res = 1;
	else res = - 1;
	if (fa == fb) res = 0; 
	return res;
}


#endif
// end of 3dbase.cpp
//
