//
// Pat's 3d definitions
//
//  3ddefs.c v0.4LL (4/6/2003)
//
// Started 12/21/00
//
// Remove some stuff to vectors.c, added some
// utility functions for object dumping and moving 12/27/00
// 1/3/2001 -- Shading now works...  rgb colors... 
//	    -- new spherical coords. based screen function
// 1/13/2001 - Start on lighting effects
// 1/15/2001 - More fun stuff..  Face info..
// 1/16/2001 - Hacked stuff up (modularization)
//		general routines used for gunsighting
// 1/17/2001 - Double precision conversion
// 1/18/2001 - Dynamic object allocation
// 3/5/2002 - Start on degenerate objects, multiple dist. scales
// 5/6/2003 - Newer version of memory allocation, etc. 
// 5/10/03  - Still working on above
// 5/19/03  - All routines working except PartObj

#ifndef _3DDEFS
#define _3DDEFS 1

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vga.h>
#include <vgagl.h>

#ifndef _VECTORS
#include "mylib/vectors.c"
#endif

#ifndef PI
#define PI 3.1415926535898
#endif

#define _ZSGROW 8192

// important structures and macros...

#ifndef _3DDATA
#include "3ddata.c"
#endif


// FUNCTION PROTOTYPES 

	// break integer color into rgb
_3DCol GetRGB(long int lcol, int vcol);

	// rgb to long int
long int PackRGB(_3DCol prgb, int vcol);

	// Get a unit vector from screen coordinates...
dVECTOR ScrUnit3D(p3DEnv tenv, _XYCrd tgt); 

	// Find out if a ray intersects a face
int FInter3D(dVECTOR v1, dVECTOR v2, dVECTOR v3, dVECTOR lv,
	dVECTOR vv, double *ti, dVECTOR *pv); 

	// Alternate to above, polar coordinates
_XYCrd Screen3Dr(p3DEnv tenv, dVECTOR vvert);

	// Fill a triangle with tcol (uses gl_line())
int tricolor3D(_XYCrd c1, _XYCrd c2, _XYCrd c3, long int tcol, int wf);

	// Calc area of triangle on screen in pixels
long int triarea3D(_XYCrd c1, _XYCrd c2, _XYCrd c3);
 
	// Do lighted shading - unlit faces
_3DCol FcShade3D(_3DCol fcol, p3DEnv senv, dVECTOR norm, dVECTOR ctr,
			 float refl);
 
	// Do lighted shading - luminunous faces
_3DCol LtShade3D(_3DCol fcol, p3DEnv senv, dVECTOR norm, dVECTOR ctr,
			 float lum); 

	// Find a face.., return -1 if no intercept, otherwise face#
void RayFace3D(p3DObjA wobj, dVECTOR pv, dVECTOR vv, 
			p3DFace fp, p3DObjL op, double *ETA);

	// Get nearest faces of objects and put in env.
int NearFace3D(p3DObjA wobj, p3DEnv wenv);

	// Draw wireframes of objects
int DrawWire3D(p3DEnv tenv, p3DObjA tobj);

	// Draw wirefram of one face
_XYCrd DrawFW3D(p3DEnv wenv, p3DFace fn);

	// Draw zsorted and shaded objects
int DrawZS3D(p3DEnv tenv, p3DObjA tobj);

	// Renormalize view unit vectors (needed after a rotation)
void Renorm3D(p3DEnv oenv);

	// Partition an object into 4 faces/per face
int PartObj3D(p3DObjL pobj, int plvl, int tflag, 
			int rflag, double rvar, double rv2);

	// Move all objects in structure one time step and update
void ObjMove(p3DObjA mobj, p3DEnv menv);

	// Return velocity of face 
dVECTOR FaceVel3D(p3DObjL obj, p3DFace fc);

	// Rotate all objects
void ObjRot(p3DObjA mobj, double dt); 


// FUNCTION DEFINITIONS 

long int PackRGB(_3DCol prgb, int vcol) {
	long int rcol;
	int zm;

	zm = 256/vcol;	
	rcol = -1;
	switch (vcol) {
		case 64:
			rcol = (prgb.r / (zm*2)) * 32 * vcol;
			rcol += (prgb.g / zm) * 32;
			rcol += prgb.b / (zm * 2);
			break;
		case 32:
		case 256:
			rcol = (prgb.r / zm) * vcol * vcol;
			rcol += (prgb.g / zm) * vcol;
			rcol += prgb.b / zm;
		default: break;
	}

	return rcol;
}

_3DCol GetRGB(long int lcol, int vcol) {
	unsigned char rc, gc, bc;
	_3DCol rcolor;
	int zm;

	zm = 256/vcol;	
	switch (vcol) {
		case 64:
			rc = 2 * zm * lcol / (vcol * 32);
			gc = zm * (lcol - rc * vcol * 32) / 32;
			bc = 2 * zm * (lcol - rc * vcol * 32 - gc * 32);
			break;
		case 32:
		case 256:
			rc = zm * lcol / (vcol * vcol);
			gc = zm * (lcol - rc * vcol * vcol) / vcol;
			bc = zm * (lcol - rc * vcol * vcol - gc * vcol);
		default: break;
	}
	rcolor.r = rc;
	rcolor.g = gc;
	rcolor.b = bc;
	return rcolor;
}

