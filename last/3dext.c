//
// 3dext.c -  v0.4LL (5/11/03)
//    A bunch of handy object generation and
//      manipulation tools  
//	to use with my 3D Engine.
//
// - Pat (Captain Chaos) Struthers
//
// Some original code from 12/20/00
//
// Amalgamated to this module 12/27/00
// Added square fractal terran generator 1/7/2001
// 	-> fracht, htrand, randi
//      -> Also fleshed out Sphere3D generator
// Double precision conversion 01/17/2001
// Dynamic object allocation 01/18/2001
//	Linked-list object model re-write:  5/11/03
//  

#ifndef _3DEXT
#define _3DEXT 1

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef _VECTORS
#include "vectors.c"
#endif

// Some cool functions to extend and exploit 3ddefs.c
//

#define _CMAPMAX 256
#define _EXTRUDEMAX 256

struct _htcmap;
struct _extmap;

typedef struct _htcmap {
	int lcnt;			// # of color levels
	double lvl[_CMAPMAX];		// level cutoff (0 - .999)
	_3DCol hcol[_CMAPMAX];		// color for that level
	int gradf;			// Grade between colors?
} _HtCMap;

typedef struct _extmap {
	int segs;
	int pnts;
	int xtyp;  // 0 - flat, 1 - wrap around z, 2 - arc around z
		// 3 - ??
	double x[_EXTRUDEMAX];
	double y[_EXTRUDEMAX];
	_3DCol fcol[_EXTRUDEMAX];
	double rad;  // radius or height from ctr
	double p1;  // depends on context of xtyp 
	dVECTOR ctr;
	double wid;  	
} _ExtMap;

typedef _HtCMap *pHtCMap;
typedef _ExtMap *pExtMap;


// Random terrain/grid mapping stuff added 1/7/2001

#define ARRSIZ 256  
#define ARRWID 257  // for use with nonsqure grids 
#define ARRLEN 257  // yee yee for putting stuff in other objects

int htarr[ARRSIZ+1][ARRSIZ+1];

	// Basic 0 -> (ir-1) random integer function
int rndi(int ir);

	// Basic -fr/2 -> fr/2 random float and double function
float rndf(float fr);
 
double rndd(double fr);

 	// Random function for terrain maps
int htrand(int rarg, int varf);

	// A square fractal terrain routine (no wrapping)
	// Note:  Need to rewrite this and any descendants so that
	// a pointer to the array is passed.  That way can dynamically
	// allocate the array....
	//
int fracht(int vf1, int vf2, int vf3, int vf4, int vf5, int vf6); 
   		// Corner Scale 	vf1
   		// Countdown Scale 	vf2
   		// Countup Scale 	vf3
   		// Partition Scale 	vf4
   		// Up-Down Offset	vf5
   		// Smoothing Scale 	vf6

	//Maximum number of points to extrude

// FUNCTION PROTOTYPES

	// Get an acceleration vector due to mass at rvec (relative)
dVECTOR sGrav(double mass, dVECTOR rvec);

	// Get accelerations of objects due to mass and update vels
int sGravOx(p3DObjA pobj, double dt, int otf);

	// Return a square plane wide x deep of tsize 
	// if rflag > 0 vary height by rflag * tsize 
p3DObjL PlaneObj3D(p3DObjA pobj, int wide, int deep, 
		double tsize, double high, _3DCol pcol, double rflag);

	// Extrudes tsize pnts of pobj in segs around the z-axis
p3DObjL Extrude3D(p3DObjA pobj, pExtMap xm);

	// Make a 'degenerate' object - no verts/faces  
p3DObjL DegenObj3D(p3DObjA dobj, double rad, dVECTOR center);
	
	// Generate a sphere of a given 'level' 
p3DObjL SphereObj3D(p3DObjA pobj, double rad, int clvl, dVECTOR center,
			int rflag, double rv1, double rv2);

	// Paint a centered object with a graded color map
double HtPaint3D(p3DObjL pobj, pHtCMap cmap); 

	// Function to smooth/roughen an object after coloring or ...
