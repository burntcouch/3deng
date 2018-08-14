//
// VECTORS.C - reworked for Linux 11/13/2000
//
// reworked again, c++ style
// see vectors.h for prototypes
//

#include "vectors.h"

// Flesh out single precision stuff.....

/*vector add*/
inline sVECTOR vAdd(sVECTOR avect, sVECTOR bvect)
{
   sVECTOR tempvect;
   tempvect.vx = avect.vx + bvect.vx;
   tempvect.vy = avect.vy + bvect.vy;
   tempvect.vz = avect.vz + bvect.vz;
   return (tempvect);   
}

/*vector scalar multiply*/
inline sVECTOR vScal(sVECTOR avect, float ascal)
{
   sVECTOR tempvect;
   tempvect.vx = avect.vx * ascal;
   tempvect.vy = avect.vy * ascal;
   tempvect.vz = avect.vz * ascal;
   return (tempvect);
} 

/*vector dot product*/
inline float vDot(sVECTOR avect, sVECTOR bvect)
{
   double tempf;
   tempf = avect.vx * bvect.vx + avect.vy * bvect.vy +avect.vz * bvect.vz;
   return ((float)tempf);
}

/*vector cross product*/
inline sVECTOR sCross(sVECTOR avect, sVECTOR bvect)
{
   sVECTOR tempvect;
   tempvect.vx = avect.vy * bvect.vz - avect.vz * bvect.vy;
   tempvect.vy = avect.vz * bvect.vx - avect.vx * bvect.vz;
   tempvect.vz = avect.vx * bvect.vy - avect.vy * bvect.vx;
   return (tempvect);
}

/*vector magnitude*/
inline float vMag(sVECTOR avect)
{
   double tempf;
   tempf = avect.vx*avect.vx+avect.vy*avect.vy+avect.vz*avect.vz; 
   tempf = sqrt(tempf);
   return ((float)tempf);
}

sVECTOR vRot(sVECTOR avect, char axis, short frot)
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

float vAcos(sVECTOR avect, sVECTOR bvect, sVECTOR cvect)
{
	double theCos;
	sVECTOR theVec;
	
	theCos= (vDot(avect,bvect)/(vMag(avect))/vMag(bvect));
	theCos= acos(theCos);
	theVec= vCross(avect,bvect);
	if (vDot(theVec,cvect)<0) theCos=2*PI - theCos;
    return ((float) theCos);
}

// Double precision stuff now.... 

/*vector add*/
inline dVECTOR vAdd(dVECTOR avect, dVECTOR bvect)
{
   dVECTOR tpvect;
   tpvect.vx = avect.vx + bvect.vx;
   tpvect.vy = avect.vy + bvect.vy;
   tpvect.vz = avect.vz + bvect.vz;
   return (tpvect);   
}

/*vector scalar multiply*/
inline dVECTOR vScal(dVECTOR avect, double ascal)
{
   dVECTOR tempvect;
   tempvect.vx = avect.vx * ascal;
   tempvect.vy = avect.vy * ascal;
   tempvect.vz = avect.vz * ascal;
   return (tempvect);
} 

/*vector dot product*/
inline double vDot(dVECTOR avect, dVECTOR bvect)
{
   double tempf;
   tempf = avect.vx * bvect.vx + avect.vy * bvect.vy +avect.vz * bvect.vz;
   return (tempf);
}

/*vector cross product*/
inline dVECTOR vCross(dVECTOR avect, dVECTOR bvect)
{
   dVECTOR tempvect;
   tempvect.vx = avect.vy * bvect.vz - avect.vz * bvect.vy;
   tempvect.vy = avect.vz * bvect.vx - avect.vx * bvect.vz;
   tempvect.vz = avect.vx * bvect.vy - avect.vy * bvect.vx;
   return (tempvect);
}

/*vector magnitude*/
inline double vMag(dVECTOR avect)
{
   double tempf;
   tempf = avect.vx*avect.vx+avect.vy*avect.vy+avect.vz*avect.vz; 
   tempf = sqrt(tempf);
   return (tempf);
}

dVECTOR vRot(dVECTOR avect, char axis, short frot)
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

double vAcos(dVECTOR avect, dVECTOR bvect, dVECTOR cvect)
{
	double theCos;
	dVECTOR theVec;
	
	theCos = (vDot(avect,bvect)/(vMag(avect) * vMag(bvect)));
	theCos = acos(theCos);
	theVec = vCross(avect,bvect);
	if (vDot(theVec,cvect) < 0) theCos = 2 * PI - theCos;
    return (theCos);
}