void Renorm3D(p3DEnv oenv) {
	double ffa;
	dVECTOR fva;
	
	ffa = dvMag(oenv->oiv);
	oenv->oiv = dvScal(oenv->oiv, 1 / ffa);
	fva = dvCross(oenv->oiv, oenv->ojv);
	ffa = dvMag(fva);
	oenv->okv = dvScal(fva, 1 / ffa);
	oenv->ojv = dvCross(oenv->okv, oenv->oiv);
}

dVECTOR FaceVel3D(p3DObjL obj, p3DFace fc) {

	dVECTOR fv, ta, tb, tc, cv, cfv, rv;
	double fg, fx, fy, fz;

	fv = obj->vel;
	cv = obj->center;
	ta = fc->v[0]->v;
	tb = fc->v[1]->v;
	tc = fc->v[2]->v;
	cfv = dvAdd(dCenter(ta, tb, tc), dvScal(cv, -1));

	rv.vx = dvDot(cfv, obj->rox[0]);
	rv.vy = dvDot(cfv, obj->rox[1]);
	rv.vz = dvDot(cfv, obj->rox[2]);
	fx = dvDot(fv, obj->rox[0]);
	fy = dvDot(fv, obj->rox[1]);
	fz = dvDot(fv, obj->rox[2]);

	if (fabs(obj->rot.vx) > 0) {
		fg = obj->rot.vx * PI / 180;
		fz = fz - rv.vy * fg / sqrt(rv.vy*rv.vy + rv.vz*rv.vz);
		fy = fy + rv.vz * fg / sqrt(rv.vy*rv.vy + rv.vz*rv.vz);
	}	
	if (fabs(obj->rot.vy) > 0) {
		fg = obj->rot.vy * PI / 180;
		fx = fx - rv.vz * fg / sqrt(rv.vx*rv.vx + rv.vz*rv.vz);
		fz = fz + rv.vx * fg / sqrt(rv.vx*rv.vx + rv.vz*rv.vz);
	}	
	if (fabs(obj->rot.vz) > 0) {
		fg = obj->rot.vz * PI / 180;
		fy = fy - rv.vx * fg / sqrt(rv.vy*rv.vy + rv.vx*rv.vx);
		fx = fx + rv.vy * fg / sqrt(rv.vy*rv.vy + rv.vx*rv.vx);
	}	
	fv.vx = fx * obj->rox[0].vx + fy * obj->rox[1].vx +
			fz * obj->rox[2].vx;
	fv.vy = fx * obj->rox[0].vy + fy * obj->rox[1].vy +
			fz * obj->rox[2].vy;
	fv.vz = fx * obj->rox[0].vz + fy * obj->rox[1].vz +
			fz * obj->rox[2].vz;

	return (fv);
}

void ObjRot(p3DObjA mobj, double dt) {
	int fm, fn;
	double fg;
	dVECTOR fv, fw;
	p3DObjL objr;
	p3DVert tvrt;

	// Do rotations
	objr = mobj->fo;
	while (objr != NULL) {  
		fv = objr->center;
		tvrt = objr->v;
		while ((tvrt != NULL) && (objr->df != 2)) {
			fw = dvAdd(tvrt->v, dvScal(fv,-1));
			if (fabs(objr->rot.vx) > 0) {
				fg = objr->rot.vx * dt;
				fw = dVecRot3D(fw, objr->rox[0], 
					objr->rox[1], fg);	
			}	
			if (fabs(objr->rot.vy) > 0) {
				fg = objr->rot.vy * dt;
				fw = dVecRot3D(fw, objr->rox[1], 
					objr->rox[2], fg);	
			}	
			if (fabs(objr->rot.vz) > 0) {
				fg = objr->rot.vz * dt;
				fw = dVecRot3D(fw, objr->rox[2], 
					objr->rox[0], fg);	
			}	
			tvrt->v = dvAdd(fw, fv);
			tvrt = tvrt->nx;
		}
		objr = objr->nx;
	}
	return;	
} 


int FInter3D(dVECTOR v1, dVECTOR v2, dVECTOR v3, dVECTOR lv,
	dVECTOR vv, double *ti, dVECTOR *pv) {
	
	dVECTOR nv, bv, cv, ppv, ah, bh, ch, ph;
	double vz, lz, dcb, dpb, dpc, rb, rc, rp, r1;
	double ax, ay, px, py, bx, by, cx, cy;
	double m1, b1, m2, b2, rx, ry;
	int res;

	*pv = dNULLV;
	nv = dNorm(v1, v3, v2);

	lz = dvDot(nv, dvAdd(lv, dvScal(v1, -1)));
	vz = dvDot(nv, vv);

	*ti = -lz / vz;
	while (1) {
		if (*ti < 0) { res = 0; break;}

		*pv = dvAdd(lv, dvScal(vv, *ti));
		bv = dvAdd(v2, dvScal(v1, -1));
		rb = dvMag(bv);
		cv = dvAdd(v3, dvScal(v1, -1));
		rc = dvMag(cv);
		ppv = dvAdd(*pv, dvScal(v1, -1));
		rp = dvMag(ppv);
		bh = dvScal(bv, 1 / dvMag(bv));
		ch = dvScal(cv, 1 / dvMag(cv));
		ph = dvScal(ppv, 1 / dvMag(ppv));
		dcb = acos(dvDot(bh, ch));
		dpb = acos(dvDot(ph, bh));
		dpc = acos(dvDot(ph, ch));
		if (dpb > dcb) {res = 0; break;}
		if (dpc > dcb) {res = 0; break;}
		if (rp > rb) {res = 0; break;}
		if (rp > rc) {res = 0; break;}
		if ((rp < rb) && (rp < rc)) { res = 1; break;}
		bh = dvCross(nv, ch);
		ax = dvDot(v1, ch); px = dvDot(*pv, ch);
		ay = dvDot(v1, bh); py = dvDot(*pv, bh);
		bx = dvDot(v2, ch); cx = dvDot(v3, ch);
		by = dvDot(v2, bh); cy = dvDot(v3, bh);
		m1 = (ay - py) / (ax - px);
		m2 = (by - cy) / (bx - cx);
		b1 = ay - ax * m1;
		b2 = by - bx * m2;
		rx = (ay - by + bx * m1 - ax * m2) / (m1 - m2);
		ry = m1 * rx + b1;
		r1 = sqrt(rx * rx + ry * ry);
		if (rp <= r1) res = 1;
		else res = 0;
		break;
	}

	return (res);
}


