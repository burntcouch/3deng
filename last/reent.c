//
// reent.c
//		Reentry code testing
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "navcomdx.c"
#include "mylib/phttp.c"

struct _navprog;

typedef struct _navprog {
	int nvpi;		// -1 = end of list
	int cmd;		// 0 - nop, 1 - re-orient ship, 2 - main engine
					// 3 - attitude jets, 4 - recalc orbit
	double *cprm;	// extra parms for above
	dVECTOR v1;		// vector1, i.e. thrust vector or orientation
	double t;		// timestamp
	double dt;		// duration of operation
	int ovf;		// overide flag.  Complete this operation even if 
					// next one is overdue starting?
} nvdProg;

typedef nvdProg *pnvdProg;

const double EM_DU = 10000;  // kms
const double EM_VU = 6.3135;   // km/sec --- 
const double EM_AU = 3.9860;   // m/sec^2
const double EM_TU = 2.7644;  // hrs  

double frand();  // crappy double prec random #

void DumpOrb(pnvdORBIT orb);

int main(int argc, char *argv[]);

int main(int argc, char *argv[]) {

	int i, j, k;
	double fi, fj, fk, fm, fn, fl;


	double delv, tr[4], rng1, rng2, phib;
	double tt;

	double JDT = 2450000.0;

	nvdORBIT orb[6];
	nvdORBIT torb;
	double Long[6], Omeg;

	double M_Gany = 0.0248;
	double R_Gany = 0.2638;
	double a_Gany = 107.1;	
	double e_Gany = 0.0014;
	double M_Eur = 0.00796;
	double R_Eur = 0.1563;
	double a_Eur = 67.09;
	double e_Eur = 0.0001;

	double M_Jup = 317.8;
	double R_Jup = 7.0;

	double M_ship = 10000;  // kg
	double Ax_ship = 1.0;  // EM_AU
	double ro_ship = 500; // km
	double i_ship = 0.0;  // inclination
	double a_ship, e_ship;
	double DT = 1.0 / (EM_TU * 3600);	// 1 sec

	dVECTOR tgt = dIunit;
	dVECTOR di, dj, hv;
	dVECTOR Pv, Qv;
	double vrot;

	srand(time(NULL));
	orb[0].SMA = a_Gany; orb[0].Ecc = e_Gany; orb[0].muP = M_Jup;
	orb[1].SMA = a_Eur; orb[1].Ecc = e_Eur; orb[1].muP = M_Jup;

	vrot = sqrt(pow(a_Gany, 3.0)/M_Jup) * EM_TU * 3600;
	vrot = 2 * PI * R_Gany / vrot;
	tgt = dvScal(tgt, R_Gany);

	// Current position of Eur/Gany

	for (i = 0; i < 3; i++) {
		Omeg = 2 * PI * frand();
		orb[i].AscN = 2 * PI * frand();
		orb[i].Peri = orb[i].AscN + Omeg;
		if (orb[i].Peri > 2*PI) orb[i].Peri = orb[i].Peri - 2*PI;
		if (orb[i].Peri < 0) orb[i].Peri = orb[i].Peri + 2*PI;

		orb[i].Incl = 2.0 * frand() * frand() * Deg2Rad;
		orb[i].NuZero = 2 * PI * frand();
		if (i >= 2) {
			orb[i].Incl = PI * frand();
			orb[i].SMA = ro_ship / 10000.0 + R_Gany; 
			orb[i].Ecc = 0.02 * frand(); 
			orb[i].muP = M_Gany;
		}
		orb[i].CurTime = JDT * 24 / (EM_TU);    
		orb[i].UpdTime = JDT * 24 / (EM_TU);
		orb[i].Di[0] = -1;
			//
			// convert orbital elements to r & v vectors
		dOrb2RV(&orb[i]);
			// 
			// check orbit and calc. Epoch & longitude
		di.vx = orb[i].CurRad.vx;
		di.vy = orb[i].CurRad.vy;
		di.vz = 0.0;
	//	di.vz = orb[i].CurRad.vz;

		Long[i] = orb[i].Peri + orb[i].NuZero;
		if (Long[i] > 2 * PI) Long[i] = Long[i] - 2 * PI;
		if (Long[i] < 0) Long[i] = Long[i] + 2 * PI;

		dGetOrb(orb[i].muP, orb[i].CurRad, orb[i].CurVel, &torb);
		orb[i].Epoch = torb.Epoch;
		hv = dvCross(orb[i].CurRad, orb[i].CurVel);
		fk = acos(hv.vz/dvMag(hv));
		printf("---------------------------------------------\n");
		printf("Object #%d : Long - %6.2f : incl  "
			"%5.2f\n", i, Long[i] * Rad2Deg, fk * Rad2Deg);
		DumpOrb(&orb[i]);
		printf("Check  \n", i);
		torb.CurTime = orb[i].CurTime;    
		torb.UpdTime = orb[i].UpdTime;
		torb.Di[0] = -1;
		DumpOrb(&torb);
		
	}


//	tt = nvdReEnterCalc( , , , , , tr, *rng1, *rng2, *delv, *phib);


	exit(0);
}

double frand() {
	double fi;

	fi = (double)(rand() % 10000);
	return (fi/10000.0); 
}

void DumpOrb(pnvdORBIT orb) {
	int i;
	printf("\n");
	printf(" SMA       Ecc   ArgPer  AscN    NuZero  "
			"Incl  -   rx     ry     rz\n");
	printf(" %6.2f  %4.4f  %5.1f  %5.1f  %5.1f  %5.2f : "
		"%6.3f %6.3f %6.3f\n",
		orb->SMA, orb->Ecc, orb->Peri * Rad2Deg, orb->AscN * Rad2Deg
		, orb->NuZero * Rad2Deg, orb->Incl * Rad2Deg
		, orb->CurRad.vx, orb->CurRad.vy, orb->CurRad.vz);	
	printf("     Epoch %.3f    Date %.3f       :  %6.3f %6.3f %6.3f\n",
		orb->Epoch * EM_TU / 24, orb->CurTime * EM_TU / 24, 
		orb->CurVel.vx * EM_VU, orb->CurVel.vy * EM_VU, 
		orb->CurVel.vz * EM_VU); 
	return;
}

// ReEnter(double qr, double vq, double amax, double g, double vrot,
//	 double *t, double *brng, double *urng, double *dv, double *thetab);
//
//		nvdORBIT;
//	double muP;          // Mass function of primary
//	dVECTOR CurRad;     // Current radius vect WRT muP frame
//	dVECTOR CurVel;     // Current velocity WRT muP frame
//	double CurTime;      // Current time 
//	double UpdTime;	 // Update time
//	double Epoch;        // Date of passage through pericentre
//	double Incl;         // Incl to ref. plane of muP
//	double AscN;         // Ascending node through plane of muP
//	double Ecc;          // Eccentricity
//	double NuZero;     	// True anomaly (ang between peri and curr) 
//	double Peri;         // Arg of Pericentre WRT muP frame
//	double SMA;          // Semi-major axis 
//	int Di[10];		// disturber indexes (-1 for none)
//	dVECTOR Dax[10];	// disturbing acc (up to 10)

// end of reent.c

