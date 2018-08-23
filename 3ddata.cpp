
//
// Pat's 3d data structure definitions 
//
//  3DDATA.C - v0.47 (3/27/06)
//
//  Originally in 3ddefs.c v0.3 (1/13/01)
//
// Started 12/21/00
//
// Remove some stuff to vectors.c, added some
// utility functions for object dumping and moving 12/27/00
// 1/3/2001 -- Shading now works...  rgb colors... 
//	    -- new spherical coords. based screen function
// 1/13/2001 - Start on lighting effects
// 1/16/2001 - Added display structure
// 1/17/2001 - Double precision conversion	
// 1/18/2001 - Pointered structures with dynamic memory allocation
// 1/19/2001 - And now it works...  More modularizing today!
// 3/5/2002 - Starting on degenerate objects, multiple dist scales,
//	changing levels of detail, dynamic object management,
//	real orbits
// 5/6/2003 - Rewrite of engine using all dynamic memory and linked lists
// 3/27/2006 - refamiliarizing and starting new work
//

#include "3ddata.h"

// qsort compare functions for z-sorts
//
// using ZSCompS2 instead
int ZSCompS( const void * a, const void * b) {
   	long ia, ib;
	double fa, fb;
	int res;

	ia = *(long *)a;
	ib = *(long *)b;
	fa = ZSort3D[ia].z;
	fb = ZSort3D[ib].z;
	if ((fa - fb) > 0) res = 1;
	else res = - 1;
	if (fa == fb) res = 0; 
	return res;
}

int ZSCompS2( const void * a, const void * b) {
   	_ZS3D ia, ib;
	double fa, fb;
	int res;

	ia = *(_ZS3D *)a;
	ib = *(_ZS3D *)b;
	fa = ia.z;
	fb = ib.z;
	if ((fa - fb) > 0) res = 1;
	else res = - 1;
	if (fa == fb) res = 0; 
	return res;
}

// FUNCTION DEFINITIONS

p3DFace AddFace3D(p3DObjL objr) {

	p3DFace static face;
	p3DFace ntlf;
	
	face = (p3DFace) malloc(sizeof(_3DFace));

	ntlf = objr->fx;
	if (ntlf == NULL)  objr->f = face;  // new object, no faces added yet
	else  ntlf->nx = face;
	objr->fx = face;
	face->obj = objr;
	face->sub = NULL;
	face->v = (p3DVert *) malloc(3 * sizeof(p3DVert));
	face->v[0] = NULL;
	face->v[1] = NULL;
	face->v[2] = NULL;
	face->rgb = sLGrey;
	face->nx = NULL;
	face->back = 0;
	
	return (face);
}

p3DBitmap AddBmp3D(p3DObjL obja, p3DFace fc, p3DVert pv) {

	p3DBitmap static bm;		
	p3DBitmap ntlb;

	bm = (p3DBitmap) malloc(sizeof(_3DBitmap));

	ntlb = obja->bmx;	
	if (ntlb == NULL)  obja->bm = bm;  // new object, no faces added yet
	else  ntlb->nx = bm;

	obja->bmx = bm;
	bm->obj = obja;
	bm->f = fc;
	bm->pov = pv;
	bm->prt = 0.0;
	bm->pix = NULL;
	bm->pscal = 0.01;   // one pixel = 1 cm
	bm->pw = 0;
	bm->ph = 0;
	bm->nx = NULL;

	return (bm);
}

p3DVert AddVert3D(p3DObjL objr, dVECTOR vval) {

	p3DVert static vert;
	p3DVert ntlv;

	vert = (p3DVert) malloc(sizeof(_3DVert));

	ntlv = objr->vx;
	if (ntlv == NULL)  objr->v = vert;  //new object, no vertices yet
	else  ntlv->nx = vert;
	objr->vx = vert;
	vert->obj = objr;
	vert->v = vval;
	vert->nx = NULL;
	vert->seg = NULL;
	return (vert);
}


