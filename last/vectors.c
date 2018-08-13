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
sVECTOR svAdd(sVECTOR avect, sVECTOR bvect);
sVECTOR svScal(sVECTOR avect, float ascal);
float svDot(sVECTOR avect, sVECTOR bvect);
sVECTOR svCross(sVECTOR avect, sVECTOR bvect);
float svMag(sVECTOR avect);
sVECTOR svRot(sVECTOR avect, char axis, short frot);
float svAcos(sVECTOR avect, sVECTOR bvect, sVECTOR cvect);


// Double precision functions
dVECTOR dvAdd(dVECTOR avect, dVECTOR bvect);
dVECTOR dvScal(dVECTOR avect, double ascal);
double dvDot(dVECTOR avect, dVECTOR bvect);
dVECTOR dvCross(dVECTOR avect, dVECTOR bvect);
double dvMag(dVECTOR avect);
dVECTOR dvRot(dVECTOR avect, char axis, short frot);
double dvAcos(dVECTOR avect, dVECTOR bvect, dVECTOR cvect);

// Stuff added for the 3DEngine project 12/27/00
	// Normal vector of a triangle define by three vectors
sVECTOR sNorm(sVECTOR v1, sVECTOR v2, sVECTOR v3);
	// Center (avg.) of three vectors
sVECTOR sCenter(sVECTOR v1, sVECTOR v2, sVECTOR v3);
	// Rotate vector in object frame (tv rota degrees around irv)
sVECTOR VecRot3D(sVECTOR tv, sVECTOR irv, sVECTOR jrv, float rota);
	// Float version of svRot
sVECTOR fvRot(sVECTOR resv, char axis, float frot);

	// Calc. midpoint of s1, s2; optionally via center/radius
	// 1/7/2001 12:30 AM uggghh.
sVECTOR svMidPt(sVECTOR s1, sVECTOR s2, int tflag, sVECTOR ctrv, 
			int rflag, float rvar);


// 3DEngine project 12/27/00 - Double prec. stuff: 01/17/01
 
	// Normal vector of a triangle define by three vectors
dVECTOR dNorm(dVECTOR v1, dVECTOR v2, dVECTOR v3);
	// Center (avg.) of three vectors
dVECTOR dCenter(dVECTOR v1, dVECTOR v2, dVECTOR v3);
	// Rotate vector in object frame (tv rota degrees around irv)
dVECTOR dVecRot3D(dVECTOR tv, dVECTOR irv, dVECTOR jrv, double rota);
	// Float version of svRot
dVECTOR dfvRot(dVECTOR resv, char axis, double frot);
	// Calc. midpoint of s1, s2; optionally via center/radius
dVECTOR dvMidPt(dVECTOR s1, dVECTOR s2, int tflag, dVECTOR ctrv, 
			int rflag, double rvar);

// Conversions...
#define SDC_OK 0
#define SDC_ERR 1
short sdConv(dVECTOR cvect, psVECTOR psv);
void dsConv(sVECTOR cvect, pdVECTOR pdv);
 
// Flesh out conversions...
 
short sdConv(dVECTOR cvect, psVECTOR psv)
{
	short ErrLvl=SDC_OK;
	
	if ((dvMag(cvect)<FLT_MAX) && (dvMag(cvect)>FLT_MIN))
	{
	 	psv->vx= (float) cvect.vx;
		psv->vy= (float) cvect.vy;
		psv->vz= (float) cvect.vz;
	}
	else ErrLvl=SDC_ERR;
	return (ErrLvl);
}

void dsConv(sVECTOR cvect, pdVECTOR pdv)
{
	pdv->vx=cvect.vx;
	pdv->vy=cvect.vy;
	pdv->vz=cvect.vz;
}

// Flesh out single precision stuff.....

/*vector add*/
sVECTOR svAdd(sVECTOR avect, sVECTOR bvect)
{
   sVECTOR tempvect;
   tempvect.vx = avect.vx + bvect.vx;
   tempvect.vy = avect.vy + bvect.vy;
   tempvect.vz = avect.vz + bvect.vz;
   return (tempvect);   
}

