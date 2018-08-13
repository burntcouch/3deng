/* Navcom double precison test
	11/14/2000
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>           
#include <time.h>

#include "navcomdx.c"

     /* Global constants and variables*/
     // Note: the units I am currently using
     // for a consistent orbital model are:
     //   1 time unit  TU = 58.13282 days (1 Year/2*PI)
     //   1 leng. unit DU = 1 AU (149.6 million km to the Sun)
     //   1 vel. unit  VU = 29.78 km/sec (orbital speed of Earth)
     //   1 mass unit  MU = 1.334E+20 (Mass of Sun * G)
     //     This results in  TU = 2*PI*SQRT(DU^3/MU)
     //             and    VU = SQRT(MU/DU)
     //  These are defined in navcomd.c but just so you know...
  
/*---These also def'd in navcomd.c-----------------*/ 
//  MSun = 2.00E+30   		Mass of Sun in kg 
//  Sec_Day = 86400		seconds per standard (solar) day
//  Sid_Day = 86164.09		seconds per sidereal day		
//  UT2000 = 23962.92		I'm not sure 
//  Sec_Yr = 3.155693E+7	seconds per solar year		
//  Rad2Deg = 180 / PI		radians per degree
//  Deg2Rad = PI / 180		degrees per radian
//  I_ACC = 1E-6		convegence limit for Gauss and Kepler
//  Day_Yr = 365.2422		days per year (solar)
//  km_DU = 1.495997E8		kilometer in one DU
//  km_VU = 29.78485		km/sec in one VU
//  Day_TU = 58.13282		days in one TU
//  MU = 1			mass units in one MU (Sun = 1)
//  TU_Yr = 2*PI		TU in one solar year
/*--------------------------------*/ 

int main (void)
{
	nvdORBIT orb1, orb2;
	dVECTOR tv1, tv2, tv3, tv4, tv5, tv6;
	double tf1, tf2, SDate, TOF;
	double minT, maxT;
	int gcnt;

	SDate=2000.9;
	
	orb1.CurTime=SDate;
	orb2.CurTime=SDate;

	printf("\n\n\n");
	printf("\nEnter a radius vector in AUs: ");
	scanf("%lf %lf %lf",&tv1.vx,&tv1.vy,&tv1.vz);
	
	printf("\nEnter a velocity vector in km/sec: ");
	scanf("%lf %lf %lf",&tv2.vx,&tv2.vy,&tv2.vz);
	
	printf("\nEnter a Time-of-Flight in days:  ");
	scanf("%lf",&TOF);
	
	TOF = TOF / Day_TU;
	
	printf("\nEnter a radius vector in AUs: ");
	scanf("%lf %lf %lf",&tv3.vx,&tv3.vy,&tv3.vz);
	
	printf("\nEnter a velocity vector in km/sec: ");
	scanf("%lf %lf %lf",&tv4.vx,&tv4.vy,&tv4.vz);
	
	printf("\nEnter maximum TOF for transfer: ");
	scanf("%lf",&maxT);
	 
	printf("\nEnter minimum TOF for transfer: ");
	scanf("%lf",&minT);
	
    tv2=dvScal(tv2,1/km_VU);
    tv4=dvScal(tv4,1/km_VU);
    
    dGetOrb(MU, tv1, tv2, &orb1);  // Calc. basic elements
    dGetOrb(MU, tv3, tv4, &orb2);
    
    orb1.Epoch = orb1.CurTime + orb1.Epoch/TU_Yr;
    orb2.Epoch = orb2.CurTime + orb2.Epoch/TU_Yr;
    
    dKepler(TOF, &orb1, &tv1, &tv2);   // Calc r, v for orbit, TOF

    tv1=orb1.CurRad;                  // Calc v1, v2 for r1, r2, TOF
    tv2=orb1.CurVel;                                 
    tf1=dvMag(orb1.CurRad);
    tf2=dvMag(orb1.CurVel);
    
    /* Now display basic orbital elements */
    
    printf("\n\n Results.......");
    printf("\nSMA (AUs)    : %4.4f    %4.4f",orb1.SMA, orb2.SMA);
    printf("\nEcc          : %4.4f    %4.4f",orb1.Ecc, orb2.Ecc);
    printf("\nArg.Peri (Dg): %4.4f    %4.4f",
			orb1.Peri*Rad2Deg, orb2.Peri*Rad2Deg);
    printf("\nNu Zero  (Dg): %4.4f    %4.4f",
			orb1.NuZero*Rad2Deg, orb2.NuZero*Rad2Deg);
    printf("\nIncl     (Dg): %4.4f    %4.4f",
			orb1.Incl*Rad2Deg, orb2.Incl*Rad2Deg);
    printf("\nAsc.Node (Dg): %4.4f    %4.4f",
			orb1.AscN*Rad2Deg, orb2.AscN*Rad2Deg);
    printf("\nEpoch (Years): %4.4f    %4.4f",orb1.Epoch, orb2.Epoch);
    if (orb1.SMA<0) printf("\nPeriod  (Yrs): hyperbolic");
    else printf("\nPeriod  (Yrs): %4.4f",
			2*PI*sqrt(pow(orb1.SMA,3.0)/MU)/TU_Yr);
    if (orb2.SMA<0) printf(" hyperbolic");
    else printf("    %4.4f", 2*PI*sqrt(pow(orb2.SMA,3.0)/MU)/TU_Yr);
    printf("\n\nCurr. Distance from Sun  : %4.4f AUs", tf1);
    printf("\nCurr. Orbital Velocity   : %4.4f km/sec", tf2*km_VU);
    printf("\nCurr. Angle from Origin  : %4.4f deg",
		dvAcos(Iunit,tv1,dvCross(tv1,tv2))*Rad2Deg);
    printf("\n\n Gauss's Problem Test ");
    printf("\n Time     Semi-Maj     Ecc     DeltaVee1    DeltaVee2\n");
    
    tv5=tv1; 
    tv6=tv3;
    for (TOF = maxT; TOF > minT; TOF = TOF - 5)
    {
	tv1=tv5; tv3=tv6;
	gcnt = dGauss(TOF/((double) Day_TU), &orb1, &tv1, &tv3, 1);
	if (gcnt>=200) {
		tv1 = tv5; tv3 = tv6;
		gcnt = dGauss(TOF/((double) Day_TU), &orb1, &tv1, &tv3, -1);
	}
	if (gcnt<200) {
		printf("\n%5.0f ",TOF);
		printf("%d  ", gcnt);
		dGetOrb(MU, tv6, tv3, &orb2);
		printf("%4.4f    ", orb2.SMA);
		printf("%4.4f    ", orb2.Ecc);           
		printf("%4.4f    ", dvMag(dvAdd(tv2,dvScal(tv1,-1)))*km_VU);
		printf("%4.4f ",dvMag(dvAdd(tv4,dvScal(tv3,-1)))*km_VU);
    	}
	}
    printf("\n");
    exit(0);

}
