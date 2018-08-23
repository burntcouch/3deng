
// 
// 3ddata.h
//
//  see 3ddata.cpp for history
//
// 8/14/18 - breaking out header files for modularization and C++
//

#ifndef _3DDATA
#define _3DDATA 1

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vga.h>
#include <vgagl.h>

#ifndef _VECTORS
#include "vectors.cpp"
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

  // segments
typedef struct _3dseg {
	struct _3dseg *nx;
	struct _3dvert *v;	
	struct _3dvert *vf;
} _3DSeg;

typedef _3DSeg *p3DSeg;

  // vertices
typedef struct _3dvert {
	dVECTOR v;
	struct _3dobjl *obj;
 	struct _3dvert *nx;
	struct _3dseg *seg;	 // pointer to first segment
} _3DVert;

typedef _3DVert *p3DVert;

  // faces
typedef struct _3dface {
	p3DVert *v;
	_3DCol rgb;
	long back;
	struct _3dobjl *obj;
	struct _3dobjl *sub;
	struct _3dface *nx;
} _3DFace;

typedef _3DFace *p3DFace;

  // bitmaps (for texture mapping?  Not sure this was ever used...
  //
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
// The main object structure (one element of the object linked list below)
//
//  all objects, faces and vertices are linked lists
//
//
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

//
// The main object list, or a 'universe'; pretty much anything renderable is stored in a 3DObjA structure
//

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

//
// Extended 'physics' properties for objects 
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
struct _s3ddisp;  // display settings

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

// qsort compare function prototypes for z-sorting

int ZSCompS( const void * a, const void * b);
int ZSCompS2( const void * a, const void * b);


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


#endif
// end of 3ddata.h
//