/*vector scalar multiply*/
sVECTOR svScal(sVECTOR avect, float ascal)
{
   sVECTOR tempvect;
   tempvect.vx = avect.vx * ascal;
   tempvect.vy = avect.vy * ascal;
   tempvect.vz = avect.vz * ascal;
   return (tempvect);
} 

/*vector dot product*/
float svDot(sVECTOR avect, sVECTOR bvect)
{
   double tempf;
   tempf = avect.vx * bvect.vx + avect.vy * bvect.vy +avect.vz * bvect.vz;
   return ((float)tempf);
}

/*vector cross product*/
sVECTOR svCross(sVECTOR avect, sVECTOR bvect)
{
   sVECTOR tempvect;
   tempvect.vx = avect.vy * bvect.vz - avect.vz * bvect.vy;
   tempvect.vy = avect.vz * bvect.vx - avect.vx * bvect.vz;
   tempvect.vz = avect.vx * bvect.vy - avect.vy * bvect.vx;
   return (tempvect);
}

/*vector magnitude*/
float svMag(sVECTOR avect)
{
   double tempf;
   tempf = avect.vx*avect.vx+avect.vy*avect.vy+avect.vz*avect.vz; 
   tempf = sqrt(tempf);
   return ((float)tempf);
}

sVECTOR svRot(sVECTOR avect, char axis, short frot)
{
   sVECTOR tempvect;
   double flrot;
   double rotax[3][3]= {{1,0,0},{0,1,0},{0,0,1}};
   
   flrot = PI * ((double) frot) / 180.0;
   switch (axis)
   {
      case 'x':
         rotax[1][1]=cos(flrot);
         rotax[2][1]=-sin(flrot);
         rotax[1][2]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'y':
         rotax[0][0]=cos(flrot); 
         rotax[0][2]=-sin(flrot);
         rotax[2][0]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'z':
         rotax[0][0]=cos(flrot); 
         rotax[1][0]=-sin(flrot);
         rotax[0][1]=sin(flrot); 
         rotax[1][1]=cos(flrot);
         break;
      default:
         break;
   }
   tempvect.vx =(float)(avect.vx*rotax[0][0]+avect.vy*rotax[1][0]+avect.vz*rotax[2][0]);
   tempvect.vy =(float)(avect.vx*rotax[0][1]+avect.vy*rotax[1][1]+avect.vz*rotax[2][1]);
   tempvect.vz =(float)(avect.vx*rotax[0][2]+avect.vy*rotax[1][2]+avect.vz*rotax[2][2]);
   return (tempvect);
}    

// A non-quadrant ambiguous arc-cosine function.
// A and B vectors define angle and C-vect is the vector
// defining the "motion reference" for A and B.

float svAcos(sVECTOR avect, sVECTOR bvect, sVECTOR cvect)
{
	double theCos;
	sVECTOR theVec;
	
	theCos= (svDot(avect,bvect)/(svMag(avect))/svMag(bvect));
	theCos= acos(theCos);
	theVec= svCross(avect,bvect);
	if (svDot(theVec,cvect)<0) theCos=2*PI - theCos;
    return ((float) theCos);
}

// Double precision stuff now.... 


/*vector add*/
dVECTOR dvAdd(dVECTOR avect, dVECTOR bvect)
{
   dVECTOR tpvect;
   tpvect.vx = avect.vx + bvect.vx;
   tpvect.vy = avect.vy + bvect.vy;
   tpvect.vz = avect.vz + bvect.vz;
   return (tpvect);   
}

/*vector scalar multiply*/
dVECTOR dvScal(dVECTOR avect, double ascal)
{
   dVECTOR tempvect;
   tempvect.vx = avect.vx * ascal;
   tempvect.vy = avect.vy * ascal;
   tempvect.vz = avect.vz * ascal;
   return (tempvect);
} 

