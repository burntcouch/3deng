//
// 3dnav.C  - v0.44 (1/19/01)
//
// Ship navigation stuff
//
// 
//
// 01/15/2001 
// 01/17/2001  - Double precision conversion
// 01/19/2001  - Dynamic object conversion DONE!
// 01/20/2001  - Doing some fixing, setting up ship types
// 05/11/2003  - Updated this for linked list object conversion
//

#ifndef _3DNAV
#define _3DNAV 1

#define _ORBMAXSEGS 100
#define _ORBMAXMASS 10

#define _SCOOTERTYPE 0
#define _RAMMERTYPE 1
#define _JETSIMTYPE 2

struct _3dship;
struct _s3dnav;

typedef struct _s3dnav {
	dVECTOR cp;			// current position
	dVECTOR cv;			// current velocity
	dVECTOR ca;			// current accel.
	p3DObjL omass[_ORBMAXMASS];	// obj#'s with affecting masses
	double ntime;			// nav time chronometer
	int dockf;			// docking flag
	int collf;			// collision alarm!
	p3DObjL conum;			// object to run into
	p3DFace cface;			// face am going to run into
	int dflag;			// show docking face?
	p3DObjL donum;			// docking target object
	p3DFace dface;			// docking target face#
	dVECTOR dp;			// docking target position
	dVECTOR odp;			// docking target old position
	dVECTOR dv; 			// docking target vel
	int gflag;			// gunsight on?
	p3DFace gface;			// gunsighted face#
	p3DObjL gonum;			// gunsighted obj#
	dVECTOR gtp;			// g'd face position
	dVECTOR ogtp;			// g'd face old position
	dVECTOR gtv;			// g'd face velocity
	int nearf;			// near faces displayed?		
	int orbtf;			// orbit track on?
	int orbsegs;			// orbit track segments
	int orbtmax;			// orbit track time limit
	double orbt[_ORBMAXSEGS];	// orbit time stamp at seg#
	dVECTOR orbp[_ORBMAXSEGS];	// orbit position at seg#
	dVECTOR orbv[_ORBMAXSEGS];	// orbit vel at seg#
} _3DNav;

typedef _3DNav *p3DNav;

struct _vscooter;

typedef struct _vscooter {
	int axf;		// acceleration on?
	int thrt;		// throttle setting 0-100
	double fisp;		// kg fuel used per N-sec
	double maxa;		// max acceleration
//	will be a pointer here for keyb/mouse/screen interface
	//NOTE: current ax = (thrt/100) * maxa
	// fuel used = fisp * ax * mass * time
} _VScooter;

typedef _VScooter pVScooter;

union _sctrl;

typedef union _sctrl {
	_VScooter Sc;
//	_RamCtrl R;     to be added soon
//	_BadGuy  EA;	Evil Alien ship!
} _SCtrl;

typedef _SCtrl *pSCtrl;

typedef struct _3dship {
	int stype;			// for future
	p3DNav nav;			// pointer to nav structure
	double mass;			// ship mass kg
	double fuel;			// fuel mass kg
	double shipr;			// ship size (offset for docking)
	pSCtrl ctrl;			// pointer to a ctrl union
} _3DShip;

typedef _3DShip *p3DShip;

// FUNCTION PROTOTYPES

int N3D_GetGunS(p3DNav nav);

int N3D_DrawGunS(p3DEnv tden, p3DDisp disp, p3DNav nav);

int N3D_DrawNear(p3DEnv tden, p3DDisp disp);

int N3D_DrawDock(p3DEnv tden, p3DDisp disp, p3DNav nav);

int N3D_UpdGunS(p3DNav nav);

int N3D_Grav(p3DNav nav, double dt, int otf); 

int N3D_ShipEnv(p3DNav nav, p3DEnv tden);

// FUNCTION DEFINITIONS - That you almost deleted, stupid..

int N3D_UpdGunS(p3DNav nav) {

	dVECTOR tfva, tfvb, tfvc;
	p3DObjL objr;
	p3DFace fc;

	objr = nav->gonum;
	fc = nav->gface;

	nav->ogtp = nav->gtp;
	tfva = fc->v[0]->v;
	tfvb = fc->v[1]->v;
	tfvc = fc->v[2]->v;
	nav->gtp = dCenter(tfva, tfvb, tfvc);
	nav->gtp = dvAdd(nav->gtp, dvScal(nav->cp, -1));
	nav->gtv = dvAdd(nav->cv, 
			dvScal(FaceVel3D(objr, fc), -1));

	if (nav->dflag) {
		objr = nav->donum;
		fc = nav->dface;
		nav->odp = nav->dp;
		tfva = fc->v[0]->v;
		tfvb = fc->v[1]->v;
		tfvc = fc->v[2]->v;
		nav->dp = dCenter(tfva, tfvb, tfvc);
		nav->dp = dvAdd(nav->dp, dvScal(nav->cp, -1));
		nav->dv = dvAdd(nav->cv, 
			dvScal(FaceVel3D(objr, fc), -1));
	}

	return 1;
}