dVECTOR ScrUnit3D(p3DEnv tenv, _XYCrd tgt) {

	dVECTOR sunit, tv, iv, jv, kv;
	double fx, fy, pp, pt;

	iv = tenv->oiv;
	jv = tenv->ojv;
	kv = tenv->okv;
	
	fx = tenv->hScrn - tgt.x;
	fy = tenv->vScrn - tgt.y;
	pt = fx / tenv->vscal;
	pp = fy / tenv->vscal;

	sunit.vx = cos(pt) * cos(pp);
	sunit.vy = sin(pt) * cos(pp);
	sunit.vz = sin(pp);
	tv = sunit;
	sunit.vx = tv.vx * iv.vx + tv.vy * jv.vx + tv.vz * kv.vx; 
	sunit.vy = tv.vx * iv.vy + tv.vy * jv.vy + tv.vz * kv.vy; 
	sunit.vz = tv.vx * iv.vz + tv.vy * jv.vz + tv.vz * kv.vz; 

	return (sunit);	
}

_XYCrd Screen3Dr(p3DEnv tenv, dVECTOR vvert) {
	_XYCrd scrn;
	double ia, ja, ka, fx, fy;
	double pr, pt, pp, alim, slack;
	dVECTOR tmpv;

	scrn.flag = 0;
	slack = 50;
	alim = 3 * PI/4;

	tmpv = dvAdd(vvert, dvScal(tenv->opos, -1));
	pr = dvMag(tmpv);
	ia = dvDot(tenv->oiv, tmpv);
	ja = dvDot(tenv->ojv, tmpv);
	ka = dvDot(tenv->okv, tmpv);
	pt = atan(fabs(ja/ia));
	if (ia < 0) pt = PI - pt;
	if (ja < 0) pt = -pt;
	pp = asin(ka/pr);
		// Angle limiter for horizontal coord.
	if (pt > alim) pt = alim;
	if (pt < -alim) pt = -alim;
		//
	fx = -tenv->vscal * pt;
	fy = tenv->vscal * pp;
	if (fabs(fx) > tenv->hScrn + slack) {
		if (fabs(fy) > tenv->vScrn + slack) scrn.flag = 1;
	} 
	scrn.x = (int) fx +  tenv->hScrn;
	scrn.y = tenv->vScrn - (int) fy;

	return scrn;
}

long int triarea3D(_XYCrd c1, _XYCrd c2, _XYCrd c3) {

	int fi, lx, rx;
	float s12, s23, s13;
	int tx1, ty1, tx2, ty2, tx3, ty3;
	long int area;

	if (c1.y > c2.y) {ty2 = c1.y; tx2 = c1.x; ty1 = c2.y; tx1 = c2.x;}
	else {ty2 = c2.y; tx2 = c2.x; ty1 = c1.y; tx1 = c1.x;}
	if (c3.y < ty2) { 
		if (c3.y < ty1) {
			ty3 = ty2; tx3 = tx2; ty2 = ty1; tx2 = tx1;
			ty1 = c3.y; tx1 = c3.x;}
		else {ty3 = ty2; tx3 = tx2; ty2 = c3.y; tx2 = c3.x;}}
	else {ty3 = c3.y; tx3 = c3.x;}

	s12 = 1.0 * (tx2 - tx1)/(ty2 - ty1);
	s23 = 1.0 * (tx3 - tx2)/(ty3 - ty2);
	s13 = 1.0 * (tx3 - tx1)/(ty3 - ty1);

	area = 0;

	// calc section from t1 to t2
	for (fi = ty1 + 1; fi < ty2; fi++) {
		lx = tx1 + s12 * (fi - ty1);
		rx = tx1 + s13 * (fi - ty1);
		area += abs(lx - rx);
	}
	// calc section from t2 to t3
	for (fi = ty2; fi <= (ty3- 1); fi++) {
		lx = tx2 + s23 * (fi - ty2);
		rx = tx1 + s13 * (fi - ty1);
		area += abs(lx - rx);
	}

	// return area in pixels	
	return area;
}


