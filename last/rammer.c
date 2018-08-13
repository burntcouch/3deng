//
//
// rammer.c  -- retooled for Linux starting 01/20/2001
//
// originally Ver. 1 for VC++
//
// Pat Struthers ..  initialized 5/23/97
//
// v0.1  - 01/20/2001
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>    // for sqrt()

#ifndef _RAMMER
#define _RAMMER 1

struct _ramctrl;

typedef struct _ramctrl
{
	double sMass;  //Mass of ship in kg
	int bi;        //ionization field power 0-100
	int bf;        //funnel field power 0-100
	int bg;        //generator field power 0-100
	int bp;        //pinch field power 0-100
	int fdiv;      //funnel field divergence 5.0 - 45.0 deg
    	double vel;    //velocity km/sec
    	double PGen;   //Internal power generated
    	double Flux;   //Proton flux in throat
    	double Drag;   //Total drag
    	double TNet;   //Net thrust
} _RamCtrl;

typedef _RamCtrl *pRamCtrl;

//FUNCTION PROTOTYPES

double RamAcc(pRamCtrl aRam, double nH2);

	// nH2  -- number density of H2 in mole/cm^3
	//    this is typically 1.0 in Milky Way
	//  but in solar neigh. is 0.02 to 0.1.
	//  Much higher for solar wind.....
	//    nH2 approx (1 x 10^6) / (dist in AUs from sun)

double gamma(double vel);

//GLOBAL VARIABLES


const float Rc = 3E+8;			// speed of light m/sec
const float Rk = 1.381E-23;		// Boltzman's constant
const float mH = 1.672E-27;		// mass of proton kg
const float Sec_Yr = 3.155693E+7;	// seconds in a year
const float Sec_Day = 86400;		// seconds in a solar day
const float m_AU = 1.496E+11;		// astronomical unit in m

double Ly = Rc * Sec_Yr;		// 1 Light Year in m

/*
******* 
 Example initialization stuff	
	_RamCtrl odyRam;
    
	odyRam.sMass=55000000;  
	odyRam.bi=0;
	odyRam.bf=0;
	odyRam.bg=0;
	odyRam.bp=50;
	odyRam.fdiv=50;
	odyRam.vel=vel;
	odyRam.PGen=0;
	
       	ax=RamAcc(&odyRam,n1);    
**********
*/
 
				// the relativistic gamma function
double gamma(double vel)
{
	double ig;
	
	ig = 1/sqrt(1-(vel * vel)/(Rc * Rc));
	
	return (ig);
}

double RamAcc(pRamCtrl aRam, double nH2)
{
	const double efDrag = 4E-17;
	const double etDrag = 4E-35;
	const double eH = 0.0073;
	const double eG = 1E-10;
	
	double fRad = 1E+6;
	double nR=5E+4;
	double E0=mH * Rc * Rc;
	double g, x, eI, F, D, eP, Tg, Tn, Beta, fang;
    
    	g = gamma(aRam->vel);
    	Beta = aRam->vel / Rc;
    	fang = (aRam->fdiv * 0.1) * PI / 180;   
					// divergence angle in radians
 
   	x = fRad * (aRam->bf * 0.01) * sin(fang); 
					// x - funnel area

    	eI = (aRam->bi * 0.01)/sqrt(4*sin(fang)*g);  
					// ionization eff.

    	F = PI * x * x * n * eI * aRam->vel;         
                                        //Total proton flux in throat

	eP = (aRam->bp * 0.01) * (n/nR) * sqrt(g * Beta) * 0.5;
					// Pinch field eff.
	Tg = F * E0 * eH * eP * g;                   
                                        //Gross thrust

	D = x * x * efDrag * pow(Rc * Beta, 2) * eI;
	D = D + etDrag * eP * eP * F * F * (0.5 + aRam->bg * 0.005);
    				//Subtract drag due to
	Tn = Tg - D;	        //funnel and throat and get..
                                //Net thrust
	aRam->PGen = aRam->bg * F * eG  * .001;      //Generator Power
	aRam->Flux = F;
	aRam->Drag = D;
	aRam->TNet = Tn;
    
	return (Tn/(aRam->sMass * g));   //Return acceleration in m/sec^2
}

#endif
//  End of rammer.c

