//
// SOLEXS.C
//
// Solar System Explorer session functions for solex3/4
//
// 1/15/2001 -  yarggghh..
// 1/17/2001 - Double precision conv.
//	added lots of stuff for view changes
// 1/18/2001 - Finishing up dynamic object allocation
// 1/19/2001 - Finally finished!
//	5/11/2003 - Updating for linked list object re-write
//

// FUNCTION PROTOTYPES

int S3D_SInit(p3DEnv tden, p3DDisp disp, int imode);

int S3D_VInit(p3DDisp disp);

int S3D_ChgView(p3DEnv tden, p3DObjL obja, p3DNav nav, int nupd);

int S3D_DoKeyb(p3DEnv tden, p3DDisp disp, p3DObjL obja, p3DNav nav);

int S3D_DoMouse(p3DEnv tden, p3DDisp disp, p3DObjL obja, p3DNav nav);

// FUNCTION DEFINITIONS

int S3D_SInit(p3DEnv tden, p3DDisp disp, int imode) {

	int l, r, t, b;
	double f1;

	disp->inc = 1; 
	disp->inca = 1;
	disp->frcnt = 0;
	disp->frames = 0; 

	tden->dmode = 2;
	tden->view = 0;
	tden->zscnt = 0;

	disp->etime = 0.0;
	disp->rtime = 0.0;
	disp->frcnt = 0;
	disp->fskip = 4;
	disp->mx = 100; disp->my = 100;
	disp->mb = 0;


	switch (disp->chgmode) {
		case 8:
		case 7:
		case 6: disp->lmar = l = 32; 
			disp->rmar = r = 32; 
			disp->tmar = t = 32; 
			disp->bmar = b = 64;
			tden->vScrn = (768 - t - b) / 2;
			tden->hScrn = (1024 - l - r) / 2;
			break;
		case 5:
		case 4:
		case 3: disp->lmar = l = 25; 
			disp->rmar = r = 25; 
			disp->tmar = t = 25; 
			disp->bmar = b = 50;
			tden->vScrn = (600 - t - b) / 2;
			tden->hScrn = (800 - l - r) / 2;
			break;
		case -2:
		case -1: disp->lmar = l = 18; 
			disp->rmar = r = 18; 
			disp->tmar = t = 18; 
			disp->bmar = b = 18;
			tden->vScrn = (480 - t - b) / 2;
			tden->hScrn = (512 - l - r) / 2;
			break;
		case 2:
		case 1:
		case 0:
		default: disp->lmar = l = 20; 
			disp->rmar = r = 20; 
			disp->tmar = t = 20; 
			disp->bmar = b = 40;
			tden->vScrn = (480 - t - b) / 2;
			tden->hScrn = (640 - l - r) / 2;
			break;
	}

	switch (disp->chgmode) {
		case 8: disp->vmode = G1024x768x16M;
			tden->scols = 256;
			break;
		case 7: disp->vmode = G1024x768x64K;
			tden->scols = 64;
			break;
		case 6: disp->vmode = G1024x768x32K;
			tden->scols = 32;
			break;
		case 5: disp->vmode = G800x600x16M;
			tden->scols = 256;
			break;
		case 4: disp->vmode = G800x600x64K;
			tden->scols = 64;
			break;
		case 3: disp->vmode = G800x600x32K;
			tden->scols = 32;
			break;
		case 2: disp->vmode = G640x480x16M;
			tden->scols = 256;
			break;
		case 1: disp->vmode = G640x480x64K;
			tden->scols = 64;
			break;
		case -2: disp->vmode = G512x480x64K;
			tden->scols = 64;
			break;
		case -1: disp->vmode = G512x480x32K;
			tden->scols = 32;
			break;
		case 0:
		default: disp->vmode = G640x480x32K;
			tden->scols = 32;
			break;
	}

	f1 = 1.875 * 2 * (double) tden->hScrn;
	tden->vscal = f1;

	return 1;
}

int S3D_VInit(p3DDisp disp) {

	return 1;
}


int S3D_ChgView(p3DEnv tden, p3DObjL obja, p3DNav nav, int nupd) {

	dVECTOR gv, dva, dvb, dvc;

	if (nupd == 1) NearFace3D(obja, tden);
	if (tden->view != 0) {
		if (tden->view == 1) {
			if (nav->gflag) {
				gv = nav->gtp;
				gv = dvScal(gv, 1 / dvMag(gv));
				dva = dvCross(dKunit, gv);
				dva = dvScal(dva, 1 / dvMag(dva));
				dvb = dvCross(gv, dva);
				tden->oiv = gv;
				tden->ojv = dva;
				tden->okv = dvb;
			}	
		}
			// Change to v-vector view
		if (tden->view == 2) {
			gv = tden->ovel;
			gv = dvScal(gv, 1 / dvMag(gv));
			dva = dvCross(dKunit, gv);
			dva = dvScal(dva, 1 / dvMag(dva));
			dvb = dvCross(gv, dva);
			tden->oiv = gv;
			tden->ojv = dva;
			tden->okv = dvb;	
		}
			// fix this so view can find objects
			// comment out for now 
//		if (tden->view > 2) {
//			gv = tden->ncu[tden->view-3];
//			gv = dvScal(gv, -1.0 / dvMag(gv));
//			dva = dvCross(dKunit, gv);
//			dva = dvScal(dva, 1 / dvMag(dva));
//			dvb = dvCross(gv, dva);
//			tden->oiv = gv;
//			tden->ojv = dva;
//			tden->okv = dvb;
//		}

	}
	return 1;
}