void HtSmooth3D(p3DObjL pobj, double rad, double sf); 

// FUNCTION DEFINITIONS

	// Returns an acceleration vector corresponding to gravity!
	// Add sequentially for multi-mass situations
	//
dVECTOR sGrav(double mass, dVECTOR rvec) {
	dVECTOR acc;
	double fr;

	fr = pow(dvMag(rvec), 3.0);
	fr = -mass / fr;
	acc = dvScal(rvec, fr);

	return acc;
}

	// A more general gravity updater...
	// otf - flag to enable updating of orbital tracks

int sGravOx(p3DObjA pobj, double dt, int otf) {

	int i, j, k;

	dVECTOR acc, ctro, ctri;
	double fr, mass;
	p3DObjEX pobx, poby;
	p3DObjL objx, objy;

	objx = pobj->fo;
	while (objx != NULL) {
		pobx = objx->xdat;
		if (pobx != NULL) {
			j = 0; 
			acc = dNULLV;
			ctro = objx->center;
			while (pobx->amass[j] != NULL) {
				objy = pobx->amass[j];
				poby = objy->xdat;
				if (poby != NULL) {
					mass = poby->mass;
					ctri = objy->center;
					ctri = dvAdd(ctro, dvScal(ctri, -1));
					fr = pow(dvMag(ctri), 3.0);
					fr = -mass / fr;
					acc = dvAdd(acc, dvScal(ctri, fr));
				}
				j++;
			}
			pobx->ax = acc;
			objx->vel = dvAdd(objx->vel, dvScal(acc, dt));
		}
		objx = objx->nx;
	}

	return 1;
}

p3DObjL PlaneObj3D(p3DObjA pobj, int wide, int deep,
		 double tsize, double high, _3DCol pcol, double rflag) {
	int pi, pj, pk, pm;
	int nv, nf, ne;
	double twid, hvar;
	p3DObjL objr = NULL;
	long osize;
	int nobj = 0;
	dVECTOR vt;
	p3DVert *varr;
	p3DFace f1;

	srand(time(NULL));

	hvar = (double) RAND_MAX;
	hvar = tsize / (2 * hvar);

	twid = wide * tsize / 2;
	nv = (wide + 1) * (deep + 1);
	nf = wide * deep * 2;
	ne = 3 * wide * deep + wide + deep;

	osize = (nf * _3DFACESIZE * 5 / 4) + (nv * _3DVERTSIZE)
		+ (ne * _3DSEGSIZE * 5 / 2) + _3DOBJSIZE + _3DOBEXSIZE;

	pobj->mem = pobj->mem + osize;

	if (osize < _3DOBJSIZEMAX) {

		objr = AddObj3D(pobj, -1, 1);  

		if (objr != NULL) {
			nobj = 1; 
			varr = (p3DVert *) malloc(nv * sizeof(p3DVert));

			for (pi = 0; pi < wide + 1; pi++) {
				for (pj = 0; pj < deep + 1; pj++) {
					vt.vx = pj * tsize;
					vt.vy =  pi * tsize - twid;	
					vt.vz =  high + rflag * rand() * hvar;
					varr[pi*(deep+1)+pj] = AddVert3D(objr, vt);
				}
			}
			for (pi = 0; pi < wide; pi++) {
				for (pj = 0; pj < deep; pj++) {
					f1 = AddFace3D(objr);					
					f1->v[0] = varr[pi*(deep+1)+pj];
					f1->v[1] = varr[(pi+1)*(deep+1)+pj];
					f1->v[2] = varr[(pi+1)*(deep+1)+pj+1];
					f1->back = 1;
					f1->rgb = pcol;

					f1 = AddFace3D(objr);					
					f1->v[0] = varr[pi*(deep+1)+pj];
					f1->v[1] = varr[(pi+1)*(deep+1)+pj+1];
					f1->v[2] = varr[pi*(deep+1)+pj+1];
					f1->back = 1;
					f1->rgb = pcol;
				}
			}	
			objr->center.vx = deep * tsize / 2; 
			objr->center.vy = 0.0; 
			objr->center.vz = high; 
	   }
	}
		printf("\n Plane ok?  verts/faces/edges = %d/%d/%d \n", 
					nv, nf, ne);
		fflush(stdout);
	return (objr);
}

