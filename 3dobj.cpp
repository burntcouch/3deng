//
// 3dobj.cpp
//
//
// basic 3D objects and data structures

#include <math.h>
#include <iostream>

#ifndef __DVECTOR
#include "vectoro.cpp"
#endif

#ifndef __3DBASE
#include "3dbase.cpp"    // XYCrd, Colora, Trig3D objects
#endif

#ifndef __3DOBJ
#define __3DOBJ 1

#define AMASSMAX 10   // limiting # of gravitational 'effecting masses' in ObjXdat

#include "3dobj.h"
//
//*** Seg3D
//
Seg3D::Seg3D() {}
Seg3D::~Seg3D() {}

Seg3D::Seg3D(pVert3D otherv) {
	v = otherv;
	vf = NULL;
	next = NULL;
}

// end of Seg3D

//*** Vert3D
//

Vert3D::Vert3D() {}
Vert3D::~Vert3D() {}

Vert3D::Vert3D(DVector nv) {
	v = nv;
	obj = NULL;
	next = NULL;
}

DVector Vert3D::add(pVert3D bv) {
	return v.add(bv->v);
}

DVector Vert3D::diff(pVert3D bv) {
	return v.diff(bv->v);
}

double Vert3D::mag() {
	return v.mag();
}

double Vert3D::dot(pVert3D bv) {
	return v.dot(bv->v);
}

DVector Vert3D::cross(pVert3D bv) {
	return v.cross(bv->v);
}

DVector Vert3D::mult(double m) {
	return v.mult(m);
}


void Vert3D::move(DVector p, bool abs=false) {
	if (abs) v = p;
	else v = v.add(p);
}

pSeg3D Vert3D::addseg(pVert3D nv) {
	pSeg3D res = NULL;
	pSeg3D asg;
	pSeg3D newseg1, newseg2;

	asg = this->seg;
	if (asg == NULL) {	// no segs for this vert yet so add it..
		newseg1 = new Seg3D(nv);
		this->seg = newseg1;
		res = newseg1;
	}
	else {
		while (1) {
			if (asg->v == nv) break;  // segment already here
			if (asg->next == NULL) { // if aren't any more segs, add new seg
				newseg1 = new Seg3D(nv);
				this->seg = newseg1;
				res = newseg1;
				break;
			}
			else asg = asg->next;
		}
	}

	asg = nv->seg;
	if (asg == NULL) { // no segs here, so..
		newseg2 = new Seg3D(this);
		nv->seg = newseg2;
		res = newseg2;
	}
	else {
		while (1) {
			if (asg->v == this) break;  //seg already there
			if (asg->next == NULL) { // add a new one, then
				newseg2 = new Seg3D(this);
				nv->seg = newseg2;
				res = newseg2;
				break;
			}
			else asg = asg->next;
		}
	}
	return res;
}
// end of Vert3D

//
//*** Face3D
//

Face3D::Face3D() {}
Face3D::~Face3D() {}

DVector Face3D::norm() {
	DVector tv1, tv2, resv;

	tv1 = v[0]->diff(v[1]);
	tv2 = v[1]->diff(v[2]);
	resv = tv1.cross(tv2);
	resv = resv.mult(1.0 / resv.mag());
	return resv;
}

DVector Face3D::center() {
	DVector tmpv;
	
	tmpv = v[0]->add(v[1]);
	tmpv = tmpv.add(v[2]->v);
	tmpv = tmpv.mult(1 / 3.0);
	return tmpv;
}
// end of Face3D


//
//*** ObjXdat
//

ObjXdat::ObjXdat() {
	obj = NULL;
	mass = -1.0;
	amass[0] = NULL;
}

ObjXdat::ObjXdat(pObj3D pobj) {
	obj = pobj;
	mass = -1.0;
	amass[0] = NULL;
}

ObjXdat::~ObjXdat() {}

//end of ObjXdat

//*** Obj3D
//

Obj3D::Obj3D() {}
Obj3D::~Obj3D() {}

void Obj3D::init_xdat() {
	this->xdat = new ObjXdat();
	this->xdat->obj = this;
	this->xdat->mass = -1.0;
	this->xdat->amass[0] = NULL;
}

pVert3D Obj3D::addvert(DVector v) {
	pVert3D vert;
	pVert3D nlink;

	vert = new Vert3D(v);
	nlink = this->vtail;
	if (nlink == NULL) this->vhead = vert;
	else nlink->next = vert;
	this->vtail = vert;
	vert->obj = this;
	vert->next = NULL;
	vert->seg = NULL;
	return vert;
}

pFace3D Obj3D::addface() {
	pFace3D face;
	pFace3D flink;
	
	face = new Face3D;
	flink = this->ftail;
	if (flink == NULL)  this->fhead = face;  // new object, no faces added yet
	else  flink->next = face;
	this->ftail = face;
	face->obj = this;
	for (int i = 0; i < 3; i++) {
		face->v[i] = new Vert3D;
	}
	face->rgb = ltGry;
	face->next = NULL;
	return (face);
}

//end of Obj3D

//
//*** ObjList
//

ObjList::ObjList() {}
ObjList::~ObjList() {}

pObj3D ObjList::addobj( int xf, int otyp ) {

	pObj3D nlink;
	pObj3D nobj;
	
	nobj = new Obj3D;

	if (nobj) {
		if (head == NULL) {  // new object list
			head = nobj;
			tail= nobj;
		}
		else {
			nlink = tail;
			nlink->next = nobj;
			tail = nobj;
		}

		switch (xf) {
		   case 1: nobj->init_xdat();	break;
		   case -1: nobj->xdat = NULL; break;   // extended data link!!!!  WORK ON THIS TONIGHT!
		   case 0:
		   default: break;
		}

		nobj->df = otyp; 				// degenerate object or ???
		nobj->next = NULL;
		nobj->fhead = NULL;
		nobj->vhead = NULL;
		nobj->ftail = NULL;
		nobj->vtail = NULL;

		nobj->center = DVector();
		nobj->vel = DVector();
		nobj->rot = DVector();	
		nobj->rox[0] = DVector('i');	
		nobj->rox[1] = DVector('j');	
		nobj->rox[2] = DVector('k');	
		nobj->lumin = -999;
		nobj->refl = 1.0;	
		nobj->dsize = 0;
		nobj->dcol = ltGry;
		nobj->lflag = false;
	}
	return (nobj);
}
// end of ObjList

#endif
// end of 3dobj.cpp
