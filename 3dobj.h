//
// 3dobj.h
//
//
// basic object class defs (see 3dobj.cpp as well)
//

/* object template
class XX {
	XX();
	~XX();
};

typedef XX *pXX;

XX::XX() {}
XX::~XX() {}
*/

// forward decs
class Seg3D;
class Vert3D;
class Face3D;
class Obj3D;
class ObjXdat;
class ObjList;

typedef Seg3D *pSeg3D;
typedef Vert3D *pVert3D;
typedef Face3D *pFace3D;
typedef Obj3D *pObj3D;
typedef ObjXdat *pObjXdat;
typedef ObjList *pObjList;

//*** Seg3D
//
class Seg3D {
	public:
		Seg3D();
		Seg3D(Vert3D *otherv);
		~Seg3D();

		Vert3D *v;  	// to get to top of seglist, this->v1->seg
		Vert3D *vf;  // don't know what this is for, yet...
		Seg3D *next;	
};

//*** Vert3D
//
class Vert3D {
	public:
		Vert3D();
		Vert3D(DVector v);
		~Vert3D();

		DVector v;
		Vert3D *next;
		Obj3D *obj;   // to get to top of vertlist, this->obj->vhead
		Seg3D *seg;

		pSeg3D addseg(pVert3D nv);
		void move(DVector p, bool abs);  // offset to new position; if abs true, absolute reposition
			// vector function redirects
		DVector	add(pVert3D bv);
		DVector diff(pVert3D bv);
		double mag();
		double dot(pVert3D bv);
		DVector mult(double m);
		DVector cross(pVert3D bv);
};

//
//*** Face3D
//
class Face3D {

	public:
		Face3D();
		~Face3D();

		Vert3D *v[3];		// v0, v1, v2 will be CCW if visible face is pointing toward observer
				//   so that v1-v0, v2-v1, and v0-v2 proceed in that diretion
	 			//   cross product (v1-v0) x (v2-v1) will yield the 'normal' 
		Face3D *next;
		Obj3D *obj;		 // to get to top of facelist, this->obj->fhead
		bool visible;		 // default is yes
		Colora rgb;      // color of face with alpha
		DVector curv;  // 'curvature' center of face.  NULL if flat 	

		DVector norm();
		DVector center();

};

//
//*** ObjXdat
//
class ObjXdat {
	public:
		ObjXdat();
		ObjXdat(pObj3D nobj);
		~ObjXdat();

		pObj3D obj;
		double mass;
		pObj3D amass[AMASSMAX];
};

//*** Obj3D
//
class Obj3D {
	public:
		Obj3D();
		~Obj3D();

		ObjList *list;  // top of the object list will be this->list->head (or ->tail)
		Obj3D *next;   // NULL if we are at end of list
		pVert3D vhead;
		pVert3D vtail;  // points to last vertex added
		pFace3D fhead;
		pFace3D ftail;  // points to last face added

		ObjXdat *xdat;		// extended 'physics' data
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
		void init_xdat();
		pFace3D addface();
		pVert3D addvert(DVector v);

};

//
//*** ObjList
//
class ObjList {
	public:
		ObjList();
		~ObjList();

		Obj3D *head;
		Obj3D *tail;
		long ocount;

		pObj3D addobj(int xf, int otyp);
};

// end of 3dobj.h
//
