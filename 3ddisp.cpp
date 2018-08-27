//
// 3ddisp.cpp
//
//
// display management object for SDL2 3D engine
//

#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#ifndef __DVECTOR
#include "vectoro.cpp"   // DVector object and functions
#endif

#ifndef __3DBASE
#include "3dbase.cpp"    // XYCrd, Colora, Trig3D objects
#endif

#ifndef __3DOBJ				 
#include "3dobj.cpp"		// Face3D, Seg3D, Vert3D, Obj3D, ObjList
#endif

#ifndef __3DDISP
#define __3DDISP 1

//
//*** Env3D
//
class Env3D {
	public:
		Env3D();
		~Env3D();

		DVector opos;						// observer position
		DVector ovel;						// observer velocity
		DVector oax;						// observer accel
		DVector oiv;						// obs. orientation frame. +i is 'forward'
		DVector ojv;						// +j is 'left'
		DVector okv;						// +k is 'up'

		ObjList * olist;				// object list
		Objlist * lites;				// lights			

	protected:

};

Env3D::Env3D() {}
Env3D::~Env3D() {}

// definitions

//
//*** Disp3D
//
class Disp3D {
	public:
		Disp3D();
		~Disp3D();

		XYCrd winsize;							// dimensions of window
		double drawscale;						// scale of screen
		SDL_Renderer * render;		// renderer struct
		SDL_Window * window;			// window struct

		XYCrd mouse;							// last mouse position
		int key;							// last key
		int kmod;							// last key modifier (shift, alt, etc.)

		int startTime;				// start ticks
		double tscale;				// time scale
		int frcnt;						// frame count
		double frate;					// latest frame rate
		int fskip;						// how many frames to skip between moves
	
	protected:

}

Disp3D::Disp3D() {}
~Disp3D::Disp3D() {}

// Z-sort array declaration and qsort compare function

struct _zsort3D;

typedef struct _zsort3D {
	double z;
	float zc;
	p3DFace zf;
} _ZS3D;

typedef _ZS3D *pZS3D;

pZS3D ZSort3D;
long *ZSX;

// qsort compare function prototypes for z-sorting

int ZSCompS( const void * a, const void * b);
// qsort compare functions for z-sorts
//

int ZSCompS2( const void * a, const void * b) {
   	_ZS3D ia, ib;
	double fa, fb;
	int res;

	ia = *(_ZS3D *)a;
	ib = *(_ZS3D *)b;
	fa = ia.z;
	fb = ib.z;
	if ((fa - fb) > 0) res = 1;
	else res = - 1;
	if (fa == fb) res = 0; 
	return res;
}


#endif
// end of 3ddisp.cpp
