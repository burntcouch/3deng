
//
// Pat's 3d data structure definitions 
//
//  3DDATA.C - v0.47 (3/27/06)
//
//  Originally in 3ddefs.c v0.3 (1/13/01)
//
// Started 12/21/00
//
// Remove some stuff to vectors.c, added some
// utility functions for object dumping and moving 12/27/00
// 1/3/2001 -- Shading now works...  rgb colors... 
//	    -- new spherical coords. based screen function
// 1/13/2001 - Start on lighting effects
// 1/16/2001 - Added display structure
// 1/17/2001 - Double precision conversion	
// 1/18/2001 - Pointered structures with dynamic memory allocation
// 1/19/2001 - And now it works...  More modularizing today!
// 3/5/2002 - Starting on degenerate objects, multiple dist scales,
//	changing levels of detail, dynamic object management,
//	real orbits
// 5/6/2003 - Rewrite of engine using all dynamic memory and linked lists
// 3/27/2006 - refamiliarizing and starting new work
//


#ifndef _3DDATA
#define _3DDATA 1

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vga.h>
#include <vgagl.h>

#ifndef _VECTORS
#include "mylib/vectors.c"
#endif

#ifndef PI
#define PI 3.1415926535898
#endif
// important structures and macros...

#define _3DOBJSIZEMAX (4 * 1024 * 1024) 

#define _3DVERTSIZE 36
#define _3DSEGSIZE 12 
#define _3DFACESIZE 24
#define _3DOBJSIZE 212
#define _3DOBEXSIZE 412
#define _3DZSASIZE 16
#define _3DZSLSIZE 28
#define _3DBMPSIZE 40 

#define _3DENVTRAKMAX 100
#define _3DENVLITEMAX 100
#define _3DENVMAX 5

	// drawing masks on (_3DObjL).df
#define _DMASKON	1
#define _DMASKDEG	2
#define _DMASKATT	4
#define _DMASKSUB	8

//#define 

struct _3dseg;		// Segments

struct _3dvert;		// Vertex structure

struct _3dface;		// Face structure

struct _3dobjex;	// Extra stuff for objects in _3dobjl

struct _3dobjl;		// Object link

struct _3dobja;		// Object assemblage

struct _3dbitmap;	// bitmap object

struct _xycrd;   	// X-Y coords. for screen
struct _3dcol;  	// RGB structure 


typedef struct _3dcol {
	unsigned char r;	//red
	unsigned char g;	//green
	unsigned char b;	//blue
	char flg;			// flags:  -1 = transparent (bitmaps)
} _3DCol;

typedef _3DCol *p3DCol;

typedef struct _3dseg {
	struct _3dseg *nx;
	struct _3dvert *v;	
	struct _3dvert *vf;
} _3DSeg;

typedef struct _3dvert {
	dVECTOR v;
	struct _3dobjl *obj;
 	struct _3dvert *nx;
	struct _3dseg *seg;	 // pointer to first segment
} _3DVert;

typedef _3DVert *p3DVert;

typedef struct _3dface {
	p3DVert *v;
	_3DCol rgb;
	long back;
	struct _3dobjl *obj;
	struct _3dobjl *sub;
	struct _3dface *nx;
} _3DFace;

typedef _3DSeg *p3DSeg;
typedef _3DFace *p3DFace;

typedef struct _3dbitmap {
	p3DFace f;				// pointer to face/plane to draw on
	p3DVert pov;			// position of bmp if 'f' is NULL
	float prt;				// rotation in radians of bmp 
	_3DCol *pix;			// pixel array
	long pw;				// array width	
	long ph;				// array height
	struct _3dbitmap *nx;	// pointer to next map
	double pscal;			// scaling factor of bitmap
	struct _3dobjl *obj; 	// ptr to parent 
} _3DBitmap;

typedef _3DBitmap *p3DBitmap;


// Size of _3dobjl = 212 

typedef struct _3dobjl {
	long onum;			//index number
	p3DFace f;			// pointer to first face
	p3DVert v;			// pointer to first vert
	p3DBitmap bm;		// pointer to first bitmap 
	p3DFace fx;			// last face
	p3DVert vx;			// last vert
	p3DBitmap bmx;		// last bitmap
	struct _3dobjex *xdat;		//pointer to extra data
	struct _3dobjl *nx;		//pointer to next object
	dVECTOR center;			//rotational center
	struct _3dobjl *atch;	//pointer to object this one is 'attached' to 
	dVECTOR vel;			//velocity of object
	dVECTOR rot;			//rotation period about axes
	dVECTOR rox[3];			//rotational axis 'i', 'j', 'k'
	float lumin;			//luminosity unless <0
	float refl;				// reflectance (default 1)
	int df;					// drawing flags
								// 0 - ignore this object 
								// 1 - normal, 2 - deg.
								// 4 - attached
								// 8 - bitmap
	_3DCol dcol;			// color of deg. object
	float dsize;			// radius of deg. object
	float tinit;			// Epoch when obj. initialized
	int lflag;				// Use as a light?
} _3DObjL;