int tricolor3D(_XYCrd c1, _XYCrd c2, _XYCrd c3, long int tcol, int wf) {
	int fi, lx, rx;
	float s12, s23, s13;
	int tx1, ty1, tx2, ty2, tx3, ty3;
	int tty1, tty2, tty3;

	// __clipx1, __clipy1, __clipx2, __clipy2 define clip window

		// sort 1 2 3 by y value
	if (c1.y > c2.y) {ty2 = c1.y; tx2 = c1.x; ty1 = c2.y; tx1 = c2.x;}
	else {ty2 = c2.y; tx2 = c2.x; ty1 = c1.y; tx1 = c1.x;}
	if (c3.y < ty2) { 
		if (c3.y < ty1) {
			ty3 = ty2; tx3 = tx2; ty2 = ty1; tx2 = tx1;
			ty1 = c3.y; tx1 = c3.x;}
		else {ty3 = ty2; tx3 = tx2; ty2 = c3.y; tx2 = c3.x;}}
	else {ty3 = c3.y; tx3 = c3.x;}

		// find slopes
	s12 = 1.0 * (tx2 - tx1)/(ty2 - ty1);
	s23 = 1.0 * (tx3 - tx2)/(ty3 - ty2);
	s13 = 1.0 * (tx3 - tx1)/(ty3 - ty1);
	// if wireframe...
	if (wf) {
	gl_line(tx1, ty1, tx2, ty2, tcol); //printf("line 1 OK \n");
	gl_line(tx1, ty1, tx3, ty3, tcol); //printf("line 2 OK \n");
	gl_line(tx2, ty2, tx3, ty3, tcol); //printf("line 3 OK \n");
	}
		// if not wireframe...
	else {
		tty1 = ty1; tty2 = ty2; tty3 = ty3;

		// calc and draw section from t1 to t2
		for (fi = ty1 + 1; fi < ty2; fi++) {
			lx = tx1 + s12 * (fi - tty1);
			rx = tx1 + s13 * (fi - tty1);
			gl_line(lx, fi, rx, fi, tcol);
			//gl_hline(lx, fi, rx, tcol);
			//printf("line%d OK \n", fi);
		}
		// calc and draw section from t2 to t3
		for (fi = ty2; fi <= (ty3- 1); fi++) {
			lx = tx2 + s23 * (fi - tty2);
			rx = tx1 + s13 * (fi - tty1);
			gl_line(lx, fi, rx, fi, tcol);
			//gl_hline(lx, fi, rx, tcol);
			//printf("line%d OK \n", fi);
		}
	}
	// return number of lines drawn	
	return (ty3 - ty1) + 1;
}


_3DCol LtShade3D(_3DCol fcol, p3DEnv senv, dVECTOR norm, dVECTOR ctr,
			 float lum) {
	_3DCol rcol;

	double Mf, fc, c1;
	double AU = 149.6E9;
	int cval;
	dVECTOR cv, cu;

	cv = dvAdd(senv->opos, dvScal(ctr, -1));
	fc = dvMag(cv);
	cu = dvScal(cv, 1 / fc);
	c1 = dvDot(cu, norm);
	if (c1 > 0) {
		Mf = 5 * log10(fc/(AU * c1)) + lum;
	}
	else Mf = (112.0 / senv->lvc) - senv->lvr;

	cval = fcol.r - (int) (senv->lvc * (Mf + senv->lvr));
	if (cval < 1) cval = 1; if (cval>255) cval = 255;
	rcol.r = cval;
	cval = fcol.g - (int) (senv->lvc * (Mf + senv->lvr));
	if (cval < 1) cval = 1; if (cval>255) cval = 255;
	rcol.g = cval;
	cval = fcol.b - (int) (senv->lvc * (Mf + senv->lvr));
	if (cval < 1) cval = 1; if (cval>255) cval = 255;
	rcol.b = cval;

	return (rcol);
}


_3DCol FcShade3D(_3DCol fcol, p3DEnv senv, dVECTOR norm, dVECTOR ctr,
			 float refl) {
	_3DCol rcol;

	double tlum;
	double Mf, Mv, fr, fc;
	int rs, gs, bs;
	double AU = 149.6E9;
	int i, j, cval;
	double c1, c2, cs;
	dVECTOR rv, ru, cv, cu;

	i = 0;	
	fr = 0.0;
	ru = dNULLV; rv = dNULLV;
	cv = dvAdd(senv->opos, dvScal(ctr, -1));
	fc = dvMag(cv);
	cu = dvScal(cv, 1 / fc);
	c1 = dvDot(cu, norm);
	if (c1 > 0) {
		i = 0;
		tlum = 0.0;
		while (1) {
			rv = dvAdd(senv->lo[i]->center, dvScal(ctr, -1));
			fr = dvMag(rv);
			ru = dvScal(rv, 1 / fr);
			c2 = dvDot(ru, norm);
			if (c2 > 0) {
				Mf = 5 * log10((fc * fr)/
				  (refl * AU * AU * c1 * c2))
				  + senv->lo[i]->lumin;
				tlum += pow(10.0, - Mf * 0.4);
			} 		
			else { 
				Mf = (112.0 / senv->lvc) - senv->lvr;
				tlum += pow(10.0, - Mf * 0.4);
			}
			i++;
			if (senv->lo[i] == NULL) break;
		}
		Mf = -2.5 * log10(tlum);
	}
	else Mf = (112.0 / senv->lvc) - senv->lvr;

	cval = fcol.r - (int) (senv->lvc * (Mf + senv->lvr));
	if (cval < 3) cval = 3; if (cval>255) cval = 255;
	rcol.r = cval;
	cval = fcol.g - (int) (senv->lvc * (Mf + senv->lvr));
	if (cval < 3) cval = 3; if (cval>255) cval = 255;
	rcol.g = cval;
	cval = fcol.b - (int) (senv->lvc * (Mf + senv->lvr));
	if (cval < 3) cval = 3; if (cval>255) cval = 255;
	rcol.b = cval;

	return (rcol);
}