p3DSeg AddSeg3D(p3DVert v1, p3DVert v2) {

	p3DSeg res = NULL;
	p3DSeg asg;
	p3DSeg static nsg1, nsg2;

	asg = v1->seg;
	if (asg == NULL) {	// no segs for this vert yet so add it..
		nsg1 = (p3DSeg) malloc(sizeof(_3DSeg));
		v1->seg = nsg1;
		nsg1->nx = NULL;
		nsg1->v = v2;
		nsg1->vf = NULL;
		res = nsg1;
	}
	else {
		while (1) {
			if (asg->v == v2) break;  // segment already here
			if (asg->nx == NULL) { // add new seg
				nsg1 = (p3DSeg) malloc(sizeof(_3DSeg));
				asg->nx = nsg1;
				nsg1->nx = NULL;
				nsg1->v = v2;
				nsg1->vf = NULL;
				res = nsg1;
				break;
			}
			else asg = asg->nx;
		}
	}

	asg = v2->seg;
	if (asg == NULL) {	// no segs for this vert yet so add it..
		nsg2 = (p3DSeg) malloc(sizeof(_3DSeg));
		v2->seg = nsg2;
		nsg2->nx = NULL;
		nsg2->v = v1;
		nsg2->vf = NULL;
		res = nsg2;
	}
	else {
		while (1) {
			if (asg->v == v1) break;  // segment already here
			if (asg->nx == NULL) { // add new seg
				nsg2 = (p3DSeg) malloc(sizeof(_3DSeg));
				asg->nx = nsg2;
				nsg2->nx = NULL;
				nsg2->v = v1;
				nsg2->vf = NULL;
				res = nsg2;
				break;
			}
			else asg = asg->nx;
		}
	}

	return res;
}

void InitObjXDet(p3DObjL objx, long onum) {

	p3DObjEX static pox;

	pox = (p3DObjEX) malloc(sizeof(_3DObjEX));
	objx->xdat = pox;
	pox->onum = onum;
	pox->mass = -1;
	pox->ax = dNULLV;
	pox->rho = 0.0; 
	pox->sht = 0.0;
	pox->amass[0] = NULL;
	pox->orbs = 0;
	pox->orbp[0] = dNULLV;
	pox->orbt[0] = 0.0;	
	
	return;
}


p3DObjA InitObjA3D(int atyp) {
	// .fo, .lo, .atype

	p3DObjA static obja = NULL;

	obja = (p3DObjA) malloc(sizeof(_3DObjA));

	if (obja != NULL) {
		obja->fo = NULL;
		obja->lo = NULL;
		obja->oscal = 1.0;
		obja->mem = sizeof(_3DObjA);
		switch (atyp) {
			case 1: { // NEAR
				obja->atype = 1;
				obja->dmin = 100000;
				obja->dmax = 149.6E9;
				obja->oscal = 149.6E9;
				break;
			}
			case 2: { // FAR
				obja->atype = 2;
				obja->dmin = 149.6E9;
				obja->dmax = 1.0E30;
				obja->oscal = 9.7E15;
				break;
			}
			case 0:
			default: { // LOCAL 
				obja->atype = 2;   // is this right?  did I ever decide what this field is for?
				obja->dmin = -1;
				obja->dmax = 100000;
				break;
			}
		}
	}
	return (obja);
}

p3DObjL AddObj3D(p3DObjA obja, int xf, int otyp) {

	p3DObjL ntlo;
	p3DObjL static nobj = NULL;
	
	nobj = (p3DObjL) malloc(sizeof(_3DObjL));

	if (nobj != NULL) {
		if (obja->fo == NULL) {  // new object list
			nobj->onum = 0;
			obja->fo = nobj;
			obja->lo = nobj;
		}
		else {
			ntlo = obja->lo;
			ntlo->nx = nobj;
			obja->lo = nobj;
			nobj->onum = ntlo->onum + 1;
		}

		switch (xf) {
		   case 1:
			InitObjXDet(nobj, nobj->onum);
			break;
		   case -1: 
			nobj->xdat = NULL;
			break;
		   case 0:
		   default: break;
		}

		nobj->df = otyp; 				// degenerate object or ???
	
		nobj->atch = NULL;
		nobj->bm = NULL;
		nobj->bmx = NULL;
		nobj->nx = NULL;
		nobj->f = NULL;
		nobj->v = NULL;
		nobj->fx = NULL;
		nobj->vx = NULL;

		nobj->center = dNULLV;
		nobj->vel = dNULLV;
		nobj->rot = dNULLV;	
		nobj->rox[0] = dIunit;	
		nobj->rox[1] = dJunit;	
		nobj->rox[2] = dKunit;	
		nobj->lumin = -999;
		nobj->refl = 1.0;	
		nobj->dsize = 0;
		nobj->dcol = sLGrey;
		nobj->tinit = 0;
		nobj->lflag = 0;
	}
	return (nobj);
}

void FreeObj3D(p3DObjL obja, int fmode) {  // and what the f****!!

}

// END OF 3ddata.cpp