/*vector dot product*/
double dvDot(dVECTOR avect, dVECTOR bvect)
{
   double tempf;
   tempf = avect.vx * bvect.vx + avect.vy * bvect.vy +avect.vz * bvect.vz;
   return (tempf);
}

/*vector cross product*/
dVECTOR dvCross(dVECTOR avect, dVECTOR bvect)
{
   dVECTOR tempvect;
   tempvect.vx = avect.vy * bvect.vz - avect.vz * bvect.vy;
   tempvect.vy = avect.vz * bvect.vx - avect.vx * bvect.vz;
   tempvect.vz = avect.vx * bvect.vy - avect.vy * bvect.vx;
   return (tempvect);
}

/*vector magnitude*/
double dvMag(dVECTOR avect)
{
   double tempf;
   tempf = avect.vx*avect.vx+avect.vy*avect.vy+avect.vz*avect.vz; 
   tempf = sqrt(tempf);
   return (tempf);
}

dVECTOR dvRot(dVECTOR avect, char axis, short frot)
{
   dVECTOR tempvect;
   double flrot;
   double rotax[3][3]= {{1,0,0},{0,1,0},{0,0,1}};
   
   flrot = PI * frot / 180;
   switch (axis)
   {
      case 'x':
         rotax[1][1]=cos(flrot);
         rotax[2][1]=-sin(flrot);
         rotax[1][2]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'y':
         rotax[0][0]=cos(flrot); 
         rotax[0][2]=-sin(flrot);
         rotax[2][0]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'z':
         rotax[0][0]=cos(flrot); 
         rotax[1][0]=-sin(flrot);
         rotax[0][1]=sin(flrot); 
         rotax[1][1]=cos(flrot);
         break;
      default:
         break;
   }
   tempvect.vx =(avect.vx*rotax[0][0]+
	avect.vy*rotax[1][0]+avect.vz*rotax[2][0]);
   tempvect.vy =(avect.vx*rotax[0][1]+
	avect.vy*rotax[1][1]+avect.vz*rotax[2][1]);
   tempvect.vz =(avect.vx*rotax[0][2]+
	avect.vy*rotax[1][2]+avect.vz*rotax[2][2]);
   return (tempvect);
}

// A non-quadrant ambiguous arc-cosine function.
// A and B vectors define angle and C-vect is the vector
// defining the "motion reference" for A and B.
// Be sure NOT to get A & B backwards!  12/03/02

double dvAcos(dVECTOR avect, dVECTOR bvect, dVECTOR cvect)
{
	double theCos;
	dVECTOR theVec;
	
	theCos = (dvDot(avect,bvect)/(dvMag(avect) * dvMag(bvect)));
	theCos = acos(theCos);
	theVec = dvCross(avect,bvect);
	if (dvDot(theVec,cvect) < 0) theCos = 2 * PI - theCos;
    return (theCos);
}

sVECTOR sNorm(sVECTOR v1, sVECTOR v2, sVECTOR v3) {
	sVECTOR tv1, tv2, resv;
	
	tv1 = svAdd(v1, svScal(v2, -1));
	tv2 = svAdd(v2, svScal(v3, -1));
	resv = svCross(tv1, tv2);
	resv = svScal(resv, 1/svMag(resv));
	return resv;
}

sVECTOR sCenter(sVECTOR v1, sVECTOR v2, sVECTOR v3) {
	sVECTOR tmpv;
	
	tmpv = svAdd(v1, v2);
	tmpv = svAdd(tmpv, v3);
	tmpv = svScal(tmpv, 1 / 3.0F);
	return tmpv;
}