typedef _3DObjL *p3DObjL;

typedef struct _3dobja {
	int atype;			// assemblage type (0 - local, 1 - near, 2 - far)
	p3DObjL	fo; 		// head link of list
	p3DObjL lo;			// last link of list
	double oscal;		// distance scale
	double dmax;		// maximum dist for objects in this ass.
	double dmin;		// minimum dist at which to move object to lower
	long mem;			// current estimate of memeory usage for assemblage
} _3DObjA;

typedef _3DObjA *p3DObjA;


// size of _3dobjex - 412 

typedef struct _3dobjex {
	long onum;				// index number 
	double mass;			// mass
	dVECTOR ax;				// acceleration
	double rho;				// atmospheric pressure 
	double sht;				// scale height of atm
	p3DObjL amass[10];			// other affecting masses
	int orbs;				// orbit segments
	dVECTOR orbp[10];			// position at seg#
	double orbt[10];			// time stamp at seg#
} _3DObjEX;

typedef _3DObjEX *p3DObjEX;

typedef struct _xycrd {
	int x;
	int y;
	int flag;
	p3DFace f;
} _XYCrd;


typedef _XYCrd *pXYCrd;

// Session display control structure...


struct _3denv;   	// 3D drawing environment parameters
struct _s3ddisp;

typedef struct _3denv {
	dVECTOR opos;	          // postion of observer
	dVECTOR ovel;   	  // velocity of observer
	dVECTOR oiv;		  // 'forward' view
	dVECTOR ojv;		  // 'left' view
	dVECTOR okv;		  // 'up' view
	double delta;		  // time scale
	int hScrn;	 	  // horizontal view res. (clip region)
	int vScrn;	 	  // vertical view res. (clip region)
	int scols;	 	  // col comp.(32K = 32, 64K =64, 16M = 256)
	double vscal;		  // pixels per meter -- 1.875 * 2 * hScrn 
	int liteon;		  // do lighting? if not, use Ghod shading
	int dmode;		  // 0 - wire frame, 1 - solid, 2 - shaded
	long zscnt;			// last face cnt for z-sort
						// 0 - initialize,  -1 - reinitialize
	int view;		  // which view? 0 current 1-g 2-v 3+ - obj
	p3DObjA local;		// local object assemblage
	p3DObjA neara;		// near object assemblage
	p3DObjA fara;		// far object assemblage
	p3DObjL lo[_3DENVLITEMAX];      // pointer to lite object array
	double lvc;		  	// lighting range scale: 5 typical
	double lvr;		  	// lighting range: 32 typical
	p3DObjL nonum[_3DENVTRAKMAX];	// tracked object list
	p3DObjL tno[_3DENVTRAKMAX];		// pointer to tracked objects
	p3DFace nface[_3DENVTRAKMAX];	// closest faces of tracked objects
	dVECTOR nfp[_3DENVTRAKMAX];	// last position of near faces
	dVECTOR nfv[_3DENVTRAKMAX];	// velocity of near faces
	dVECTOR ncu[_3DENVTRAKMAX];	// center of object relative to opos
} _3DEnv;

typedef _3DEnv *p3DEnv;

typedef struct _s3ddisp {
	int chgmode;			// video mode 0-9
	int vmode;			// svga video mode
	void *font;			// font pointer
	GraphicsContext phys;		// context pointer for screen
	int lmar;			// left border width
	int rmar;			// right border width
	int tmar;			// top border width
	int bmar;			// bottom border width
	int ETop;			// which Env window is on top
	p3DEnv Env[_3DENVMAX];		// pointer to env window
	_XYCrd Etxy[_3DENVMAX];		// topleft corner of env window
	_XYCrd Ebxy[_3DENVMAX];		// bottomright corner of env window
	int Egrow[_3DENVMAX];		// grow/shrink window if mode change?
	double isecs;			// real-time scale
	int frames;			// frame counter 1 (for timing)
	int frcnt;			// frame counter 2 (for skips)
	int fskip;			// frames to skip 
	double etime;			// elapsed disp. time
	double rtime;			// elapsed real. time
	double Eptime;			// Epoch of game initialization
	int inc;			// integer move increment
	double inca;			// float move increment
	int mx;				// last mouse x-coord.
	int my;				// last mouse y-coors.
	int mb;				// last mouse button (0 if handled)
	int key;			// last key hit (0 if handled)
	long int ccol;			// text color
	_3DCol gcol;			// gunsight color
	_3DCol dcol;			// docking tgt color
	_3DCol ncol;			// near faces color
} _3DDisp;

