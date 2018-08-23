//
// 3ddefs.h
//
// see 3ddefs.cpp for history
//
// 8/14/18 - part of c++ conversion project for the SDL 3DEngine
//

#ifndef _3DDEFS
#define _3DDEFS 1

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vga.h>
#include <vgagl.h>

#ifndef _VECTORS
#include "vectors.cpp"
#endif

#ifndef PI
#define PI 3.1415926535898
#endif

#define _ZSGROW 8192

// important structures and macros...

#ifndef _3DDATA
#include "3ddata.cpp"
#endif

// FUNCTION PROTOTYPES 

	// break integer color into rgb
_3DCol GetRGB(long int lcol, int vcol);

	// rgb to long int
long int PackRGB(_3DCol prgb, int vcol);

	// Get a unit vector from screen coordinates...
dVECTOR ScrUnit3D(p3DEnv tenv, _XYCrd tgt); 

	// Find out if a ray intersects a face
int FInter3D(dVECTOR v1, dVECTOR v2, dVECTOR v3, dVECTOR lv, dVECTOR vv, double *ti, dVECTOR *pv); 

	// Alternate to above, polar coordinates
_XYCrd Screen3Dr(p3DEnv tenv, dVECTOR vvert);

	// Fill a triangle with tcol (uses gl_line())
int tricolor3D(_XYCrd c1, _XYCrd c2, _XYCrd c3, long int tcol, int wf);

	// Calc area of triangle on screen in pixels
long int triarea3D(_XYCrd c1, _XYCrd c2, _XYCrd c3);
 
	// Do lighted shading - unlit faces
_3DCol FcShade3D(_3DCol fcol, p3DEnv senv, dVECTOR norm, dVECTOR ctr, float refl);
 
	// Do lighted shading - luminunous faces
_3DCol LtShade3D(_3DCol fcol, p3DEnv senv, dVECTOR norm, dVECTOR ctr, float lum); 

	// Find a face.., return -1 if no intercept, otherwise face#
void RayFace3D(p3DObjA wobj, dVECTOR pv, dVECTOR vv, p3DFace fp, p3DObjL op, double *ETA);

	// Get nearest faces of objects and put in env.
int NearFace3D(p3DObjA wobj, p3DEnv wenv);

	// Draw wireframes of objects
int DrawWire3D(p3DEnv tenv, p3DObjA tobj);

	// Draw wirefram of one face
_XYCrd DrawFW3D(p3DEnv wenv, p3DFace fn);

	// Draw zsorted and shaded objects
int DrawZS3D(p3DEnv tenv, p3DObjA tobj);

	// Renormalize view unit vectors (needed after a rotation)
void Renorm3D(p3DEnv oenv);

	// Partition an object into 4 faces/per face
int PartObj3D(p3DObjL pobj, int plvl, int tflag, int rflag, double rvar, double rv2);

	// Move all objects in structure one time step and update
void ObjMove(p3DObjA mobj, p3DEnv menv);

	// Return velocity of face 
dVECTOR FaceVel3D(p3DObjL obj, p3DFace fc);

	// Rotate all objects
void ObjRot(p3DObjA mobj, double dt);

#endif

// end of 3ddefs.h
//
