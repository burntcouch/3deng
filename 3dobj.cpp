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

/* object template
class XX {
	XX();
	~XX();
};

XX:XX() {}
XX:~XX() {}
*/

// protos
class Vert3D {};
class Face3D {};
class Seg3D {};
class Obj3D {};
class ObjList {};
class ObjXdat {};

typedef Vert3D *pVert3D;
typedef Face3D *pFace3D;
typedef Seg3D *pSeg3D;
typedef Obj3D *pObj3D;
typedef ObjList *pObjList;
typedef ObjXdat *pObjXdat;

// Vert3D
class Vert3D {
	Vert3D();
	Vert3D(DVector v);
	~Vert3D();

	DVector v;
	DVector * next;
	Obj3D * obj;   // to get to top of vertlist, this->obj->vhead
	Seg3D * seg;

	Seg3D * addseg(Vect3D * nv);
	void move(DVector p, bool abs);  // offset to new position; if abs true, absolute reposition
};

Vert3D::Vert3D();
Vert3D::~Vert3D();

Vert3D::Vert3D(DVector nv) {
	v = nv;
	obj = NULL;
	next = NULL;
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
		newseg1 = *new Seg3D(nv);
		this->seg = newseg1;
		res = newseg1;
	}
	else {
		while (1) {
			if (asg->v == nv) break;  // segment already here
			if (asg->next == NULL) { // if aren't any more segs, add new seg
				newseg1 = *new Seg3D(nv);
				this->seg = newseg1;
				res = newseg1;
				break;
			}
			else asg = asg->next;
		}
	}

	asg = nv->seg;
	if (asg == NULL) { // no segs here, so..
		newseg2 = *new Seg3D(this);
		nv->seg = newseg2;
		res = newseg2;
	}
	else {
		while (1) {
			if (asg->v == this) break;  //seg already there
			if (asg->next == NULL) { // add a new one, then
				newseg2 = *new Seg3D(this);
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

// Face3D
class Face3D {
	Face3D();
	~Face3D();

	Vert3D v[3];		// v0, v1, v2 will be CCW if visible face is pointing toward observer
			//   so that v1-v0, v2-v1, and v0-v2 proceed in that diretion
 			//   cross product (v1-v0) x (v2-v1) will yield the 'normal' 
	Face3D * next;
	Obj3D * obj;		 // to get to top of facelist, this->obj->fhead
	bool visible;		 // default is yes
	Colora rgb;      // color of face with alpha
	DVector curv;  // 'curvature' center of face.  NULL if flat 	

	DVector norm();
	DVector center();

};

Face3D:Face3D() {}
Face3D:~Face3D() {}

DVector Face3D::norm() {
	DVector tv1, tv2, resv;

	tv1 = v[0].diff(v[1]);
	tv2 = v[1].diff(v[2]);
	resv = tv1.cross(tv2);
	resv = resv.mult(1.0 / resv.mag());
	return resv;
}

DVector Face3D::center() {
	DVector tmpv;
	
	tmpv = v[0].add(v[1]);
	tmpv = tmpv.add(v[2]);
	tmpv = tmpv.mult(1 / 3.0);
	return tmpv;
}
// end of Face3D


// Seg3D
class Seg3D {
	Seg3D();
	Seg3D(Vert3D * otherv);
	~Seg3D();

	Vert3D * v;  	// to get to top of seglist, this->v1->seg
	Vert3D * vf;  // don't know what this is for, yet...
	Seg3D * next;	
};

Seg3D:Seg3D() {}
Seg3D:~Seg3D() {}

Seg3D::Seg3D(Vert3D * otherv) {
	v = otherv;
	vf = NULL;
	next = NULL;
}
// end of Seg3D

//
// ObjXdat
//
class ObjXdat {
	ObjXdat();
	~ObjXdat();

	Obj3D * obj;
	double mass;
	Obj3D * amass;
};

ObjXdat:ObjXdat() {
	obj = NULL;
	mass = -1.0;
	amass = NULL;
}

ObjXdat:~ObjXdat() {}

//end of ObjXdat

// Obj3D
class Obj3D {
	Obj3D();
	~Obj3D();

	ObjList * list;  // top of the object list will be this->list->head (or ->tail)
	Obj3D * next;   // NULL if we are at end of list
	pVert3D vhead;
	pVert3D vtail;  // points to last vertex added
	pFace3D fhead;
	pFace3D ftail;  // points to last face added

	ObjXdat * xdat;		// extended 'physics' data
	DVector center;		// current rotational center
	DVector vel;			// current v of object
	DVector ax;				// induced acc of object
	DVector rot;			//rotation PERIOD (not rate) about axes
	DVector rox[3];			//rotational axes 'i', 'j', 'k'
	double lumin;			//luminosity unless <0
	double refl;				// reflectance (default 1)
	int df;					// drawing flags
								// 0 - ignore this object 
								// 1 - normal, 2 - deg.
								// 4 - attached
								// 8 - bitmap
	Colora dcol;			// color of deg. object
	double dsize;			// radius of deg. object
	int lflag;				// Use as a light?

		// other prototypes
	void init_xdat()
	pFace3D addface();
	pVert3D addvert(DVector v);

};

Obj3D::Obj3D() {}
Obj3D::~Obj3D() {}

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
	
	face = *new Face3D;
	flink = this->ftail;
	if (flink == NULL)  this->fhead = face;  // new object, no faces added yet
	else  flink->nx = face;
	this->ftail = face;
	face->obj = this;
	for (int i = 0; i < 3; i++) {
		face->v[i] = *new Vert3D;
		face->v[i] = NULL;
	}
	face->rgb = ltGry;
	face->next = NULL;
	return (face);
}

void Obj3D::init_xdat() {
	xdat = *new ObjXdat();

}
//end of Obj3D

//
// ObjList
//
class ObjList {
	ObjList();
	~ObjList();

	static Obj3D * head;
	static Obj3D * tail;
	static long ocount;

	pObj3D add(int xf, int otype);
};

ObjList::ObjList() {}
ObjList::~ObjList() {}

pObj3D ObjList::addobj( int xf, int otyp ) {

	Obj3D ntlo;
	Obj3D nobj;
	
	nobj = new Obj3D();

	if (nobj != NULL) {
		if (obja->head == NULL) {  // new object list
			obja->head = nobj;
			obja->tail= nobj;
		}
		else {
			ntlo = obja->tail;
			ntlo->next = nobj;
			obja->tail = nobj;
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

		nobj->center = new DVector;
		nobj->vel = new DVector;
		nobj->rot = new DVector;	
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
