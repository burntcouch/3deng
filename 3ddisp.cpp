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

#include "3ddisp.h"   // header type thingies...

//
//*** Env3D
//

Env3D::Env3D() {}
Env3D::~Env3D() {}

// definitions

//
//*** Disp3D
//

Disp3D::Disp3D() {
	winsize.x = 1024;
	winsize.y = 600;
	delta = 1.0;
	hScrn = winsize.x / 2;
	vScrn = winsize.y / 2;
	vscal = 1.875 * 2 * (double) hScrn;
	
	tscale = 1.0;
	frcnt = 0;
	frate = 0.0;
	fskip = 4;

	startTime = SDL_GetTicks();
	srand((time(NULL) * startTime) / 997); 
}

Disp3D::~Disp3D() {}

#endif
// end of 3ddisp.cpp
//