sVECTOR fvRot(sVECTOR resv, char axis, float frot){

   sVECTOR tempvect;
   double flrot;
   double rotax[3][3]= {{1,0,0},{0,1,0},{0,0,1}};
   
   flrot = PI * frot / 180.0;
   switch (axis)
   {
      case 'x':
         rotax[1][1]=cos(flrot);
         rotax[2][1]=-sin(flrot);
         rotax[1][2]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'y':
         rotax[0][0]=cos(flrot); 
         rotax[0][2]=-sin(flrot);
         rotax[2][0]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'z':
         rotax[0][0]=cos(flrot); 
         rotax[1][0]=-sin(flrot);
         rotax[0][1]=sin(flrot); 
         rotax[1][1]=cos(flrot);
         break;
      default:
         break;
   }
   tempvect.vx =(float)(resv.vx*rotax[0][0]+
		resv.vy*rotax[1][0]+resv.vz*rotax[2][0]);
   tempvect.vy =(float)(resv.vx*rotax[0][1]+
		resv.vy*rotax[1][1]+resv.vz*rotax[2][1]);
   tempvect.vz =(float)(resv.vx*rotax[0][2]+
		resv.vy*rotax[1][2]+resv.vz*rotax[2][2]);
   return tempvect;
}

sVECTOR VecRot3D(sVECTOR tv, sVECTOR irv, sVECTOR jrv, float rota) {
	char rax = 'x';
	sVECTOR krv, rv1, rv2;

	krv = svCross(irv, jrv);
	rv1.vx = svDot(irv, tv);
	rv1.vy = svDot(jrv, tv);
	rv1.vz = svDot(krv, tv);
	rv1 = fvRot(rv1, rax, rota);
	rv2.vx = rv1.vx*irv.vx + rv1.vy*jrv.vx + rv1.vz*krv.vx;
	rv2.vy = rv1.vx*irv.vy + rv1.vy*jrv.vy + rv1.vz*krv.vy;
	rv2.vz = rv1.vx*irv.vz + rv1.vy*jrv.vz + rv1.vz*krv.vz;
	return rv2;
}


sVECTOR svMidPt(sVECTOR s1, sVECTOR s2, int tflag, sVECTOR ctrv,
			int rflag, float rvar) {
		// NOTE: rvar should be the proportion by which
		//   you wish to vary the radius (or whatever)
		// i.e. typically < 0.5

	float ls1, ls2, lr;
	sVECTOR vhat, vt1, vt2;

	vhat = svScal(svAdd(s1, s2), 0.5);

	//if a centered radius midpoint is wanted...
	if (tflag) {
 		ls1 = svMag(svAdd(s1, svScal(ctrv, -1)));
		ls2 = svMag(svAdd(s2, svScal(ctrv, -1)));
		lr = (ls1 + ls2) / 2;
		if (rflag) {
			lr += (lr*rvar) * ((float) rand() / RAND_MAX - 0.5);
		}
		vt2 = svAdd(vhat, svScal(ctrv, -1));
		vt1 = svScal(vt2, 1 / svMag(vt2));
		vhat = svScal(vt1, lr);
		vhat = svAdd(vhat, ctrv);
	}
	else if (rflag) {
 		ls1 = svMag(svAdd(s1, svScal(vhat, -1)));
		ls2 = svMag(svAdd(s2, svScal(vhat, -1)));
		lr = (ls1 + ls2) / 2;
		lr = (lr*rvar) * (((float) rand()) / RAND_MAX - 0.5);
		vhat.vx += lr;
		lr = (lr*rvar) * (((float) rand()) / RAND_MAX - 0.5);
		vhat.vy += lr;
		lr = (lr*rvar) * (((float) rand()) / RAND_MAX - 0.5);
		vhat.vz += lr;
	}

	return (vhat);
}

dVECTOR dNorm(dVECTOR v1, dVECTOR v2, dVECTOR v3) {
	dVECTOR tv1, tv2, resv;
	
	tv1 = dvAdd(v1, dvScal(v2, -1));
	tv2 = dvAdd(v2, dvScal(v3, -1));
	resv = dvCross(tv1, tv2);
	resv = dvScal(resv, 1/dvMag(resv));
	return resv;
}

dVECTOR dCenter(dVECTOR v1, dVECTOR v2, dVECTOR v3) {
	dVECTOR tmpv;
	
	tmpv = dvAdd(v1, v2);
	tmpv = dvAdd(tmpv, v3);
	tmpv = dvScal(tmpv, 1 / 3.0F);
	return tmpv;
}

