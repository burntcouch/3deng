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

XYCrd vec3D_to_screen(pEnv3D wenv, DVector v) {   // FLESH THIS OUT

}


// Z-sort routine

int draw_Zsort(pEnv3D wenv) {
	// allocate stuff for z-sorts
	int res;

	long zcnt, zi = 0;
	long *ZX;

	pObj3D objlink;
	DVector zv, cv, nv;
	double d1, d2, d3;

	zcnt = wenv->zcnt();     // NEED TO WRITE THIS function...and need to start using <vector> apparently...
	Zarr = new ZS3D[zcnt];
	ZX = new long[zcnt];

	// find out what faces need to be rendered and sorted

	objlink = wenv->head;  // start at the top
	while (objlink) {
		switch(objlink->df) {   // is this a degenerate object?
			case 1: { // not degenerate
			  face = objlink->fhead;
			  while (face != NULL) {
					zv = (face->center()).diff(wenv->opos);
					cv = zv.unit();
					nv = face->norm();

					d1 = cv.dot(nv);
					if (d1 < 0) {    // face is toward view.  
													 // check if it is okay to render the back of the face here, too, someday...
						d2 = zv.dot(wenv->oiv);
						if (d2 > 0) {  // center of face is 'ahead' of viewer, so...
							XYCrd vc1 = vec3D_to_screen(wenv, face->v[0].v);
							svf =  vc1.offscrn ? 1 : 0;
							XYCrd vc2 = vec3D_to_screen(wenv, face->v[1].v);   // 'flag' is set if XYCrd is off the screen
							svf += vc2.offscrn ? 1 : 0;
							XYCrd vc3 = vec3D_to_screen(wenv, face->v[2].v);
							svf += vc3.offscrn ? 1 : 0;
							  // check size of polygon..
							  // if vc4 x+y < 2 is just a pixel
							XYCrd vc4;
							vc4.x = abs(vc1.x - vc2.x) + abs(vc2.x - vc3.x);
							vc4.y = abs(vc1.y - vc2.y) + abs(vc2.y - vc3.y);

							if (svf != 3) {
						    d3 = sqrt(-d1);
						    if ((vc4.x + vc4.y) < 2) d3 += 1.0;
								Zarr[zi].z = d2;
								Zarr[zi].zc = d3;  // this is ???
								Zarr[zi].zf = face;
								ZX[zi] = zi;
						
								zi++;
							}
						}
					}
					face = face->next;
				}	
			  break;
			case 2: {   // degenerate?		
				zv = (objlink->center).diff(wenv->opos);
				d2 = zv.dot(wenv->oiv);
				if (d2 > 0) {  // in front of viewer, that is
				if (ft2 > 0) {
					vc1 = Screen3Dr(wenv, zv);  // STILL hafta find out what this is...
					if (!vc1.flag) {   					// still don't know
						Zarr[zi].z = d2;
					  Zarr[zi].zc = 1.0;        // and this?
					  Zarr[zi].zf = objlink->fhead;
						ZX[zi] = zi;
						zi++;
					}
					face = face->next;					
				}
			}
			case 0:
			default: break;
		}  // end of switch
		objlink = objlink->next;
	} // end of while

	// and by-em-by we will do...
	qsort(ZX, zi, sizeof(long), ZSCompS);

	for (long fi = zi-1; fi > -1; fi--) {   // draw 'em backwards
		Face3D *drawface = (Face3D *) Zarr[ZSX[fi]].zf;
		
		objlink = drawface->obj;
		pflag = 0;   // 'pixel flag'; if drawn face is small enough, just draw a pixel

		
	
	}

	delete[] Zarr;   // do we want to cache some data?
	delete[] ZX;

	return res;
}


#endif
// end of 3ddisp.cpp
//