_XYCrd DrawFW3D(p3DEnv wenv, p3DFace fn) {
	_XYCrd vc1, vc2, vc3;
	long int vcol;
	double ft1, ft2;
	dVECTOR zv, tva, tvb, tvc;

	tva = fn->v[0]->v;
	tvb = fn->v[1]->v;
	tvc = fn->v[2]->v;
	zv = dCenter(tva, tvb, tvc);
	zv = dvAdd(zv, dvScal(wenv->opos, -1));
	ft1 = dvDot(zv, wenv->oiv);
	if (ft1 > 0) {
		vc1 = Screen3Dr(wenv, tva);
		vc2 = Screen3Dr(wenv, tvb);
		vc3 = Screen3Dr(wenv, tvc);
			// NOTE: am using polar version
		vcol = PackRGB(fn->rgb, wenv->scols);
		tricolor3D(vc1, vc2, vc3, vcol, 1);
		return (vc1);
	}
	else {
		vc1.x = -1;
		vc1.y = -1;
		return (vc1);
	}
}

int DrawWire3D(p3DEnv wenv, p3DObjA wobj) {
	int fi, fj, fk, fcnt = 0;
	_XYCrd vc1, vc2, vc3;
	long int vcol;
	double ft1, ft2;
	dVECTOR zv, tva, tvb, tvc;
	p3DObjL objr;
	p3DFace fc;

	objr = wobj->fo;
	fc = objr->f;
	while (objr != NULL) {
		while (fc != NULL) {
			tva = fc->v[0]->v;
			tvb = fc->v[1]->v;
			tvc = fc->v[2]->v;
			zv = dCenter(tva, tvb, tvc);
			zv = dvAdd(zv, dvScal(wenv->opos, -1));
			ft1 = dvDot(zv, wenv->oiv);
			if (ft1 > 0) {
				vc1 = Screen3Dr(wenv, tva);
				vc2 = Screen3Dr(wenv, tvb);
				vc3 = Screen3Dr(wenv, tvc);
					// NOTE: am using polar version
				vcol = PackRGB(fc->rgb, wenv->scols);
				tricolor3D(vc1, vc2, vc3, vcol, 1);
				fcnt++;
			}
			fc = fc->nx;
		}
		objr = objr->nx;
	}
 
	return fcnt;
}

int NearFace3D(p3DObjA wobj, p3DEnv wenv) {

	int fi, fj, fk, fl;
	double ft1, ft2, ft3;
	dVECTOR pv, vv, tva, tvb, tvc, tv1, tv2;
	p3DObjL objr, op;
	p3DFace fc, fp;

	pv = wenv->opos;
	fl = 0;

	objr = wobj->fo;
	while ((objr != NULL) && (objr->df == 1)) {
		fc = objr->f;
		fp = NULL; ft2 = 1.0E30;
		vv = dvAdd(objr->center, dvScal(wenv->opos, -1));
		wenv->ncu[fl] = vv;
		while(fc != NULL) {
			tva = fc->v[0]->v;
			tvb = fc->v[1]->v;
			tvc = fc->v[2]->v;
			fk = FInter3D(tva, tvb, tvc, pv, vv, &ft1, &tv1); 
			if (fk) {
				if (ft1 < ft2) { 
					fp = fc;
					ft2 = ft1; }
			}
			fc = fc->nx;
		}
		if (fp != NULL) {
			wenv->nonum[fl] = objr;
			wenv->nface[fl] = fp;
			tva = fp->v[0]->v;
			tvb = fp->v[1]->v;
			tvc = fp->v[2]->v;
			wenv->nfp[fl] = dCenter(tva, tvb, tvc);
			wenv->nfv[fl] = FaceVel3D(objr, fp);
			fl++;
		}
	    objr = objr->nx;
	}
	wenv->nonum[fl] = NULL;
	wenv->nface[fl] = NULL;

	return 1;
} 

void RayFace3D(p3DObjA wobj, dVECTOR pv, dVECTOR vv, 
			p3DFace fp, p3DObjL op, double *ETA)
{
	int fi, fj, fk;
	double ft1, ft2, ft3;
	dVECTOR zv, nv, tva, tvb, tvc, tv1, tv2;
	p3DObjL objr;
	p3DFace fc;

	fp = NULL; 
	ft2 = 1.0E30;
	*ETA = 0.0;
	op = NULL;


	objr = wobj->fo;
	while (objr != NULL) {
		fc = objr->f;
		while ((fc != NULL) && (objr->df == 1)) {
			tva = fc->v[0]->v;
			tvb = fc->v[1]->v;
			tvc = fc->v[2]->v;
			fk = FInter3D(tva, tvb, tvc, pv, vv, &ft1, &tv1); 
			if (fk) {
				if (ft1 < ft2) { 
					fp = fc;
					op = objr;
					ft2 = ft1; }
			}
			fc = fc->nx;
		}
		objr = objr->nx;
	}

	*ETA = ft2; 

	return;
}