void HtSmooth3D(p3DObjL pobj, double rad, double sf) {
	// if sf > 1 exaggerate instead of smooth
	// sf = 1 do nothing for a while
	int i;
	dVECTOR ctr, tv1, tv2;
	double orad, nrad;
	p3DVert vt;

	ctr = pobj->center;

	vt = pobj->v;
	while (vt != NULL) {
		tv1 = dvAdd(vt->v, dvScal(ctr, -1));
		orad = dvMag(tv1);
		tv2 = dvScal(tv1, 1 / orad);
		nrad = rad + (orad - rad) * sf;
		tv1 = dvScal(tv2, nrad);
		vt->v = dvAdd(tv1, ctr);

		vt = vt->nx;
	} 

	return;
}

double HtPaint3D(p3DObjL pobj, pHtCMap cmap) {
	int i, j;
	int ftop;
	double ar, ag, ab;
	double ht, vall, peak, hr, t1;
	dVECTOR tva, tvb, tvc, ctr, cv;
	p3DFace fc;

	vall = 1.0E30;
	peak = 0.0;
	
	if (pobj != NULL) {
		ctr = pobj->center;
		// figure scale by faces
		fc = pobj->f;
		while (fc != NULL) {
			tva = fc->v[0]->v; 	
			tvb = fc->v[1]->v; 	
			tvc = fc->v[2]->v; 	
			cv = dCenter(tva, tvb, tvc);
			cv = dvAdd(cv, dvScal(ctr, -1));
			ht = dvMag(cv);
			if (ht > peak) peak = ht;
			if (ht < vall) vall = ht;
			fc = fc->nx;
		}
		hr = peak - vall;
	
		fc = pobj->f;
		while (fc != NULL) {
			tva = fc->v[0]->v; 	
			tvb = fc->v[1]->v; 	
			tvc = fc->v[2]->v; 	
			cv = dCenter(tva, tvb, tvc);
			cv = dvAdd(cv, dvScal(ctr, -1));
			ht = dvMag(cv);
			for (j = 0; j < cmap->lcnt; j++) {
				if ((ht-vall)/hr > cmap->lvl[j]) {
					if ((!cmap->gradf || !j) || (j == cmap->lcnt - 1)) {
					  fc->rgb = cmap->hcol[j];
					}
					else {
				 	  ar = cmap->hcol[j].r;
					  ag = cmap->hcol[j].g;
					  ab = cmap->hcol[j].b;
					  t1 = cmap->lvl[j] - cmap->lvl[j-1];
					  t1 = ((ht-vall)/hr)/t1;
					  ar += (t1 * (double) 
						cmap->hcol[j-1].r);
					  ag += (t1 * (double) 
						cmap->hcol[j-1].g);
				  	  ab += (t1 * (double)
						 cmap->hcol[j-1].b);
		 		 	  ar = ar / (1 + t1);
					  ag = ag / (1 + t1);
					  ab = ab / (1 + t1);
					  fc->rgb.r = (int) ar;
					  fc->rgb.g = (int) ag;
					  fc->rgb.b = (int) ab;
					}
					break;
				}
			}
			fc = fc->nx;		
		}
	}
	return hr;

} 

p3DObjL DegenObj3D(p3DObjA dobj, double rad, dVECTOR center) {

	int nobj = 0;
	p3DObjL obja = NULL;

	obja = AddObj3D(dobj, -1, 2);

	if (obja != NULL) {
		dobj->mem = dobj->mem + sizeof(_3DObjL);
		obja->center = center;
		obja->dcol = sMGrey;
		obja->dsize = rad;
		obja->f = AddFace3D(obja);
		nobj = 1;
	}
	return (obja);
}

