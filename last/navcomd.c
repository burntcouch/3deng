/* Navcomd.c
	
     Functions, constants, data structures for celestial mechanics:
	Orbit - structure to hold orbits of objects
	GetOrb - generates an Orbit from r,v vectors
	Kepler - calc. new r,v of object after elapsed time
	Gauss - calc. interception orbit parms between two r's, elapsed time
	plus a bunch of important constants for celestial mech.
	
     ----  Revision history  -----
	Double precision version of Navcom.c
	11/14/2000

	Initialized 5/7/97
	
	To test navigational computer components for
	Galactic Explorer...
	
	5/7  -- Add basic vector stuff and start on 
		Kepler and Guass problems.
		
	5/16 --- Removed vector stuff to vectors.inc file.
	
	11/13/2000 - basic revisions to make work for Linux (gcc)
	11/14/2000 - coverted this copy to double prec. floats
	01/17/2001 - modifications for compat. with dbl prec. 3D Engine
*/

#ifndef _NAVCOMD
#define _NAVCOMD 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>           

#include "vectors.c"

      /*  Structures and Macros */
/*--------------------------------*/
#define PI 3.1415926535898

struct dOrbit;

typedef struct dOrbit
{
	double muP;          // Mass function of primary
	dVECTOR CurRad;     // Current radius vect WRT muP frame
	dVECTOR CurVel;     // Current velocity WRT muP frame
	double CurTime;      // Current date 
	double Epoch;        // Date of passage through pericentre
	double Incl;         // Incl to ref. plane of muP
	double AscN;         // Ascending node through plane of muP
	double Ecc;          // Eccentricity
	double NuZero;     	// True anomaly (ang between peri and curr) 
	double Peri;         // Arg of Pericentre WRT muP frame
	double SMA;          // Semi-major axis 
} nvdORBIT;

typedef nvdORBIT *pnvdORBIT;

		/* Functions for Navcom */
/*--------------------------------*/
double CZ (double zval);

double CZp (double zval);

double SZ (double zval);

double SZp (double zval);

double dFact(short fnum);    // factorial of fnum

double IntPow(double mant, short exp);   // integer powers of doubles 

void dGetOrb(double fpmu, dVECTOR radv, dVECTOR velv, pnvdORBIT porb);

void dKepler(double fTOF, pnvdORBIT pOrbit, pdVECTOR pNewR, pdVECTOR pNewV);

int dGauss(double fTOF, pnvdORBIT pOrbit, 
		pdVECTOR pav, pdVECTOR pbv, int odir);

     /* Global constants and variables*/
     // Note: the units I am currently using
     // for a consistent orbital model are:
     //   1 time unit  TU = 58.13282 days (1 Year/2*PI)
     //   1 leng. unit DU = 1 AU (149.6 million km to the Sun)
     //   1 vel. unit  VU = 29.78 km/sec (orbital speed of Earth)
     //   1 mass unit  MU = 1.334E+20 (Mass of Sun * G)
     //     This results in  TU = 2*PI*SQRT(DU^3/MU)
     //             and    VU = SQRT(MU/DU)
       
/*--------------------------------*/ 
const double MSun = 2.00E+30;
const double Sec_Day = 86400;
const double Sid_Day = 86164.09;
const double UT2000 = 23962.92;
const double Sec_Yr = 3.155693E+7;
const double Rad2Deg = 180 / PI;
const double Deg2Rad = PI / 180;
const double I_ACC = 1E-6;
const double Day_Yr = 365.2422;
const double km_DU = 1.495997E8;
const double km_VU = 29.78485;
const double Day_TU = 58.13282;
const double MU = 1;
const double TU_Yr = 2*PI;
/*--------------------------------*/ 
#ifndef dNULLV
dVECTOR dIunit = {1,0,0};     // direction of vernal equinox
dVECTOR dJunit = {0,1,0};     // 90 degrees CCW around K from I
dVECTOR dKunit = {0,0,1};     // ecliptic 'north' pole
dVECTOR dNULLV = {0,0,0};     // nothing going nowhere
#endif
/*--------------------------------*/ 