sVECTOR vNorm(sVECTOR v1, sVECTOR v2, sVECTOR v3) {
	sVECTOR tv1, tv2, resv;
	
	tv1 = vAdd(v1, vScal(v2, -1));
	tv2 = vAdd(v2, vScal(v3, -1));
	resv = vCross(tv1, tv2);
	resv = vScal(resv, 1/vMag(resv));
	return resv;
}

sVECTOR vCenter(sVECTOR v1, sVECTOR v2, sVECTOR v3) {
	sVECTOR tmpv;
	
	tmpv = vAdd(v1, v2);
	tmpv = vAdd(tmpv, v3);
	tmpv = vScal(tmpv, 1 / 3.0F);
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

sVECTOR vRot3D(sVECTOR tv, sVECTOR irv, sVECTOR jrv, float rota) {
	char rax = 'x';
	sVECTOR krv, rv1, rv2;

	krv = vCross(irv, jrv);
	rv1.vx = vDot(irv, tv);
	rv1.vy = vDot(jrv, tv);
	rv1.vz = vDot(krv, tv);
	rv1 = fvRot(rv1, rax, rota);
	rv2.vx = rv1.vx*irv.vx + rv1.vy*jrv.vx + rv1.vz*krv.vx;
	rv2.vy = rv1.vx*irv.vy + rv1.vy*jrv.vy + rv1.vz*krv.vy;
	rv2.vz = rv1.vx*irv.vz + rv1.vy*jrv.vz + rv1.vz*krv.vz;
	return rv2;
}


sVECTOR vMidPt(sVECTOR s1, sVECTOR s2, int tflag, sVECTOR ctrv,
			int rflag, float rvar) {
		// NOTE: rvar should be the proportion by which
		//   you wish to vary the radius (or whatever)
		// i.e. typically < 0.5

	float ls1, ls2, lr;
	sVECTOR vhat, vt1, vt2;

	vhat = vScal(vAdd(s1, s2), 0.5);

	//if a centered radius midpoint is wanted...
	if (tflag) {
 		ls1 = vMag(vAdd(s1, vScal(ctrv, -1)));
		ls2 = vMag(vAdd(s2, vScal(ctrv, -1)));
		lr = (ls1 + ls2) / 2;
		if (rflag) {
			lr += (lr*rvar) * ((float) rand() / RAND_MAX - 0.5);
		}
		vt2 = vAdd(vhat, vScal(ctrv, -1));
		vt1 = vScal(vt2, 1 / vMag(vt2));
		vhat = vScal(vt1, lr);
		vhat = vAdd(vhat, ctrv);
	}
	else if (rflag) {
 		ls1 = vMag(vAdd(s1, vScal(vhat, -1)));
		ls2 = vMag(vAdd(s2, vScal(vhat, -1)));
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

dVECTOR vNorm(dVECTOR v1, dVECTOR v2, dVECTOR v3) {
	dVECTOR tv1, tv2, resv;
	
	tv1 = vAdd(v1, vScal(v2, -1));
	tv2 = vAdd(v2, vScal(v3, -1));
	resv = vCross(tv1, tv2);
	resv = vScal(resv, 1/vMag(resv));
	return resv;
}

dVECTOR vCenter(dVECTOR v1, dVECTOR v2, dVECTOR v3) {
	dVECTOR tmpv;
	
	tmpv = vAdd(v1, v2);
	tmpv = vAdd(tmpv, v3);
	tmpv = vScal(tmpv, 1 / 3.0F);
	return tmpv;
}

dVECTOR fvRot(dVECTOR resv, char axis, double frot){

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

dVECTOR vRot3D(dVECTOR tv, dVECTOR irv, dVECTOR jrv, double rota) {
	char rax = 'x';
	dVECTOR krv, rv1, rv2;

	krv = vCross(irv, jrv);
	rv1.vx = vDot(irv, tv);
	rv1.vy = vDot(jrv, tv);
	rv1.vz = vDot(krv, tv);
	rv1 = fvRot(rv1, rax, rota);
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

	vhat = vScal(dvAdd(s1, s2), 0.5);

	//if a centered radius midpoint is wanted...
	if (tflag) {
 		ls1 = vMag(dvAdd(s1, dvScal(ctrv, -1)));
		ls2 = vMag(dvAdd(s2, dvScal(ctrv, -1)));
		lr = (ls1 + ls2) / 2;
		if (rflag) {
			lr += (lr*rvar) * ((double) rand() / RAND_MAX - 0.5);
		}
		vt2 = vAdd(vhat, vScal(ctrv, -1));
		vt1 = vScal(vt2, 1 / vMag(vt2));
		vhat = vScal(vt1, lr);
		vhat = vAdd(vhat, ctrv);
	}
	else if (rflag) {
 		ls1 = vMag(vAdd(s1, vScal(vhat, -1)));
		ls2 = vMag(vAdd(s2, vScal(vhat, -1)));
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

//end of vectors.cpp