int N3D_GetGunS(p3DNav nav) {

	dVECTOR tfva, tfvb, tfvc;
	p3DObjL objr;
	p3DFace fc;
	
	objr = nav->gonum;
	fc = nav->gface;
	
	nav->gtv = dvAdd(nav->cv, dvScal( 
			FaceVel3D(objr, fc), -1));

	tfva = fc->v[0]->v;
	tfvb = fc->v[1]->v;
	tfvc = fc->v[2]->v;
	nav->gtp = dCenter(tfva, tfvb, tfvc);
	nav->gtp = dvAdd(nav->gtp, dvScal(nav->cp, -1));
	nav->ogtp = nav->gtp;

	return 1;
}

int N3D_DrawNear(p3DEnv tden, p3DDisp disp) {

	int i;
	dVECTOR fp, fv;
	_XYCrd gt1;
	long int gsc;

	gsc = gl_rgbcolor(disp->ncol.r, disp->ncol.g, disp->ncol.b);

	i = 0;
	while (tden->nface[i] != NULL) {
		fp = dvAdd(tden->nfp[i], dvScal(tden->opos, -1));
		fv = dvAdd(tden->nfv[i], dvScal(tden->ovel, -1));
		gt1 = DrawFW3D(tden, tden->nface[i]);
		if (gt1.x != -1) {
			gl_colorfont(8, 8, gsc, disp->font);
			gl_printf(gt1.x, gt1.y,"R %6.5f\n",
				dvMag(fp)/1000);
			gl_printf(-1,-1,"V %3.5f ", 
				 dvMag(fv));
		}
		i++;
	}
	return 1;
}

int N3D_DrawDock(p3DEnv tden, p3DDisp disp, p3DNav nav) {
	_XYCrd gt1;
	long int gsc;

	gsc = gl_rgbcolor(disp->dcol.r, disp->dcol.g, disp->dcol.b);
	gt1 = DrawFW3D(tden, nav->dface);
	if (gt1.x != -1) {
		gl_colorfont(8, 8, gsc, disp->font);
		gl_printf(gt1.x, gt1.y,"R %6.5f\n",
			dvMag(nav->dp)/1000);
		gl_printf(-1,-1,"V %3.5f ", 
			 dvMag(nav->dv));
	}
	return 1;
}


int N3D_DrawGunS(p3DEnv tden, p3DDisp disp, p3DNav nav) {

	_XYCrd gt1;
	long int gsc;

	gsc = gl_rgbcolor(disp->gcol.r, disp->gcol.g, disp->gcol.b);
	gt1 = DrawFW3D(tden, nav->gface);
	if (gt1.x != -1) {
		gl_colorfont(8, 8, gsc, disp->font);
		gl_printf(gt1.x, gt1.y,"R %6.5f\n",
			dvMag(nav->gtp)/1000);
		gl_printf(-1,-1,"V %3.5f ", 
			 dvMag(nav->gtv));
	}
	return 1;
}

int N3D_Grav(p3DNav nav, double dt, int otf) {

	int j;

	dVECTOR acc, ctro, ctri;
	double fr, mass;
	p3DObjEX pobx;	

	j = 0; 
	acc = dNULLV;
	ctro = nav->cp;

	while (nav->omass[j] != NULL) {
		pobx = nav->omass[j]->xdat;
		if (pobx != NULL) {
			mass = pobx->mass;
			ctri = dvAdd(ctro, dvScal(nav->omass[j]->center, -1));
			fr = pow(dvMag(ctri), 3.0);
			fr = -mass / fr;
			acc = dvAdd(acc, dvScal(ctri, fr));
		}
		j++;
	}
	nav->ca = acc;
	nav->cv = dvAdd(nav->cv, dvScal(acc, dt));
	nav->cp = dvAdd(nav->cp, dvScal(nav->cv, dt));

	return 1;

} 



int N3D_ShipEnv(p3DNav nav, p3DEnv tden) {

	tden->opos = nav->cp;
	tden->ovel = nav->cv;
	return 1;
}


#endif
//  End of 3dnav.c