/* Code for Navcom functions */
double dFact(short fnum)
{
	short ii;
	double rnum;
	
	for (ii=2, rnum=1; ii<=fnum; ii++) rnum = rnum * ii;	
	return rnum;
}

double IntPow(double mant, short exp)
{
	short ii;
	double tot;

	if (exp>0)
		for (ii=1,tot=1; ii<=exp; ii++) tot=tot*mant;
	else tot=1;
    return (tot);
} 

double CZ (double zval)
{
	short i = 0;
	double zz, dz;

	zz=0.0;
	do {
		dz=zz;
		zz+=IntPow(-zval,i)/dFact(2*i+2);
		i++;	
	} while (fabs((zz-dz)/(zz+I_ACC)) > I_ACC);
	return (zz);
}

double CZp (double zval)
{
	short i = 0;
	double zz, dz;

	zz=0.0;
	do
	{
		dz=zz;
		zz+=(i+1)*IntPow(-zval,i)/dFact(2*i+4);
		i++;	
	} while (fabs((zz-dz)/(zz+I_ACC)) > I_ACC);
	return (zz);
}

double SZ (double zval)
{
	short i = 0;
	double zz, dz;
	zz=0;
	do
	{
		dz=zz;
		zz+=IntPow(-zval,i)/dFact(2*i+3);
		i++;	
	} while (fabs((zz-dz)/(zz+I_ACC)) > I_ACC);
	return (zz);
}

double SZp (double zval)
{
	short i = 0;
	double zz, dz;
	zz=0;
	do {
		dz = zz;
		zz += (i+1)*IntPow(-zval,i)/dFact(2*i+5);
		i++;	
	} while (fabs((zz-dz)/(zz+I_ACC)) > I_ACC);
	return (zz);
}

void dGetOrb(double fpmu, dVECTOR radv, dVECTOR velv, pnvdORBIT porb)
{
	double fvel, frad, f1, f2, fSMA, fnu, fecc;
	dVECTOR hv, nv, ev;
	double dEF, dTOF;
	
	hv = dvCross(radv,velv);
	nv = dvCross(dKunit,hv);
	f1 = -dvDot(radv,velv);
	frad = dvMag(radv);
	fvel = dvMag(velv);
	f2 = fvel*fvel-fpmu/frad;
	ev = dvAdd(dvScal(radv,f2),dvScal(velv,f1));
	ev = dvScal(ev,1/fpmu);

	porb->CurRad = radv;
	porb->CurVel = velv;
	porb->Peri  = dvAcos(dIunit,ev,hv);
	porb->NuZero = dvAcos(radv,ev,hv);
	porb->Incl  = acos(hv.vz/dvMag(hv));
	porb->AscN  = dvAcos(dIunit,nv,hv);
	porb->Ecc   = dvMag(ev);
	porb->muP   = fpmu;
	porb->SMA   = fpmu/(2*fpmu/frad - fvel*fvel);
    
	dEF = (fecc + cos(fnu))/(1+fecc*cos(fnu));
	fSMA = porb->SMA;
	fnu = porb->NuZero;
	fecc = porb->Ecc;

	if (fSMA > 0)    // elliptical orbit
	{
		dEF = acos(dEF);
		if ((fnu>PI) && (dEF<PI)) dEF = 2*PI - dEF;
		dTOF = sqrt(fSMA*fSMA*fSMA/fpmu)*(dEF-fecc*sin(dEF));	
	}
	else            // hyperbolic orbit
	{
		dEF = log(dEF+sqrt(dEF*dEF-1));
		dTOF = sqrt(pow(-fSMA,3.0)/fpmu)*(fecc*sinh(dEF)-dEF);	
	}
    
	porb->Epoch  = dTOF;
}


