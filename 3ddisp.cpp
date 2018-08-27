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

// protos?

class Env3D {};
class Disp3D {};

typedef Env3D *pEnv3D;
typedef Disp3D *pDisp3D;

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
		Objlist * lites;				// lights	list
		double lvc;		  				// lighting range scale: 5 typical
		double lvr;		  				// lighting range: 32 typical
		bool liteson;		  			// do lighting? if not, use Ghod shading
		int drawmode;		  			// 0 - wire frame, 1 - solid, 2 - shaded		

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
		double delta;		  // time scale
		int hScrn;	 	  // horizontal view res. (clip region)
		int vScrn;	 	  // vertical view res. (clip region)
		double vscal;		  // pixels per meter -- 1.875 * 2 * hScrn 

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

Disp3D::Disp3D() {
	winsize.x = 1024;
	winsize.y = 600;
	delta = 1.0;
	hScrn = ;
	vScrn = ;
	vscal = ;
	
	tscale = 1.0;
	frcnt = 0;
	frate = 0.0;
	fskip = 0;

	startTime = SDL_GetTicks();
	srand((Time(NULL) * startTime) / 997); 
}

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

int ZSCompS( const void * a, const void * b);  // current Z-sort uses this one
int ZSCompS2( const void * a, const void * b);
// qsort compare functions for z-sorts
//

int ZSCompS( const void * a, const void * b) {
   	long ia, ib;
	double fa, fb;
	int res;

	ia = *(long *)a;
	ib = *(long *)b;
	fa = ZSort3D[ia].z;
	fb = ZSort3D[ib].z;
	if ((fa - fb) > 0) res = 1;
	else res = - 1;
	if (fa == fb) res = 0; 
	return res;
}

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