p3DObjL SphereObj3D(p3DObjA pobj, double rad, int clvl, dVECTOR center,
			int rflag, double rvar, double rv2) {
	p3DObjL sto;
	int i, j, k;
	int nobj;
	long ne, nv, nf, osize;
	double trv = 0;
	p3DVert vt, varr[8];
	p3DFace ft;

	// Initialize random #'s
	srand(time(NULL));

	if (clvl == 0) { // Degenerate case, don't define shape for now 

		sto = AddObj3D(pobj, -1, 2);

		if (sto != NULL) {
			pobj->mem = pobj->mem + sizeof(_3DObjL);
			
			sto->center = center;
			sto->dcol = sMGrey;
			sto->dsize = rad;
			nobj = 1;
		}
		return (sto);
	}
	else {	// Okay, actually draw a sphere, i guess..
		nf = 8;
		ne = 12;
		nv = 6;
		
		for (i = 0; i < clvl; i++) { 
			nv = nv + ne;
			ne = 2 * ne + 3 * nf;
			nf = nf * 4;
		}

		osize = (nf * _3DFACESIZE * 5 / 4) + (nv * _3DVERTSIZE)
			+ (ne * _3DSEGSIZE * 5 / 2) + _3DOBJSIZE + _3DOBEXSIZE;

		if (osize < _3DOBJSIZEMAX) {

			sto = AddObj3D(pobj, -1, 1);

			pobj->mem = pobj->mem + osize;

			nobj = 1;

			// Define an octahedron

			sto->center = center;
			if (rflag) trv = rndd(rvar * 2);
			vt = AddVert3D(sto, dvAdd(center, dvScal(dKunit, trv + rad)));
			varr[0] = vt;
			if (rflag) trv = rndd(rvar * 2);
			vt = AddVert3D(sto,	dvAdd(center, dvScal(dJunit, trv + rad)));
			varr[1] = vt;
			if (rflag) trv = rndd(rvar * 2);
			vt = AddVert3D(sto,	dvAdd(center, dvScal(dIunit, trv + rad)));
			varr[2] = vt;
			if (rflag) trv = rndd(rvar * 2);
			vt = AddVert3D(sto,	dvAdd(center, dvScal(dJunit, -rad + trv)));
			varr[3] = vt;
			if (rflag) trv = rndd(rvar * 2);
			vt = AddVert3D(sto,	dvAdd(center, dvScal(dIunit, -rad + trv)));
			varr[4] = vt;
			if (rflag) trv = rndd(rvar * 2);
			vt = AddVert3D(sto,	dvAdd(center, dvScal(dKunit, -rad + trv)));
			varr[5] = vt;

			ft = AddFace3D(sto);
			ft->v[0] = varr[5]; ft->v[1] = varr[1]; 
			ft->v[2] = varr[2];
			ft->rgb = sMGrey;
			ft->back = 0;
			ft = AddFace3D(sto);
			ft->v[0] = varr[5]; ft->v[1] = varr[2]; 
			ft->v[2] = varr[3];
			ft->rgb = sMGrey;
			ft->back = 0;
			ft = AddFace3D(sto);
			ft->v[0] = varr[5]; ft->v[1] = varr[3]; 
			ft->v[2] = varr[4];
			ft->rgb = sMGrey;
			ft->back = 0;
			ft = AddFace3D(sto);
			ft->v[0] = varr[5]; ft->v[1] = varr[4]; 
			ft->v[2] = varr[1];
			ft->rgb = sMGrey;
			ft->back = 0;
			ft = AddFace3D(sto);
			ft->v[0] = varr[0]; ft->v[1] = varr[1]; 
			ft->v[2] = varr[4];
			ft->rgb = sMGrey;
			ft->back = 0;
			ft = AddFace3D(sto);
			ft->v[0] = varr[0]; ft->v[1] = varr[4]; 
			ft->v[2] = varr[3];
			ft->rgb = sMGrey;
			ft->back = 0;
			ft = AddFace3D(sto);
			ft->v[0] = varr[0]; ft->v[1] = varr[3]; 
			ft->v[2] = varr[2];
			ft->rgb = sMGrey;
			ft->back = 0;
			ft = AddFace3D(sto);
			ft->v[0] = varr[0]; ft->v[1] = varr[2]; 
			ft->v[2] = varr[1];
			ft->rgb = sMGrey;
			ft->back = 0;

			i = PartObj3D(sto, clvl, 1, rflag, rvar, rv2);

			printf("\n Sphere ok?  verts/faces/edges = %d/%d/%d \n", 
					nv, nf, ne);
			fflush(stdout);
	
			return (sto);
		}
		else return (NULL);
	}
}