int DrawZS3D(p3DEnv wenv, p3DObjA wobj) {
	int fj, fk, dcnt = 0;
	_XYCrd vc1, vc2, vc3, vc4;
	long int vcol;
	int vr, vg, vb, vsc, svf, pflag;
	double ft1, ft2, ft3;
	dVECTOR zv, nv, cv, tva, tvb, tvc;

	long int fi, fcnt;

	int shadef;
	p3DObjL objr;
	p3DFace face;
	_3DCol lscol;

	//  use ZSort3D after malloc-ing it....
	//  same number of elements as zf.

	// uncomment this and other stuff here if is too slow...
//	long *zf;    // do I need this?

	long zcnt, fsize;

	int lcnt;

	shadef = wenv->dmode - 1;

	switch (wenv->zscnt) {  // allocate memory for ZSort3D
		case 0: {
			ZSort3D = (pZS3D) malloc(_ZSGROW * sizeof(_ZS3D));
			ZSX = (long *) calloc(sizeof(long), _ZSGROW);
			fsize = _ZSGROW;
			break;
		}
		default: {
//			zf = (long *) calloc(sizeof(long), wenv->zscnt);
			fsize = wenv->zscnt;
			break;
		}	
	}
	zcnt = 0; fcnt = 0;
	lcnt = 0;

	objr = wobj->fo;
	while (objr != NULL) {
	// if lighting turned on update light list
		if (wenv->liteon) {
			if (wenv->lo[0] == NULL) {
				wenv->lvc = 4;
				wenv->lvr = 35; 
			}

			if (objr->lflag != 0) {
				wenv->lo[lcnt] = objr;
				lcnt++;
			}
			wenv->lo[lcnt] = NULL;
		}

		switch(objr->df) {   
			  case 1: {
			  	face = objr->f;
			   	while (face != NULL) {
					tva = face->v[0]->v;
					tvb = face->v[1]->v;
					tvc = face->v[2]->v;
					zv = dCenter(tva, tvb, tvc);
					zv = dvAdd(zv, dvScal(wenv->opos, -1));
					cv = dvScal(zv, 1 / dvMag(zv));
					nv = dNorm(tva, tvb, tvc);
					ft1 = dvDot(cv, nv);
					if ((ft1 < 0) || (face->back == 1)) {
						ft2 = dvDot(zv, wenv->oiv);
						if (ft2 > 0) {
							vc1 = Screen3Dr(wenv, tva);
							svf =  vc1.flag;
							vc2 = Screen3Dr(wenv, tvb);
							svf += vc2.flag;
							vc3 = Screen3Dr(wenv, tvc);
							svf += vc3.flag;
							  // check size of polygon..
							  // if vc4 x+y < 2 is just a pixel
							vc4.x = abs(vc1.x - vc2.x) + abs(
								vc2.x - vc3.x);
							vc4.y = abs(vc1.y - vc2.y) + abs(
								vc2.y - vc3.y);
							if (svf != 3) {
							    ft3 = sqrt(-ft1);
							    if ((vc4.x + vc4.y) < 2) ft3 +=1.0;
								ZSort3D[zcnt].z = ft2;
							    ZSort3D[zcnt].zc = ft3;
							    ZSort3D[zcnt].zf = face;
					//			zf[zcnt] = zcnt;
								ZSX[zcnt] = zcnt;
								
								zcnt++;
								if ((zcnt + 1) == fsize) {
									fsize = fsize + _ZSGROW;
									ZSort3D = (pZS3D) 
										realloc(ZSort3D, fsize * sizeof(_ZS3D));
									ZSX = (long *) 
										realloc(ZSX, fsize * sizeof(long));
								}
							}
						}
					}
					face = face->nx;
				}	
			  	break;
			  }
			  case 2:{
				zv = objr->center;
				zv = dvAdd(zv, dvScal(wenv->opos, -1));
				ft2 = dvDot(zv, wenv->oiv);
				if (ft2 > 0) {
					vc1 = Screen3Dr(wenv, zv);
					if (!vc1.flag) {
						ZSort3D[zcnt].z = ft2;
					    ZSort3D[zcnt].zc = 1.0;
					    ZSort3D[zcnt].zf = objr->f;
						ZSX[zcnt] = zcnt;
						zcnt++;
						if ((zcnt + 1) == fsize) {
							fsize = fsize + _ZSGROW;
							ZSort3D = (pZS3D) 
								realloc(ZSort3D, fsize * sizeof(_ZS3D));
							ZSX = (long *) 
								realloc(ZSX, fsize * sizeof(long));
						}
					}
					face = face->nx;
				}
		 	 	break;
			}
		  	case 0:
		  	default: break;
		}
		objr = objr->nx;
	} 

		// Sort the zinds by ZSort3D (dist) ascending.
		// 	zf holds indexes.
		// e.g. ZSort3D[zf[0]] is first in order,
		//     ''  ''  [  '' [zcnt-1]] is last. 

	if (((fsize - zcnt) > _ZSGROW) && (fsize > _ZSGROW)) 
		fsize = fsize - _ZSGROW;
	wenv->zscnt = fsize;


   	qsort(ZSX, zcnt, sizeof(long), ZSCompS);

//	qsort(ZSort3D, zcnt, sizeof(_ZS3D), ZSCompS2);

//		If this way is too slow then will go back to sorting indexes
//			again with zf


	for (fi = zcnt-1; fi > -1; fi--) {
//		objr = ZSort3D[fi].zf->obj;
//		face = ZSort3D[fi].zf;
		objr = ZSort3D[ZSX[fi]].zf->obj;
		face = ZSort3D[ZSX[fi]].zf;
		pflag = 0;

			// Do shading and draw

		if (objr->df == 2) {	//DEGENERATE CASE
			zv = objr->center;
			vc1 = Screen3Dr(wenv, zv);
				// find radius in pixels
			ft2 = dvMag(dvAdd(zv, dvScal(wenv->opos, -1)));
			ft2 = atan(fabs(objr->dsize / ft2));
			svf = ft2 * wenv->vscal; // radius in pixels
			if (svf < 2) pflag = 1;
				// NOTE: am using polar version
			nv = dvScal(zv, -1);
			nv = dvScal(nv, 1 / dvMag(nv));
			lscol = objr->dcol;
			lscol = LtShade3D(lscol, wenv, nv, zv, objr->lumin);
			vcol = gl_rgbcolor(lscol.r, lscol.g, lscol.b);
		
				// Finally!  Draw the damn thing
				// if is smaller than pixel, draw pixel

			if (pflag) gl_setpixel(vc1.x, vc1.y, vcol);

				// else draw a circle of correct rad..

			else for (fj = svf; fj > 0; fj--) {
				gl_circle(vc1.x, vc1.y, fj, vcol);
			}
			dcnt++;
		}
		else {	// NORMAL CASE
			tva = face->v[0]->v;
			tvb = face->v[1]->v;
			tvc = face->v[2]->v;
//			if (ZSort3D[fi].zc > 1.0) {
			if (ZSort3D[ZSX[fi]].zc > 1.0) {
				vc1 = Screen3Dr(wenv, tva);
//				ZSort3D[fi].zc -= 1.0;
				ZSort3D[ZSX[fi]].zc -= 1.0;
				pflag = 1;
				zv = tva;
			}
			else {
				zv = dCenter(tva, tvb, tvc);
				vc1 = Screen3Dr(wenv, tva);
				vc2 = Screen3Dr(wenv, tvb);
				vc3 = Screen3Dr(wenv, tvc);
			}		
				// NOTE: am using polar version
			vcol = PackRGB(face->rgb, wenv->scols);
			if (shadef)  {
					// use lighting?
					// Nope, use 'gohdly' shading
			  if (!wenv->liteon) { 
//			    ft3 = ZSort3D[fi].zc;
			    ft3 = ZSort3D[ZSX[fi]].zc;
			    if (ft3 > 0) {
				  vr = face->rgb.r;
				  vb = face->rgb.b;
				  vg = face->rgb.g;
				  vr = (int)(vr * ft3 + 1.5);
				  vg = (int)(vg * ft3 + 1.5);
				  vb = (int)(vb * ft3 + 1.5);
				  vcol = gl_rgbcolor(vr, vg, vb);
			    }
			    else {
				  vr = (int)(50.0 * ft3 + 1.5);
			   	  vcol = gl_rgbcolor(vr, vr, vr);
			    }
			  }
				// Yes, do use lighting...
			  else {
				nv = dNorm(tva, tvb, tvc);
				lscol = face->rgb;
				if (objr->lumin > -100) lscol = 
					LtShade3D(lscol, wenv, nv, zv,
					objr->lumin);
				else lscol = FcShade3D(lscol, wenv, nv, zv, 
					objr->refl); 
				vcol = gl_rgbcolor(lscol.r, lscol.g, lscol.b);
			  }
			}
				// Finally!  Draw the damn thing
		if (pflag) gl_setpixel(vc1.x, vc1.y, vcol);
		else tricolor3D(vc1, vc2, vc3, vcol, 0);
		dcnt++;
		}
	}

//  	free(zf);
		
	return dcnt;
}


