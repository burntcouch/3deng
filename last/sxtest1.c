//
// sxtest1.c
//
// testing various sections of new linked-list memory model
// for 3D engine v0.46L
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vga.h>
#include <vgagl.h>

#include "mylib/vectors.c"
#include "3ddata.c"
#include "3dobjm.c"
#include "3ddefs.c"
#include "3dext.c"

int main(int argc, char *argv[]);

int main(int argc, char *argv[]) {

	p3DObjA oas = NULL;
	p3DObjL o2 = NULL;
	p3DObjEX xd = NULL; 

	int mi, mj, mk;
	dVECTOR v1, v2;

	p3DVert vi, vj, va[10];
	p3DFace fi, fj;


	v1 = dNULLV;

	oas = InitObjA3D(0);

	o2 = AddObj3D(oas, -1, 1);  //new objlist, no xdata, normal object

	v2 = dKunit;
	va[0] = AddVert3D(o2, v2);
	v2 = dvScal(dKunit, -1);
	va[1] = AddVert3D(o2, v2);
	v2 = dIunit;
	va[2] = AddVert3D(o2, v2);
	v2 = dvScal(dIunit, -1);
	va[3] = AddVert3D(o2, v2);
	v2 = dJunit;
	va[4] = AddVert3D(o2, v2);
	v2 = dvScal(dJunit, -1);
	va[5] = AddVert3D(o2, v2);
	fi = AddFace3D(o2);	
	fi->v[0] = va[0]; fi->v[1] = va[2]; fi->v[2] = va[4];
	fi = AddFace3D(o2);
	fi->v[0] = va[0]; fi->v[1] = va[4]; fi->v[2] = va[3];
	fi = AddFace3D(o2);
	fi->v[0] = va[0]; fi->v[1] = va[3]; fi->v[2] = va[5];
	fi = AddFace3D(o2);
	fi->v[0] = va[0]; fi->v[1] = va[5]; fi->v[2] = va[2];
	fi = AddFace3D(o2);	
	fi->v[0] = va[1]; fi->v[1] = va[4]; fi->v[2] = va[2];
	fi = AddFace3D(o2);
	fi->v[0] = va[1]; fi->v[1] = va[3]; fi->v[2] = va[4];
	fi = AddFace3D(o2);
	fi->v[0] = va[1]; fi->v[1] = va[5]; fi->v[2] = va[3];
	fi = AddFace3D(o2);
	fi->v[0] = va[1]; fi->v[1] = va[2]; fi->v[2] = va[5];

	PartObj3D(o2, 2, 1, 0, 0.0, 0.0);

	v2 = dNULLV;
	v2.vx = v2.vx + 100;
	v2.vy = v2.vy + 5;

	o2 = DegenObj3D(oas, 10.0, v2); // add an object, with xdata, degenerate
	InitObjXDet(o2, o2->onum);

	v2.vy = v2.vy - 25;
	
	o2 = SphereObj3D(oas, 10.0, 1, v2, 0, 0.0, 0.0); 
		// add a spherical object, radius 10, 1 level, center v2, no randomness 

	DumpObjA(oas);

	printf("\n");

}


// end of sxtest1.c