p3DObjL Extrude3D(p3DObjA pobj, pExtMap xm) {
	double xc, yc, zc, rc, theta;
	int fi, fj, fk, fl; 
	int ne, nv, nf, noj;

	dVECTOR ec;
	long osize;
	p3DObjL objr;
	p3DVert *varr;
	p3DFace ft;

	nv = (xm->pnts + 1) * (xm->segs + 1);
	nf = xm->pnts * xm->segs * 2;
	ne = 3 * xm->pnts * xm->segs + xm->pnts + xm->segs;

	osize = (nf * _3DFACESIZE * 5 / 4) + (nv * _3DVERTSIZE)
		+ (ne * _3DSEGSIZE * 5 / 2) + _3DOBJSIZE + _3DOBEXSIZE;


	if (osize < _3DOBJSIZEMAX) {

		objr = AddObj3D(pobj, -1, 1);  

		if (objr != NULL) {

		pobj->mem = pobj->mem + osize;
		noj = 1; 

		varr = (p3DVert *) malloc(
			(xm->segs+1)*(xm->pnts+1)*sizeof(p3DVert));
		
		ec = xm->ctr; 

			// Start by generating new vertices
		for (fi = 0; fi < xm->segs; fi++) {
			theta = fi * 2 * PI / xm->segs;
			for (fj = 0; fj < xm->pnts; fj++) {
				rc = xm->rad + xm->x[fj] * xm->wid;
				zc = xm->y[fj] * xm->wid;
				xc = rc * cos(theta);
				yc = rc * sin(theta);
				varr[fi*(xm->pnts+1)+fj] = AddVert3D(objr, dNULLV);
				varr[fi*(xm->pnts+1)+fj]->v.vx = xc + ec.vx;
				varr[fi*(xm->pnts+1)+fj]->v.vy = yc + ec.vy;
				varr[fi*(xm->pnts+1)+fj]->v.vz = zc + ec.vz;
			}
			varr[fi*(xm->pnts+1)+xm->pnts] = varr[fi*(xm->pnts+1)];
		}
		for (fj = 0; fj < xm->pnts + 1; fj++) 
			varr[(xm->pnts+1)*xm->segs+fj] = varr[fj]; 
				// now do faces
		for (fi = 0; fi < xm->segs; fi++) {
			for (fj = 0; fj < xm->pnts; fj++) {
				fk = fi * (xm->pnts + 1);
				fl = (fi + 1) * (xm->pnts + 1);
				ft = AddFace3D(objr);					
				ft->v[0] = varr[fk+fj];
				ft->v[1] = varr[fl+fj];
				ft->v[2] = varr[fl+fj+1];
				ft->back = 0;
				ft->rgb = xm->fcol[fj];

				ft = AddFace3D(objr);					
				ft->v[0] = varr[fk+fj];
				ft->v[1] = varr[fl+fj+1];
				ft->v[2] = varr[fk+fj+1];
				ft->back = 0;
				ft->rgb = xm->fcol[fj];
			}
		}	
		   
		objr->center = ec;

		printf("\n Extrude ok?  verts/faces/edges = %d/%d/%d \n", 
					nv, nf, ne);
		fflush(stdout);

		return (objr);
	}
	}	
	return (NULL);
}

// basic terrain stuff

float rndf(float fr) {
	float fi;

	fi = fr * (((float) rand()) / RAND_MAX - 0.5);

	return (fi);
}

