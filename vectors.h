//
// VECTORS.C - reworked for Linux 11/13/2000
// Definition of single and double prec. vector operations
// May soon include matrix operations also
// Pat Struthers    5/16/97
//
// new stuff started 12/13/2000
// 3DEngine additions 12/27/2000
// More 3DEngine additions 1/7/2001: svMidPt
// Dbl precision 3DEngine stuff 1/17/2001
//
//  reworked for c++ 8/14/18
//

#include <math.h>
#include <float.h>

#ifndef __VECTORS
#define __VECTORS 1

#ifndef PI
#define PI 3.1415926535898
#endif

struct svect;
struct dvect;

typedef struct svect
{
   float vx;
   float vy;
   float vz;
} sVECTOR; 

typedef struct dvect
{
   double vx;
   double vy;
   double vz;
} dVECTOR;


typedef sVECTOR *psVECTOR;
typedef dVECTOR *pdVECTOR;

#ifndef NULLV
sVECTOR NULLV = {0,0,0};
sVECTOR Iunit = {1,0,0};
sVECTOR Junit = {0,1,0};
sVECTOR Kunit = {0,0,1};

dVECTOR dNULLV = {0,0,0};
dVECTOR dIunit = {1,0,0};
dVECTOR dJunit = {0,1,0};
dVECTOR dKunit = {0,0,1};
#endif

// Single precision functions - the basics
inline sVECTOR vAdd(sVECTOR avect, sVECTOR bvect);
inline sVECTOR vScal(sVECTOR avect, float ascal);
inline float vDot(sVECTOR avect, sVECTOR bvect);
inline sVECTOR vCross(sVECTOR avect, sVECTOR bvect);
inline float vMag(sVECTOR avect);
  // simple rotation with an integer angle
sVECTOR vRot(sVECTOR avect, char axis, short frot);
  // disambiguous acos between avect and bvect
float vAcos(sVECTOR avect, sVECTOR bvect, sVECTOR cvect);
	// Normal vector of a triangle define by three vectors
sVECTOR vNorm(sVECTOR v1, sVECTOR v2, sVECTOR v3);
	// Center (avg.) of three vectors
sVECTOR vCenter(sVECTOR v1, sVECTOR v2, sVECTOR v3);
	// Rotate vector in object frame (tv rota degrees around irv)
sVECTOR vRot3D(sVECTOR tv, sVECTOR irv, sVECTOR jrv, float rota);
	// Float version of vRot
sVECTOR fvRot(sVECTOR resv, char axis, float frot);
	// Calc. midpoint of s1, s2; optionally via center/radius
sVECTOR vMidPt(sVECTOR s1, sVECTOR s2, int tflag, sVECTOR ctrv, int rflag, float rvar);

//
// Double precision functions
//
inline dVECTOR vAdd(dVECTOR avect, dVECTOR bvect);
inline dVECTOR vScal(dVECTOR avect, double ascal);
inline double vDot(dVECTOR avect, dVECTOR bvect);
inline dVECTOR vCross(dVECTOR avect, dVECTOR bvect);
inline double vMag(dVECTOR avect);
  // simple rotation with an integer angle
dVECTOR vRot(dVECTOR avect, char axis, short frot);
  // disambiguous acos between avect and bvect
double vAcos(dVECTOR avect, dVECTOR bvect, dVECTOR cvect);
	// Normal vector of a triangle define by three vectors
dVECTOR vNorm(dVECTOR v1, dVECTOR v2, dVECTOR v3);
	// Center (avg.) of three vectors
dVECTOR vCenter(dVECTOR v1, dVECTOR v2, dVECTOR v3);
	// Rotate vector in object frame (tv rota degrees around irv)
dVECTOR vRot3D(dVECTOR tv, dVECTOR irv, dVECTOR jrv, double rota);
	// Float version of vRot
dVECTOR fvRot(dVECTOR resv, char axis, double frot);
	// Calc. midpoint of s1, s2; optionally via center/radius
dVECTOR vMidPt(dVECTOR s1, dVECTOR s2, int tflag, dVECTOR ctrv, int rflag, double rvar);

#endif   // end of vectors.h