typedef _3DDisp *p3DDisp;

// Global display constants etcetra

const long int cticks = CLOCKS_PER_SEC;

_3DCol sLGrey = {210, 210, 210};
_3DCol sMGrey = {128, 128, 128};
_3DCol sDGrey = {75, 75, 90};
_3DCol sBrown = {200, 160, 0};
_3DCol sYellow = {200, 200, 0};
_3DCol sLBlue = {0, 0, 230};
_3DCol sLBGrn = {0, 150, 230};
_3DCol sRRed = {255, 0, 0};
_3DCol sRBlu = {0, 0, 255};
_3DCol sRGrn = {0, 255, 0};

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


// using ZSCompS2 instead
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

// FUNCTION PROTOTYPES FOR OBJECT INITIALIZATION

p3DObjA InitObjA3D(int atyp);

p3DObjL AddObj3D(p3DObjA obja, int xf, int otyp);

p3DBitmap AddBmp3D(p3DObjL obja, p3DFace fc, p3DVert pv);

p3DVert AddVert3D(p3DObjL obja, dVECTOR vval);

p3DSeg AddSeg3D(p3DVert v1, p3DVert v2);  // returns NULL if seg already added
						// return seg1 if ONLY seg1 was added, return seg2 if
						// both or only seg2 was added
p3DFace AddFace3D(p3DObjL obja);

void InitObjXDet(p3DObjL objx, long onum);

void FreeObj3D(p3DObjL obja, int fmode);

// FUNCTION DEFINITIONS

p3DFace AddFace3D(p3DObjL objr) {

	p3DFace static face;
	p3DFace ntlf;
	
	face = (p3DFace) malloc(sizeof(_3DFace));

	ntlf = objr->fx;
	if (ntlf == NULL)  objr->f = face;  // new object, no faces added yet
	else  ntlf->nx = face;
	objr->fx = face;
	face->obj = objr;
	face->sub = NULL;
	face->v = (p3DVert *) malloc(3 * sizeof(p3DVert));
	face->v[0] = NULL;
	face->v[1] = NULL;
	face->v[2] = NULL;
	face->rgb = sLGrey;
	face->nx = NULL;
	face->back = 0;
	
	return (face);
}

p3DBitmap AddBmp3D(p3DObjL obja, p3DFace fc, p3DVert pv) {

	p3DBitmap static bm;		
	p3DBitmap ntlb;

	bm = (p3DBitmap) malloc(sizeof(_3DBitmap));

	ntlb = obja->bmx;	
	if (ntlb == NULL)  obja->bm = bm;  // new object, no faces added yet
	else  ntlb->nx = bm;

	obja->bmx = bm;
	bm->obj = obja;
	bm->f = fc;
	bm->pov = pv;
	bm->prt = 0.0;
	bm->pix = NULL;
	bm->pscal = 0.01;   // one pixel = 1 cm
	bm->pw = 0;
	bm->ph = 0;
	bm->nx = NULL;

	return (bm);
}

p3DVert AddVert3D(p3DObjL objr, dVECTOR vval) {

	p3DVert static vert;
	p3DVert ntlv;

	vert = (p3DVert) malloc(sizeof(_3DVert));

	ntlv = objr->vx;
	if (ntlv == NULL)  objr->v = vert;  //new object, no vertices yet
	else  ntlv->nx = vert;
	objr->vx = vert;
	vert->obj = objr;
	vert->v = vval;
	vert->nx = NULL;
	vert->seg = NULL;
	return (vert);
}


