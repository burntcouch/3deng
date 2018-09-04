//
// 3ddisp.cpp
//
//
// display management object for SDL2 3D engine
//

#include <math.h>
#include <iostream>
#include <vector>  // for resizeable arrays in the z-sort stuff
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

Env3D::Env3D() {
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
Env3D::~Env3D() {}

// end of Env3D

// Z-sort array declaration and qsort compare function


class ZS3D;
typedef ZS3D pZS3D;

class ZS3D {   // what we are sorting
	public:
		ZS3D();
		~ZS3D(); 
	
		double z;
		pFace3D zf;
		double zc;		
};

ZS3D::ZS3D() {}
ZS3D::~ZS3D() {}

// face to screen coord. conversion status
//
V2Screen::V2Screen() {
	face = NULL;
	trig = NULL;
	area = 0;
	behind = 0.0;
	toward = 0.0;
	offscrn = 0;
}

V2Screen::~V2Screen() {}

// qsort compare functions for z-sorts (protos and structs in 3dbase.h)
//  dependent on 
//
int ZSCompS(const void * a, const void * b) {
  double fa, fb;

	fa = (*(ZS3D *)a)->z;
	fb = (*(ZS3D *)b)->z;
	return ((fa - fb) > 0) ? 1 : ((fa == fb) ? 0 : -1);
}

// vector to coordinate math
int vec_to_screen(pEnv3D wenv, DVector v, pXYCrd xy) {
	double alim = 3.0 * PI / 4.0;
	int offs = 0;
	int slack = 20;
	DVector tmpv = v.diff(wenv->opos);
	double pr = tmpv.mag();
	double ia = wenv->oiv.dot(tmpv);
	double ja = wenv->ojv.dot(tmpv);
	double ka = wenv->okv.dot(tmpv);
	double pt = atan(fabs(ja/ia));

	if (ia < 0) pt = PI - pt;
	if (ja < 0) pt = -pt;
	double pp = asin(ka/pr);
		// angle limiter for horiz.
	if (pt > alim) pt = alim;
	if (pt < -alim) pt = -alim;
	double fx = -wenv->vscal * pt;
	double fy = wenv->vscal * pp;
	int offs = ((fabs(fx) > wenv->hScrn + slack) && (fabs(fy) > wenv->vScrn + slack)) ? true : false;
	xy->x = (int) fx +  wenv->hScrn;
	xy->y = wenv->vScrn - (int) fy;

	return offs;
}


// face to trigon math
pV2Screen vec3D_to_screen(pEnv3D wenv, pFace3D f) {
	pV2Screen res = new V2Screen;
	pXYCrd xy = new XYCrd[3];
	DVector zv, cv, nv;
	int offscrn = 0;
	double tempa = 0.0;

	zv = (face->center()).diff(wenv->opos);
	cv = zv.unit();
	nv = face->norm();
	res->toward = cv.dot(nv); //toward < 0, norm of face is 'toward' observer
	res->behind = zv.dot(wenv->oiv);  // behind > 0, face is 'ahead' of observer

	res->face = f;
	for (int i = 0; i < 3; i++) {
		offscrn = vec_to_screen(wenv, f->v[0], &xy[0]);
		offscrn += vec_to_screen(wenv, f->v[1], &xy[1]);
		offscrn += vec_to_screen(wenv, f->v[2], &xy[2]);
	}
	res->trig = new Trig3D(xy[0], xy[1], xy[2]);
	res->area = res->trig.area();
	res->offscrn = offscrn;

	return res;
} 

// Z-sort routine

long draw_Zsort(pEnv3D wenv) {
	// allocate stuff for z-sorts
	pV2Screen scrn_tg;

	long zcnt, zi = 0;
	pObj3D objlink;
	std::vector<pZS3D> ZX;

	zcnt = ZSTART;
	ZX.resize(zcnt);

	// find out what faces need to be rendered and sorted

	objlink = wenv->head;  // start at the top
	while (objlink) {
		switch(objlink->df) {   // is this a degenerate object?
			case 1: { // not degenerate
			  face = objlink->fhead;
			  while (face != NULL) {
					scrn_tg = vec3D_to_screen(wenv, face);
					if (scrn_tg->toward < 0 && scrn_tg->behind > 0 && !scrn_tg->offscrn) {    // face is toward view.  
				    double d3 = sqrt(-(scrn_tg->toward));
				    if (scrn_tg->area < 4) d3 += 1.0;
						ZX[zi] = new ZS3D;
						ZX[zi]->tg = scrn_tg;  // save the coordinates
						ZX[zi]->z = scrn_tg->behind;
						ZX[zi]->zc = d3;  // acos of angle between LOS to observer and normal of face, I think
						ZX[zi]->zf = face;
						zi++;
					}
					face = face->next;
				}	
			  break;
			case 2: {   // degenerate case
				DVector zv = (objlink->center).diff(wenv->opos);
				double d2 = zv.dot(wenv->oiv);
				if (d2 > 0) {  // in front of viewer, so...
					xy = new XYCrd;
					bool vc = vec_to_screen(wenv, zv, &xy);
					if (!vc) {   					// as long as it is on screen
						ZX[zi] = new ZS3D;
						ZX[zi]->tg = new Trig3D(xy);  // save the coordinates
						ZX[zi]->z = d2;
						ZX[zi]->zc = 1.0;        // assumes that face is fully illuminated
						ZX[zi]->zf = objlink->fhead;
						zi++;
					}
					face = face->next;					
				}
			}
			case 0:   // disabled face, don't draw
			default: break;
		}  // end of switch
		// check to see if we need to resize yet
		if (zi - zcnt < 5) {
			zcnt += zcnt + ZGROW;
			ZX.resize(zcnt);
		}
		objlink = objlink->next;
	} // end of while

	// and by-em-by we will do...
	qsort(ZX.data(), zi, sizeof(pZS3D), ZSCompS);

	for (long fi = zi-1; fi > -1; fi--) {   // draw 'em backwards
		Face3D *drawface = ZX[fi]->zf;
		
		objlink = drawface->obj;
		bool pflag = false;   // 'pixel flag'; if drawn face is small enough, just draw a pixel

		// TO DO:  lighting calcs and drawing
		//  three cases: degerate large, degerate pixel || face pixel, or regular face
	
	}

	return zi;
}


#endif
// end of 3ddisp.cpp
//