void ObjMove(p3DObjA mobj, p3DEnv menv) {
	int fm, fn;
	double fg;
	dVECTOR fv, fw;
	p3DObjL objr;
	p3DVert vc;

	// Do traslations
	objr = mobj->fo;
	while (objr != NULL) {
		if (objr->df == 2) {
			fv = objr->vel;
			objr->center = dvAdd(objr->center, 
				dvScal(fv, menv->delta));
		}
		else {
			fv = objr->vel;
			vc = objr->v;
			while (vc != NULL) {
				vc->v = dvAdd(vc->v, dvScal(fv, menv->delta));
				vc = vc->nx;	
			}
			objr->center = dvAdd(objr->center, 
				dvScal(fv, menv->delta));
		}
		objr = objr->nx;
	}

	ObjRot(mobj, menv->delta);
	return;
}

	// Partition an object into 4 faces/per face
int PartObj3D(p3DObjL pobj, int plvl, int tflag, 
		int rflag, double rvar, double rv2) {
	// plvl 1 = faces * 4, 2 -> 16, 3 -> 64, 4 -> 256, 5 -> 1024
	// tflag - radius midpoints?
	// rflag - randomly vary midpoints?
	// rvar - by how much?
	// rv2 - drop rvar by how much each level?

	unsigned int scnt, vcnt, fcnt, tfcnt;
	unsigned int i, j, k, l, m;
	p3DFace nf, f1, f2, f3;
	p3DVert nv[3], v1, v2, v3, sdone;
	p3DSeg s1, s2, s3, ns;
	dVECTOR center;
	double trv, trv2;
	
	center = pobj->center;
	trv2 = rv2;
	if (trv2 < 1.0) trv2 = 1.0;

	// HERE WE GO! Talk about effing spaghetti code...
	//
	for (i = 0; i < plvl; i++) {   // Once for each level...
	  trv = rvar / pow(trv2, i + 1); 
	  f2 = pobj->fx;  // Save old faces position 
	  f1 = pobj->f;
	  v3 = pobj->vx;
	  fcnt = 0;
	  while (1) {  // And once for each face
			// first get the new vertices
		for (k = 0; k < 3; k++)    {

		// DEBUGGING SHIT!
	//		printf("\nlvl-%d fc-%d k-%d ", i, fcnt, k);
	//		fflush(stdout);
		
			v1 = f1->v[k];
			m = k + 1; if (m > 2) m = m - 3;
			
			v2 = f1->v[m];
		// MORE DEBUGGING
	//		printf("m-%d v1-%ld v2-%ld ", m, v1, v2);
	//		fflush(stdout);
				// does seg not exist?
			s3 = AddSeg3D(v1, v2);

		// MORE DEBUGGING
//			printf("AS-%ld ", s3);
//			fflush(stdout);
			
			s1 = v1->seg;
			s2 = v2->seg;

			sdone = NULL;
			while (s1 != NULL) {
				if (s1->v == v2) {
					if (s1->vf != NULL) {
						sdone = s1->vf;
					}
					break;
				}
				else s1 = s1->nx;
			}
			while (s2 != NULL) {
				if (s2->v == v1) break;
				else s2 = s2->nx;
			}
			
	// DEBUGGING
//			printf("SDONE-%ld ", sdone);
//			fflush(stdout);

				// if seg hasn't been marked... 
			if (sdone == NULL) {
			   	nv[k] = AddVert3D(pobj, dvMidPt(v1->v,
					v2->v, tflag, center, rflag, trv));
			   	s1->vf = nv[k];
			   	s2->vf = nv[k];
			}
			else nv[k] = sdone;
	    }  		// end of k-loop (vertices for face j)
		// Now make the four new faces...
	// DEBUGGING
//			printf("\n k-loop done");
			fflush(stdout);

		nf = AddFace3D(pobj);
	    nf->rgb = f1->rgb;
	    nf->back = f1->back;
	    nf->v[0] = f1->v[0];
	    nf->v[1] = nv[0];
	    nf->v[2] = nv[2]; 
		fcnt++;

		nf = AddFace3D(pobj);
	    nf->rgb = f1->rgb;
	    nf->back = f1->back;
	    nf->v[0] = nv[0];
	    nf->v[1] = f1->v[1];
	    nf->v[2] = nv[1]; 
		fcnt++;

		nf = AddFace3D(pobj);
	    nf->rgb = f1->rgb;
	    nf->back = f1->back;
	    nf->v[0] = nv[1];
	    nf->v[1] = f1->v[2];
	    nf->v[2] = nv[2]; 
		fcnt++;

		nf = AddFace3D(pobj);
	    nf->rgb = f1->rgb;
	    nf->back = f1->back;
	    nf->v[0] = nv[0];
	    nf->v[1] = nv[1];
	    nf->v[2] = nv[2]; 
		fcnt++;

	    f1 = f1->nx;
	    if (f1 == f2->nx) break;   // reach end of face list
	  }  		// faces for level i
	// DEBUGGING
//			printf("\n face-loop done\n");
//			fflush(stdout);

		//Well, it works great BUT... got to get rid of old
		//  faces before doing another loop!
		//IF we decide to generate lower order
		//  faces we can pull them here instead of re-building..
		//

		// This is the SURE way, but wasteful?

//		pobj->f = f2->nx;
//		v1 = pobj->v;
//		while (1) {
//			v1->seg = NULL;
//			if (v1 == v3) break;
//			v1 = v1->nx;
//		}


// iffy code starts here: but as of 5/19/03 is working

		f1 = pobj->f;
		while (1) {
			f3 = f1->nx;
			free(f1);
			if (f3 == f2) break;
			f1 = f3;
		}
//			printf("\n");
		// wipe out old seg list
		v1 = pobj->v;
		while (v1 != NULL) {
			s1 = v1->seg;
			while (s1 != NULL) {
				s2 = s1->nx;
				free(s1);
				s1 = s2;
			}
			v1->seg = NULL;
			v1 = v1->nx;
		}
		pobj->f = f2->nx;	
		free(f2);
	// DEBUGGING
			printf("\n PartObj Level %d OK.", i);
			fflush(stdout);

	} 	// end of i-loop (levels)

	printf("\n\n");
	return (fcnt); // return new count of faces
}

#endif // end of 3ddefs.c 