int S3D_DoKeyb(p3DEnv tden, p3DDisp disp, p3DObjL obja, p3DNav nav) {

	int dkey;
	dVECTOR dva, dvb;

	dkey = disp->key;

	if (dkey >= 49 && dkey <= 57) {
		disp->inc = (dkey-48) * (dkey-48);
		disp->inca = disp->inc / 10.0;
	}
	else switch (dkey) {
		case 32: vga_getch();
			break;
				// '<spc>' pause

		case 119: tden->dmode = 0; 
			break; 
				// 'w' wire frame mode
 
		case 60: disp->fskip++; 
			if (disp->fskip > 7) disp->fskip = 7;
			break; 
				// '<' frame skipper up

		case 62: disp->fskip--; 
			if (disp->fskip < 1) disp->fskip = 1;
			break; 
				// '>' frame skipper down 

		case 101: tden->dmode++;
			  if (tden->dmode == 3) tden->dmode = 1;
			break; 
				// 'e' solid mode

		case 97: nav->cv = dvAdd(nav->cv, dvScal(
			tden->oiv, disp->inca * tden->delta));
			break;
				// 'a' forward thrust

		case 122: nav->cv = dvAdd(nav->cv, dvScal(
			tden->oiv, - disp->inca * tden->delta));
			break;
				// 'z' backward

		case 105: nav->cv = dvAdd(nav->cv,
			dvScal(tden->okv, tden->delta * disp->inca));
			break; 
				// 'i' up

		case 109: nav->cv = dvAdd(nav->cv,
			dvScal(tden->okv, -tden->delta * disp->inca));
			break; 
				// 'm' down

		case 106: nav->cv = dvAdd(nav->cv,
			dvScal(tden->ojv, tden->delta * disp->inca));
			break; 
				// 'j' left

		case 107: nav->cv = dvAdd(nav->cv,
			dvScal(tden->ojv, -tden->delta * disp->inca));
			break; 
				// 'k' right

		case 44: disp->isecs = disp->isecs * 1.5;
			if (disp->isecs > 30) disp->isecs = 30.0;
			break; 
				// ',' incr time step
		case 46: disp->isecs = disp->isecs / 1.5;
			if (disp->isecs < 0.01) disp->isecs = 0.01;
			break; 
				// '.' decr time step
		case 59: tden->vscal = tden->vscal * 1.5;
			if (tden->vscal > 500000) tden->vscal = 500000;
			tden->zscnt = 0;
			break;
				// ';' zoom in 
		case 39: tden->vscal = tden->vscal / 1.5;
			if (tden->vscal < 100) tden->vscal = 100;
			tden->zscnt = 0;
			break;
				// ''' zoom out
		case 114:
			tden->liteon = (tden->liteon + 1) % 2;
			break;
				// 'r' toggle lighting
		case 91: 
			tden->lvr = tden->lvr + 3;
			if (tden->lvr > 100) tden->lvr = 100;
			break;
				// '[' light level down
		case 93:
			tden->lvr = tden->lvr - 3;
			if (tden->lvr < -100 ) tden->lvr = -100;
			break;
				// ']' light level up

		case 99:
			tden->view = tden->view - 1;
			if (tden->view <= 0) tden->view = 0;
			S3D_ChgView(tden, obja, nav, 1);
			tden->zscnt = 0;
			break; 		
				// 'c' go down one view
		case 118:
			tden->view = tden->view + 1;
			if (tden->view > 2) {
			
	// need to do a new thing here and in S3D_ChgView		
//			   if ((tden->view - 3) == obja->nexto)
				 tden->view = 2;
			}
			S3D_ChgView(tden, obja, nav, 1);
			tden->zscnt = 0;
			break; 		
				// 'v' go up one view
		case 102:
			tden->view = 0;
			break; 		
				// 'f' unlock view to 0

		case 100:
			nav->dflag = (nav->dflag + 1) % 2;
			if ((nav->dflag) && (nav->gflag)) {
				nav->donum = nav->gonum;
				nav->dface = nav->gface; 
				nav->dp = nav->gtp;
				nav->dv = nav->gtv;
				nav->odp = nav->ogtp;
			}
			break; 		
				// 'd' set docking face to gs

		case 110:
			nav->nearf = (nav->nearf + 1) % 2;
			break; 		
				// 'n' toggle near faces view

		default: break;
	}
	disp->key = 0;

	return (dkey);
}



int S3D_DoMouse(p3DEnv tden, p3DDisp disp, p3DObjL obja, p3DNav nav) {

	dVECTOR dva, dvb, gunv;
	_XYCrd g;
	double trad;
	p3DObjL obt;
	p3DFace fct;
	
	int i, j;

	switch (disp->mb)
	{
		case MOUSE_LEFTBUTTON: // Rotate view to center on mouse
			g.x = disp->mx; 
			g.y = disp->my;
			gunv = ScrUnit3D(tden, g);
			dva = dvCross(tden->okv, gunv);
			dva = dvScal(dva, 1 / dvMag(dva));
			dvb = dvCross(gunv, dva);
			tden->oiv = gunv;
			tden->ojv = dva;
			tden->okv = dvb;	
			tden->zscnt = 0;
			break;
		case MOUSE_RIGHTBUTTON: // Find and mark gunsight
			g.x = disp->mx; 
			g.y = disp->my;
			gunv = ScrUnit3D(tden, g); 
			RayFace3D(obja, tden->opos, gunv, fct, obt, &trad);
			if (j > -1) {
				nav->gonum = obt;
				nav->gface = fct;
				nav->gflag = 1;
				N3D_GetGunS(nav);
				S3D_ChgView(tden, obja, nav, 1);
			}
			else nav->gflag = 0;
			break;
		default: break;
	}
	if (disp->mb) disp->mb = 0;

	return 1;
}

// end of solexsDL.c
