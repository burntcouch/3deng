//
// 3dobjm.c  - v0.46L (5/11/03)
//
//  Object management routines for Pat's 3D Engine (P3D)
//
//  Some code here from 12/23/00
//  but...  Started modularizing on 1/15/2001
//  Double precision conversions 1/17/2001
//  1/19/2001 - Dynamic object stuff done
//	5/11/03 - Gutting and starting over after linked-list re-write
//	
//
#ifndef _3DOBJM
#define _3DOBJM 1

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _VECTORS
#include "mylib/vectors.c"
#endif

#ifndef PI
#define PI 3.1415926535898
#endif

// FUNCTION PROTOTYPES


void DumpObjA(p3DObjA obj);

void DumpObj(p3DObjL obji);

void DumpObjEx(p3DObjEX xda);


	// Write Object Array structure to disk
int ObjAWrite(p3DObjL dobj, FILE *ofptr);

	// Read Object Array structure from disk
int ObjARead(p3DObjL dobj, FILE *ofptr);

	// Write Raw Object structure to disk
int RObjWrite(p3DObjL drobj, FILE *rfptr);

	// Read Raw Object structure from disk
int RObjRead(p3DObjL drobj, FILE *rfptr);


	//General purpose Object Array - Raw Object utility
int MngObjA3D(p3DObjL obja, int mode);
	// Mode 0: append nobj to obja as new object
	// Mode 1: replace onum in obja with nobj
	// Mode 2: add nobj to onum in obja
	// Mode 3: delete onum from obja (nobj should be NULL)
	// Mode 4: pull nobj from onum in obja
	// 	may be more yet? Call disk stuff from here?


// FUNCTION DEFINITIONS

void DumpObjA(p3DObjA obj) {

	int i = 0;
	p3DObjL obja;

	if (obj == NULL) printf("\nObject assemblage is NULL.\n");
	else {
		obja = obj->fo;
		while (obja != NULL) {
			printf("\nDistance scale: %9.5f meters", obj->oscal);
			printf("\nPointer to first object: %ld", obj->fo);
			printf("\nPointer to last object : %ld", obj->lo);
			printf("\nObject assemblage type : %d\n", obj->atype);
			DumpObj(obja);
			i++;
			obja = obja->nx;
		}
		printf("\nObject list has %d members.\n", i);
	}
	return;
}

void DumpObj(p3DObjL obji){
	p3DVert vt;
	p3DFace ft;
	int vc = 0, fc = 0;

	if (obji == NULL) printf("\nObject is NULL.\n");
	else {
		printf("\n---------------");
		printf("\nObject #%d ----", obji->onum);
		printf("\n---------------");
		printf("\nPtr to this object: %ld", obji);
		printf("\nPtr to next object: %ld", obji->nx);
		printf("\nPtr to parent object: %ld", obji->atch); 

		printf("\nCenter vector  : %12.5f(x) %12.5f(y) %12.5f(z)",
			obji->center.vx, obji->center.vy, obji->center.vz);
		printf("\nVelocity vector: %12.5f(x) %12.5f(y) %12.5f(z)",
			obji->vel.vx, obji->vel.vy, obji->vel.vz);
		printf("\nRotation rates : %12.5f(x) %12.5f(y) %12.5f(z)",
			obji->rot.vx, obji->rot.vy, obji->rot.vz);
		printf("\nFirst rot axis : %12.5f(x) %12.5f(y) %12.5f(z)",
			obji->rox[0].vx, obji->rox[0].vy, obji->rox[0].vz);
		printf("\n2nd rot axis   : %12.5f(x) %12.5f(y) %12.5f(z)",
			obji->rox[1].vx, obji->rox[1].vy, obji->rox[1].vz);
		printf("\n3rd rot axis   : %12.5f(x) %12.5f(y) %12.5f(z)",
			obji->rox[2].vx, obji->rox[2].vy, obji->rox[2].vz);
		printf("\nLumin: %6.3f    Refl: %5.3f   Lite-flag: %d",
			obji->lumin, obji->refl, obji->lflag);
		switch (obji->df) {
			case 1: {printf("\n\n  Non-degenerate object.\n"); break;}
			case 2: {printf("\n\n  Degenerate object."); 
				printf("\n     Size: %12.5f", obji->dsize);
				printf("\n     Color: r-%d  g-%d  b-%d\n",
					obji->dcol.r, obji->dcol.g, obji->dcol.b);
				break;}
			case 0:
			default: {printf("\n\n  Object type undefined.\n"); break;}
		}
		if (obji->xdat == NULL) printf("\n No extented data defined.\n");
		else DumpObjEx(obji->xdat);
		if (obji->v == NULL) 
			printf("\n Vertexes not defined for this object.\n");
		else {
			printf("\n Ptr to first Vertex: %ld", obji->v);
			printf("\n Ptr to last Vertex: %ld\n", obji->vx);
			vt = obji->v;
			while (vt != NULL) {
				printf("\n   VP%ld->  (x)%12.5f  (y)%12.5f  (z)%12.5f ",
					vt, vt->v.vx, vt->v.vy, vt->v.vz);
				if (vt->seg == NULL) printf(" No segs.");
				else printf(" Seglist:");
					// Don't want to deal with this right now 
				vc++;
				vt = vt->nx; 
			}
			printf("\n\n%d verts. defined for this object.\n", vc); 
		}
		if (obji->f == NULL) 
			printf("\n Faces not defined for this object.\n");
		else {
			printf("\n Ptr to first Face: %ld", obji->f);
			printf("\n Ptr to last Face: %ld\n", obji->fx);
			ft = obji->f;
			while (ft != NULL) {
				printf("\n    F#%5d-> v1=%ld  v2=%ld  v3=%ld - RGB %d/%d/%d"
					   " - b:%d", fc, ft->v[0], ft->v[1], ft->v[2], ft->rgb.r,
					   ft->rgb.g, ft->rgb.b, ft->back);
				fc++;
				ft = ft->nx; 
			}
			printf("\n\n%d faces defined for this object.\n", fc); 
		}

	}
	return;
}

void DumpObjEx(p3DObjEX xda) {
	if (xda == NULL) printf("\nExtended data not defined.\n");
	else {
		printf("\n  Extended data for object %d.---", xda->onum);

		printf("\n------------------------------------\n");
	}
	return;


}



#endif

//  End of 3DOBJM.C
