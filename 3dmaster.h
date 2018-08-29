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

#ifndef __3DBASE
#include "3dbase.cpp"    // XYCrd, Colora, Trig3D objects
#endif

#ifndef __3DOBJ
#include "3dobj.cpp"    // 3d object definitions and basic functions
#endif

#ifndef __3DDISP
#include "3ddisp.cpp"   // environment and display
#endif

#ifndef __3DGEN
#include "3dgen.cpp"    // 3d object shape generators and managers
#endif

