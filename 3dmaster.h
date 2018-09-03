//
// 3dmaster.h
//
//
//  master system include file for 3d engine
//


#include <iostream>
#include <math.h>
#include <float.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#ifndef __DVECTOR
#include "vectoro.cpp"   // vector library
#endif
// PI and PI_L
// class DVector
//
// DV_acos
// DV_norm
// DV_center
// DV_midpt

#ifndef __3DBASE
#include "3dbase.cpp"    // XYCrd, Colora, Trig3D objects
#endif
// class XYCrd
// class Trig3D
// class Colora
//
// clear_to_Colora
// draw_Trig3D

#ifndef __3DOBJ
#include "3dobj.cpp"    // 3d object definitions and basic functions
#endif
// class Seg3D
// class Vert3D
// class Face3D
// class Obj3D
// class ObjXdat
// class ObjList

#ifndef __3DDISP
#include "3ddisp.cpp"   // environment and display
#endif
// class Env3D
// class Disp3D
//


#ifndef __3DGEN
#include "3dgen.cpp"    // 3d object shape generators and managers
#endif

//