dVECTOR dfvRot(dVECTOR resv, char axis, double frot){

   dVECTOR tempvect;
   double flrot;
   double rotax[3][3]= {{1,0,0},{0,1,0},{0,0,1}};
   
   flrot = frot * PI / 180.0;
   switch (axis)
   {
      case 'x':
         rotax[1][1]=cos(flrot);
         rotax[2][1]=-sin(flrot);
         rotax[1][2]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'y':
         rotax[0][0]=cos(flrot); 
         rotax[0][2]=-sin(flrot);
         rotax[2][0]=sin(flrot); 
         rotax[2][2]=cos(flrot);
         break;
      case 'z':
         rotax[0][0]=cos(flrot); 
         rotax[1][0]=-sin(flrot);
         rotax[0][1]=sin(flrot); 
         rotax[1][1]=cos(flrot);
         break;
      default:
         break;
   }

   tempvect.vx =(resv.vx*rotax[0][0]+
		resv.vy*rotax[1][0]+resv.vz*rotax[2][0]);
   tempvect.vy =(resv.vx*rotax[0][1]+
		resv.vy*rotax[1][1]+resv.vz*rotax[2][1]);
   tempvect.vz =(resv.vx*rotax[0][2]+
		resv.vy*rotax[1][2]+resv.vz*rotax[2][2]);
   return tempvect;
}

dVECTOR dVecRot3D(dVECTOR tv, dVECTOR irv, dVECTOR jrv, double rota) {
	char rax = 'x';
	dVECTOR krv, rv1, rv2;

	krv = dvCross(irv, jrv);
	rv1.vx = dvDot(irv, tv);
	rv1.vy = dvDot(jrv, tv);
	rv1.vz = dvDot(krv, tv);
	rv1 = dfvRot(rv1, rax, rota);
	rv2.vx = rv1.vx*irv.vx + rv1.vy*jrv.vx + rv1.vz*krv.vx;
	rv2.vy = rv1.vx*irv.vy + rv1.vy*jrv.vy + rv1.vz*krv.vy;
	rv2.vz = rv1.vx*irv.vz + rv1.vy*jrv.vz + rv1.vz*krv.vz;
	return rv2;
}


dVECTOR dvMidPt(dVECTOR s1, dVECTOR s2, int tflag, dVECTOR ctrv,
			int rflag, double rvar) {
		// NOTE: rvar should be the proportion by which
		//   you wish to vary the radius (or whatever)
		// i.e. typically < 0.5

	double ls1, ls2, lr;
	dVECTOR vhat, vt1, vt2;

	vhat = dvScal(dvAdd(s1, s2), 0.5);

	//if a centered radius midpoint is wanted...
	if (tflag) {
 		ls1 = dvMag(dvAdd(s1, dvScal(ctrv, -1)));
		ls2 = dvMag(dvAdd(s2, dvScal(ctrv, -1)));
		lr = (ls1 + ls2) / 2;
		if (rflag) {
			lr += (lr*rvar) * ((double) rand() / RAND_MAX - 0.5);
		}
		vt2 = dvAdd(vhat, dvScal(ctrv, -1));
		vt1 = dvScal(vt2, 1 / dvMag(vt2));
		vhat = dvScal(vt1, lr);
		vhat = dvAdd(vhat, ctrv);
	}
	else if (rflag) {
 		ls1 = dvMag(dvAdd(s1, dvScal(vhat, -1)));
		ls2 = dvMag(dvAdd(s2, dvScal(vhat, -1)));
		lr = (ls1 + ls2) / 2;
		lr = (lr*rvar) * (((double) rand()) / RAND_MAX - 0.5);
		vhat.vx += lr;
		lr = (lr*rvar) * (((double) rand()) / RAND_MAX - 0.5);
		vhat.vy += lr;
		lr = (lr*rvar) * (((double) rand()) / RAND_MAX - 0.5);
		vhat.vz += lr;
	}

	return (vhat);
}
#endif //end of vectors.c