void dKepler(double fTOF, pnvdORBIT pOrbit, pdVECTOR pNewR, pdVECTOR pNewV)
{
	double SQRMU, fX, fZ, fC, fS, t1, tn, falpha, ftmp;
	double fFsc, fGsc, fFscp, fGscp;
	double r0, v0, rDv, newr;
	short i=0;
	
	SQRMU =sqrt(pOrbit->muP);
	falpha = pOrbit->SMA;
	r0=dvMag(pOrbit->CurRad);
    	v0=dvMag(pOrbit->CurVel);
    	rDv=dvDot(pOrbit->CurRad,pOrbit->CurVel);
    	if (falpha>0)
    		t1 = fmod(fTOF,2*PI*sqrt(pow(falpha,3.0))/SQRMU);
    	else t1=fTOF;
    
    	fX=SQRMU*t1/falpha;
    
    do
    {
    	fZ=fX*fX/falpha;
    	fC=CZ(fZ);
    	fS=SZ(fZ);
    	tn = rDv*fX*fX*fC/SQRMU;
    	tn+= (1-r0/falpha)*fX*fX*fX*fS + r0*fX;
    	tn = tn/SQRMU;
	ftmp= fX*fX*fC + rDv*fX*(1-fZ*fS)/SQRMU;
	ftmp+= r0*(1-fZ*fC);
	ftmp = ftmp/SQRMU;
	fX = fX + (t1-tn)/ftmp;
	i++;
    } while (fabs((tn-t1)/t1)>I_ACC);
    fFsc =(1 - fX*fX*fC/r0);
    fGsc =(t1 - fX*fX*fX*fS/SQRMU);
    *pNewR = dvAdd(dvScal(pOrbit->CurRad,fFsc),dvScal(pOrbit->CurVel,fGsc));
    newr = dvMag(*pNewR);
    fFscp = (SQRMU*fX*(fZ*fS - 1)/(r0 * newr));
    fGscp = (1 - fX*fX*fC/newr);
    *pNewV = dvAdd(dvScal(pOrbit->CurRad,fFscp),dvScal(pOrbit->CurVel,fGscp));    
}

int dGauss(double fTOF, pnvdORBIT pOrbit,
	 pdVECTOR pav, pdVECTOR pbv, int odir)
{
	double SQRMU, fX, fY, fZ, fC, fS, fCp, fSp, t1, tn;
	double fFsc, fGsc, fGscp;
	dVECTOR rv1, rv2, hv;
	double r1, r2, fA, delnu, ftmp, DZ, DM;
	short i=0;
	
	hv = dvCross(pOrbit->CurRad,pOrbit->CurVel);
	rv1 = *pav; 
	rv2 = *pbv;
	delnu = dvAcos(rv1,rv2,hv);
	if (delnu>PI) DM = -1.0;
		else DM = 1.0;
	if (odir) DM = (double) odir;
	r1 = dvMag(rv1); 
	r2 = dvMag(rv2);
	SQRMU = sqrt(pOrbit->muP);
	t1 = fTOF;
	fA = DM*sqrt(r1*r2*(1+cos(delnu)));
	fZ = 0;
    do
    {
    	fC=CZ(fZ); fCp=CZp(fZ);
    	fS=SZ(fZ); fSp=SZp(fZ);
    	fY =r1+r2-(fA*(1-fZ*fS)/sqrt(fC));
    	if (fY>0)
    	{
	    	fX =sqrt(fY/fC);
	    	tn =(fX*fX*fX*fS+fA*sqrt(fY))/SQRMU;
			ftmp = fX*fX*fX*(fSp-3*fS*fCp/2*fC);
			ftmp += fA*(3*fS*sqrt(fY)/fC + fA/fX)/8;
			DZ = SQRMU*(t1-tn)/ftmp;
        	fZ = fZ + DZ;
	   }
	   else 
	   {
	   	fZ=fZ-DZ/2;
	    	DZ=DZ/2;
	   }
		i++; 
	if (i>199) break;
   } while (fabs((tn-t1)/t1)>I_ACC);
	fFsc =(1 - fY/r1);
	fGsc =(fA*sqrt(fY)/SQRMU);
   	fGscp =(1 - fY/r2);
  	*pav = dvScal(dvAdd(rv2,dvScal(rv1,-fFsc)),(1/fGsc));
  	*pbv = dvScal(dvAdd(dvScal(rv2,fGscp),dvScal(rv1,-1)),(1/fGsc));	
	return i;
}

#endif
// end of navcomd.c
