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

		SDL_Renderer * render;
		SDL_Window * window;
		ObjList * olist;
		XYCrd wsize;

	protected:

};

Env3D::Env3D() {}
Env3D::~Env3D() {}

// definitions

//
//*** Disp3D
//


#endif
// end of 3ddisp.cpp