p3DSeg AddSeg3D(p3DVert v1, p3DVert v2) {

	p3DSeg res = NULL;
	p3DSeg asg;
	p3DSeg static nsg1, nsg2;

	asg = v1->seg;
	if (asg == NULL) {	// no segs for this vert yet so add it..
		nsg1 = (p3DSeg) malloc(sizeof(_3DSeg));
		v1->seg = nsg1;
		nsg1->nx = NULL;
		nsg1->v = v2;
		nsg1->vf = NULL;
		res = nsg1;
	}
	else {
		while (1) {
			if (asg->v == v2) break;  // segment already here
			if (asg->nx == NULL) { // add new seg
				nsg1 = (p3DSeg) malloc(sizeof(_3DSeg));
				asg->nx = nsg1;
				nsg1->nx = NULL;
				nsg1->v = v2;
				nsg1->vf = NULL;
				res = nsg1;
				break;
			}
			else asg = asg->nx;
		}
	}

	asg = v2->seg;
	if (asg == NULL) {	// no segs for this vert yet so add it..
		nsg2 = (p3DSeg) malloc(sizeof(_3DSeg));
		v2->seg = nsg2;
		nsg2->nx = NULL;
		nsg2->v = v1;
		nsg2->vf = NULL;
		res = nsg2;
	}
	else {
		while (1) {
			if (asg->v == v1) break;  // segment already here
			if (asg->nx == NULL) { // add new seg
				nsg2 = (p3DSeg) malloc(sizeof(_3DSeg));
				asg->nx = nsg2;
				nsg2->nx = NULL;
				nsg2->v = v1;
				nsg2->vf = NULL;
				res = nsg2;
				break;
			}
			else asg = asg->nx;
		}
	}

	return res;
}

void InitObjXDet(p3DObjL objx, long onum) {

	p3DObjEX static pox;

	pox = (p3DObjEX) malloc(sizeof(_3DObjEX));
	objx->xdat = pox;
	pox->onum = onum;
	pox->mass = -1;
	pox->ax = dNULLV;
	pox->rho = 0.0; 
	pox->sht = 0.0;
	pox->amass[0] = NULL;
	pox->orbs = 0;
	pox->orbp[0] = dNULLV;
	pox->orbt[0] = 0.0;	
	
	return;
}


p3DObjA InitObjA3D(int atyp) {
	// .fo, .lo, .atype

	p3DObjA static obja = NULL;

	obja = (p3DObjA) malloc(sizeof(_3DObjA));

	if (obja != NULL) {
		obja->fo = NULL;
		obja->lo = NULL;
		obja->oscal = 1.0;
		obja->mem = sizeof(_3DObjA);
		switch (atyp) {
			case 1: { // NEAR
				obja->atype = 1;
				obja->dmin = 100000;
				obja->dmax = 149.6E9;
				obja->oscal = 149.6E9;
				break;
			}
			case 2: { // FAR
				obja->atype = 2;
				obja->dmin = 149.6E9;
				obja->dmax = 1.0E30;
				obja->oscal = 9.7E15;
				break;
			}
			case 0:
			default: { // LOCAL 
				obja->atype = 2;
				obja->dmin = -1;
				obja->dmax = 100000;
				break;
			}
		}
	}
	return (obja);
}

p3DObjL AddObj3D(p3DObjA obja, int xf, int otyp) {

	p3DObjL ntlo;
	p3DObjL static nobj = NULL;
	
	nobj = (p3DObjL) malloc(sizeof(_3DObjL));

	if (nobj != NULL) {
		if (obja->fo == NULL) {  // new object list
			nobj->onum = 0;
			obja->fo = nobj;
			obja->lo = nobj;
		}
		else {
			ntlo = obja->lo;
			ntlo->nx = nobj;
			obja->lo = nobj;
			nobj->onum = ntlo->onum + 1;
		}

		switch (xf) {
		   case 1:
			InitObjXDet(nobj, nobj->onum);
			break;
		   case -1: 
			nobj->xdat = NULL;
			break;
		   case 0:
		   default: break;
		}

		nobj->df = otyp; 				// degenerate object or ???
	
		nobj->atch = NULL;
		nobj->bm = NULL;
		nobj->bmx = NULL;
		nobj->nx = NULL;
		nobj->f = NULL;
		nobj->v = NULL;
		nobj->fx = NULL;
		nobj->vx = NULL;

		nobj->center = dNULLV;
		nobj->vel = dNULLV;
		nobj->rot = dNULLV;	
		nobj->rox[0] = dIunit;	
		nobj->rox[1] = dJunit;	
		nobj->rox[2] = dKunit;	
		nobj->lumin = -999;
		nobj->refl = 1.0;	
		nobj->dsize = 0;
		nobj->dcol = sLGrey;
		nobj->tinit = 0;
		nobj->lflag = 0;
	}
	return (nobj);
}

void FreeObj3D(p3DObjL obja, int fmode) {

}

#endif  // END OF 3DDATA.C