double rndd(double fr) {
	double fi;

	fi = fr * (((double) rand()) / RAND_MAX - 0.5);

	return (fi);
}

int rndi(int ir) {
	float fr;

	fr = ((float) ir) * rand() / RAND_MAX;
	return (int) fr;
}

int htrand(int rarg, int varf)
{   
	int tempf,tempg;
	
	tempf = rand() - (RAND_MAX/2);
	if (rarg<1) rarg = 1;
	if (varf<1) varf = 1;
	tempg = RAND_MAX/(rarg * varf * 2);
	return (tempf/tempg);	
}

int fracht(int vf1, int vf2, int vf3, int vf4, int vf5, int vf6) { 
		// DEFAULTS for these....
   // Corner Scale 	vf1 = 4
   // Countdown Scale 	vf2 = 0
   // Countup Scale 	vf3 = 0
   // Partition Scale 	vf4 = 2
   // Up-Down Offset	vf5 = 1
   // Smoothing Scale 	vf6 = 2

   int Pt = ARRSIZ;
   int Tdz, Tdv, Tdd;
   int x, y, Ht, peak, vall;
   int inc, i, j;
   int hi, lo, noplot, water;

   water = 0;
  
   peak = -RAND_MAX; vall = RAND_MAX;

   htarr[0][0] = htrand(vf1 * ARRSIZ/8,1);
   htarr[0][Pt] = htrand(vf1 * ARRSIZ/8,1);
   htarr[Pt][0] = htrand(vf1 * ARRSIZ/8,1);
   htarr[Pt][Pt] = htrand(vf1 * ARRSIZ/8,1);


   for (hi=1, lo=0; hi<=ARRSIZ/2; hi=hi*2, lo++);
   hi = lo * vf2 + 1; 
   lo = 1; 

   while (Pt>=2)
   {
   	Pt = Pt / 2; hi = hi - vf2; lo = lo + vf3;
   	for (x = Pt; x <= ARRSIZ-Pt; x = x + 2 * Pt)
   	{
   		for (y = Pt; y <= ARRSIZ-Pt; y = y + 2 * Pt)
		{
	   		Ht = htarr[x-Pt][y-Pt];
	   		Ht += htarr[x-Pt][y+Pt];
	   		Ht += htarr[x+Pt][y-Pt];
	   		Ht += htarr[x+Pt][y+Pt];
	   		Ht = Ht/4;
	   		if (Pt>=vf6) htarr[x][y] = Ht +
				 htrand(vf4 * Pt/8, hi + lo - vf5);
	   		else htarr[x][y] = Ht;
	   		if (htarr[x][y] > peak) peak = htarr[x][y];
           		if (htarr[x][y] < vall) vall = htarr[x][y];	   		    
	   	}
   	}
   	for (x = 0; x <= ARRSIZ; x += Pt)
   	{
   		if (!(x % (Pt * 2))) Tdz = Pt;
   		else Tdz = 0;
   		for (y = Tdz; y <= ARRSIZ; y += 2*Pt)
   		{
   			Ht = 0; Tdd = 0; Tdv = 4;
   			if (x!=0) 	Ht += htarr[x-Pt][y]; else Tdd++;
   			if (x!=ARRSIZ)  Ht += htarr[x+Pt][y]; else Tdd++;
   			if (y!=0) 	Ht += htarr[x][y-Pt]; else Tdd++;
   			if (y!=ARRSIZ)  Ht += htarr[x][y+Pt]; else Tdd++;
   			if (Tdd) Tdv=3;
   			if (Pt >= vf6) htarr[x][y] = (Ht/Tdv)
				+ htrand(vf4 * Pt/8, hi + lo - vf5);
   			else htarr[x][y] = (Ht/Tdv);
   			if (htarr[x][y] > peak) peak = htarr[x][y];
           		if (htarr[x][y] < vall) vall = htarr[x][y];
   		}
   	}
   }
   
   if (water<vall) water=vall+(peak-vall)/10;
   return (1);
}

#endif
// end of 3dext.c
