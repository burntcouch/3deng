//
// SOLEX4L.C 
//
// Version 0.46L of Earth/Moon navigator 
// 1/13/01  - Pat Struthers
// 1/15/01  - Adding mouse support and gunsighting 
// 	 - and LOTS of hacking up into peices..
// 1/17/01  - Finished hacking, double precision conversion
// 1/18/01  - The dynamically allocated memory version...
// 1/19/01  - The above is finally working right
// 3/06/02  - Recompiled with engine v0.46 and svgalib 1.4.3
//	some minor changes in controls
// 3/07/02  - Degnerate objects work!  Added 15000 random stars..
// 5/11/03  - This is a linked list version
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vga.h>
#include <vgagl.h>
#include <vgamouse.h>
 
#include "3Dmaster.c"
#include "3dnav.c"
#include "solexs.c"

// vector initializations for Ship, Earth, Moon, Sun

dVECTOR SX_shipr = 	{  1951082.0, 	      0.0, 	-3129466.0};
dVECTOR SX_shipv = 	{    30369.555,    -8979.59, 	       0.0};
dVECTOR SX_earthr = 	{ -4361158.0, 	      0.0,	 -392676.0};
dVECTOR SX_earthv = 	{    30369.555,       13.21497,        0.0};
double Rearth = 6378000.0;
double Mearth = 5.976E24;
dVECTOR SX_lunar = 	{357471500.0, 	      0.0, 	32186500.0};
dVECTOR SX_lunav = 	{    30369.555,   -1083.1942, 	       0.0};
double Rluna = 1738000.0;
double Mluna = 7.2907E22;
dVECTOR SX_solr = 	{        0.0, 	    149.6E9,	       0.0};
dVECTOR SX_solv =	{	 0.0,		0.0,	       0.0}; 
double Rsol = 700000000.0;
double Msol = 2.0E30;

double _parsec = 149.6E9 * 206264.8;

double UGC = 6.672E-11;

// MAIN

int main(int argc, char *argv[]);

int main(int argc, char *argv[]) {

	_3DEnv tden; 		 			// The drawing window
	p3DObjA obja = NULL;			//object assemblage 
	p3DObjL objr, Ep, Mp, Sp, Ap; 	//object ptr, list ptrs
	p3DObjEX objx; 					// extra object data instance
	_3DDisp dsp; 					// display stuff
	_3DNav nav;  					// ship and navigation structure

	_HtCMap map1, map2; // Color maps for earth and moon

	int mi, mj, mk, mm, mn, dumi;
	int sev;
	char dumc;
	double ftp1, ftp2, ftp3; 
	double ftime, del1, del2;
	double strad, stlum;
	_3DCol stc;

	int debugf = 0;
	dVECTOR vm1, vm2, vm3, dva, dvb;
	long int t1, t2;

	int omx, omy, smx, smy;

	// initialize environment
	//
	// Get video mode from command line if there..
	//

	
	dsp.chgmode = 999;
	if (argc > 1) dsp.chgmode = atoi(argv[1]);

	S3D_SInit(&tden, &dsp, 0); 

	smx = 32767 / (2 * tden.hScrn);
	smy = 32767 / (2 * tden.vScrn);

	if (argc > 2) { debugf = 1;};

	tden.opos = SX_shipr;
	tden.ovel = SX_shipv;
	tden.oiv = dIunit;
	tden.ojv = dJunit;
	tden.okv = dKunit; 
	tden.delta = 1.0;
	dsp.isecs = tden.delta;

	// Ship setup
	nav.cp = tden.opos;  nav.cv = tden.ovel;
	nav.gflag = 0; nav.gonum = NULL; nav.gface = NULL;
	nav.dflag = 0; nav.donum = NULL; nav.dface = NULL;
	nav.dockf = 0;
	nav.nearf = 0;
	nav.gtp = dNULLV; nav.ogtp = dNULLV;  nav.gtv = dNULLV;
	nav.dp = dNULLV; nav.odp = dNULLV;  nav.dv = dNULLV;
	nav.omass[0] = NULL;   // add some masses below
	nav.ca = dNULLV;


	obja = InitObjA3D(0);  // initialize local object assemblage
	tden.local = obja;   // ...and tell env. where it is.

// Now I want an Earth, Moon, Sun

		// SETUP FOR EARTH *********
		//
	vm1 = SX_earthr;	 // Center vector for Earth = OBJ# 0
	objr = SphereObj3D(obja, Rearth, 5, vm1, 1, 0.4, 1.5);
		// DEBUGGING
		printf("\nOK object init (Earth)\n");
		fflush(stdout);
	Ep = objr;

	objr->vel = SX_earthv;
	objr->rot.vz = 0.0000729212 * 180 / PI; 
	dva = dVecRot3D(objr->rox[0], objr->rox[1], objr->rox[2], -23.44);
	dvb = dVecRot3D(objr->rox[2], objr->rox[1], objr->rox[2], -23.44);
	objr->rox[0] = dva; objr->rox[2] = dvb;
	objr->refl = 0.39;
		// DEBUGGING
		printf("\nOK data init (Earth)\n");
		fflush(stdout);

	map1.lcnt = 10;
	map1.lvl[0] = 0.95;
	map1.hcol[0].r = 128; map1.hcol[0].g = 30; map1.hcol[0].b = 0;
	map1.lvl[1] = 0.90;
	map1.hcol[1].r = 128; map1.hcol[1].g = 50; map1.hcol[1].b = 0;
	map1.lvl[2] = 0.85;
	map1.hcol[2].r = 128; map1.hcol[2].g = 70; map1.hcol[2].b = 0;
	map1.lvl[3] = 0.80;
	map1.hcol[3].r = 128; map1.hcol[3].g = 90; map1.hcol[3].b = 0;
	map1.lvl[4] = 0.75;
	map1.hcol[4].r = 128; map1.hcol[4].g = 100; map1.hcol[4].b = 0;
	map1.lvl[5] = 0.70;
	map1.hcol[5].r = 128; map1.hcol[5].g = 128; map1.hcol[5].b = 0;
	map1.lvl[6] = 0.65;
	map1.hcol[6].r = 100; map1.hcol[6].g = 128; map1.hcol[6].b = 0;
	map1.lvl[7] = 0.60;
	map1.hcol[7].r = 70; map1.hcol[7].g = 128; map1.hcol[7].b = 0;
	map1.lvl[8] = 0.55;
	map1.hcol[8].r = 50; map1.hcol[8].g = 128; map1.hcol[8].b = 0;
	map1.lvl[9] = 0.0;
	map1.hcol[9].r = 0; map1.hcol[9].g = 40; map1.hcol[9].b = 128;

		// Map colors onto the surface...
	map1.gradf = 1;
		// DEBUGGING
		printf("\nOK color map init (Earth)\n");
		fflush(stdout);
	HtPaint3D(objr, &map1);
		// DEBUGGING SHIT
		printf("\nHtPaint3D (Earth) OK.\n");
		fflush(stdout);


		// Smooth it back to a sphere...
	HtSmooth3D(objr, Rearth, 0.0005);
		// DEBUGGING SHIT
		printf("\nHtSmooth3D (Earth) OK.\n");
		fflush(stdout);
	
		// SETUP FOR MOON *********
		//
	vm1 = SX_lunar; 	// Center vector for Moon = OBJ# 1

	objr = SphereObj3D(obja, Rluna, 4, vm1, 1, 0.5, 2);
	Mp = objr; 

	objr->vel = SX_lunav;
	objr->rot.vz = 0.000002661696 * 180 / PI; 
	dva = dVecRot3D(objr->rox[0], objr->rox[1], objr->rox[2], 5.145);
	dvb = dVecRot3D(objr->rox[2], objr->rox[1], objr->rox[2], 5.145);
	objr->rox[0] = dva; objr->rox[2] = dvb;
	objr->refl = 0.11;

	map2.gradf = 0;
	map2.lcnt = 100;
	for (mi = 0; mi < 100; mi++) {
		map2.lvl[mi] = (double) (99 - mi) / 100.0;
		map2.hcol[mi].r = 128 - mi;
		map2.hcol[mi].g = 128 - mi;
		map2.hcol[mi].b = 128 - mi;
	}

	HtPaint3D(objr, &map2);
		// DEBUGGING SHIT
		printf("\nHtPaint3D (Moon) OK.\n");
		fflush(stdout);

		// Smooth it back to a sphere...
	HtSmooth3D(objr, Rluna, 0.002);
		// DEBUGGING SHIT
		printf("\nHtSmooth3D (Moon) OK.\n");
		fflush(stdout);

		// SETUP FOR SUN *********
		//
	vm1 = SX_solr; 		// Center vector for Sun = OBJ# 2

	objr = SphereObj3D(obja, Rsol, 1, vm1, 0, 0, 0);
	Sp = objr;

	objr->vel = SX_solv;
	objr->lumin = -26.7;
	objr->df = 2;
	objr->lflag = 1;
	objr->dsize = Rsol;
	objr->dcol = sYellow;

		// SETUP FOR FRIENDLY NEIGHBORHOOD ASTEROID ****
		//
	vm1 = tden.opos; // Center vector for your friendly asteroid
	vm1.vz = vm1.vz + 1000; vm1.vy = vm1.vy + 10000;

	objr = SphereObj3D(obja, 1000, 4, vm1, 1, 1.9, 2.5);
	Ap = objr;

	objr->vel = tden.ovel;
	objr->rot.vz = -.001; objr->rot.vx = 0.00001;
	objr->refl = .25;
	objr->lumin = -999;

		// NOW, add some stars out in the background..

	for (mj = 0; mj < 15000; mj++) {
		vm1.vx = rndd(1000) * _parsec;
		vm1.vy = rndd(1000) * _parsec;
		vm1.vz = rndd(1000) * _parsec;
		strad = (rndd(200) + 100) * Rsol; 	
		objr = DegenObj3D(obja, strad, vm1);
		objr->lumin = rndd(30) + 5 - 26.7;
		stc.r = 200 + (int)rndd(100);
		stc.g = 200 + (int)rndd(100);
		stc.b = 200 + (int)rndd(100);
		objr->dcol = stc;
		objr->lflag = 0;
	}	

	printf( "WHew.. done with basic shapes..\n");

	// Setup masses and so on for Earth/Moon/Sun	
	objr = obja->fo;
	mi = 0;
	while (objr != NULL) {
		objr->xdat = (p3DObjEX) malloc(sizeof(_3DObjEX));
		objr = objr->nx;
		mi++; if (mi == 4) break;
	}
		// DEBUGGING SHIT
		printf("\n Xtended data init OK.\n");
		fflush(stdout);
	
	objr = obja->fo;
	objx = objr->xdat;   // the Earth
	objx->mass = Mearth * UGC; objx->ax = dNULLV;
	objx->amass[0] = Sp; objx->amass[1] = Mp; objx->amass[2] = NULL;
		// DEBUGGING SHIT
		printf("\n Earth xt stuf OK.\n");
		fflush(stdout);

	objr = objr->nx;
	objx = objr->xdat;	// the Moon	
	objx->mass = Mluna * UGC; objx->ax = dNULLV;
	objx->amass[0] = Ep; objx->amass[1] = Sp; objx->amass[2] = NULL;
		// DEBUGGING SHIT
		printf("\n Moon xt stuff OK.\n");
		fflush(stdout);

	objr = objr->nx;
	objx = objr->xdat;   // the Sun
	objx->mass = Msol * UGC; objx->ax = dNULLV;
	objx->amass[0] = NULL; 
		// DEBUGGING SHIT
		printf("\n Sun xt stuff OK.\n");
		fflush(stdout);

	objr = objr->nx;
	objx = objr->xdat;   // an Asteroid	
	objx->mass = 0.0; objx->ax = dNULLV;
	objx->amass[0] = Ep; objx->amass[1] = Sp; objx->amass[2] = Mp;
	objx->amass[3] = NULL;
		// DEBUGGING SHIT
		printf("\n Rock xt stuff OK.\n");
		fflush(stdout);

		// put in masses also for ship
	nav.omass[0] = Ep; nav.omass[1] = Sp; nav.omass[2] = Mp;
	nav.omass[3] = NULL;
		// DEBUGGING SHIT
		printf("\n nav masses OK.\n");
		fflush(stdout);

		// turn on lighting
	tden.liteon = 1;
	
		// initialize video
	S3D_ChgView(&tden, obja, &nav, 1);

		printf("1st ChgView OK \n");
		fflush(stdout);

	vga_init();
	vga_setmode(dsp.vmode);
	vga_setlinearaddressing();
	gl_setcontextvga(dsp.vmode);
	gl_getcontext(&dsp.phys);

	gl_setcontextvgavirtual(dsp.vmode); 
	dsp.ccol = vga_white();
	dsp.gcol = sRRed;
	dsp.ncol = sRBlu;
	dsp.dcol = sRGrn;

	gl_clearscreen(0);

	// allocate memory for fonts, unpack, color, and intialize...
	dsp.font = malloc(8 * 8 * 256 * BYTESPERPIXEL); // <- VERY IMPORTANT!
	gl_expandfont(8, 8, dsp.ccol, gl_font8x8, dsp.font);
	gl_setfont(8, 8, dsp.font);

		printf("Video Init OK \n");
		fflush(stdout);

	// initialize mouse stuff

// Jon's mouse
	//mouse_init("/dev/mouse", MOUSE_IMPS2, 150);

// Pat's mouse
	mouse_init("/dev/mouse", MOUSE_MICROSOFT, 150);
// PS2 mouse
	//mouse_init("/dev/mouse", MOUSE_PS2, 150);
	omx = 100; omy = 100;

		printf("Mouse init OK \n");
		fflush(stdout);

	mj = dsp.lmar + tden.hScrn * 2;
	mk = dsp.tmar + tden.vScrn * 2;
	
	del2 = 0;
	ftime = 0.0; 
	//t1 = time(NULL);
	t1 = clock();
	t2 = 0;


		printf("All init OK\n");
		fflush(stdout);

	while (1) {
		dumi = 255;
		if ((dsp.frcnt % dsp.fskip) == 0) {
			gl_clearscreen(0);
			gl_disableclipping();
			gl_line(dsp.lmar, dsp.tmar, mj, dsp.tmar, dsp.ccol);
			gl_line(mj, dsp.tmar, mj, mk, dsp.ccol);
			gl_line(dsp.lmar, dsp.tmar, dsp.lmar, mk, dsp.ccol);
			gl_line(dsp.lmar, mk, mj, mk, dsp.ccol);
			gl_colorfont(8, 8, dsp.ccol, dsp.font);
			gl_printf(dsp.lmar, tden.vScrn * 2 + dsp.tmar + 5,
				 "Elapsed time: %5.2f   ", dsp.etime);
			gl_printf(-1,-1," Fr/sec: %4.2f  ",ftime);
			gl_printf(-1,-1," Inc : %2.2f  ", dsp.inca);
			gl_printf(-1,-1,"GT rng-> %6.5f km ",
				dvMag(nav.gtp)/1000);
			gl_printf(-1,-1," GT vel-> %3.5f km/sec  ", 
				 dvMag(nav.gtv) /1000);
			gl_printf(-1,-1, "  VIEW: %d      ", tden.view);

			gl_enableclipping();
			gl_setclippingwindow(dsp.lmar, dsp.tmar, mj, mk); 

			if (!tden.dmode) DrawWire3D(&tden, obja);
			else {
				DrawZS3D(&tden, obja);
		// DEBUGGING ONLY
		//	printf("\nDraw ZS call OK..");
		//	fflush(stdout);	

			}

			// Draw gun-sighted triangle if selected

			if (nav.nearf) {
				N3D_DrawNear(&tden, &dsp);
			}
		// DEBUGGING ONLY
		//	printf("\nDrawNear call OK..");
		//	fflush(stdout);	
			
 			if (nav.gflag) {
				N3D_DrawGunS(&tden, &dsp, &nav);
			}
		// DEBUGGING ONLY
		//	printf("\nDraw GunS call OK..");
		//	fflush(stdout);
			
			if (nav.dflag) {
				N3D_DrawDock(&tden, &dsp, &nav);
			}
		// DEBUGGING ONLY
		//	printf("\nDrawDock call OK..");
		//	fflush(stdout);	

			if (mouse_update()) {
				dsp.mx = mouse_getx() / smx;
				dsp.my = mouse_gety() / smy;
				if (!dsp.mb) dsp.mb = mouse_getbutton(); 
			}
			// Undraw - draw mouse pointer

			gl_line(omx-2, omy, omx-5, omy, 0);
			gl_line(omx+2, omy, omx+5, omy, 0);
			gl_line(omx, omy-2, omx, omy-5, 0);
			gl_line(omx, omy+2, omx, omy+5, 0);
		
			gl_line(dsp.mx-2, dsp.my, dsp.mx-5, dsp.my, dsp.ccol);
			gl_line(dsp.mx+2, dsp.my, dsp.mx+5, dsp.my, dsp.ccol);
			gl_line(dsp.mx, dsp.my-2, dsp.mx, dsp.my-5, dsp.ccol);
			gl_line(dsp.mx, dsp.my+2, dsp.mx, dsp.my+5, dsp.ccol);
	
			omx = dsp.mx; omy = dsp.my;

			gl_copyscreen(&dsp.phys);
			dsp.frcnt = 0;
		}

		dsp.frcnt++;
		dsp.frames++;

		S3D_ChgView(&tden, obja, &nav, 0);
		// DEBUGGING ONLY
		//	printf("\nChgView call OK..");
		//	fflush(stdout);	

		// t2 = time(NULL);
		t2 = clock();
		if (t1 + cticks <= t2) {
			del1 = (t2 - t1) / (double) cticks;
			ftime = dsp.frames / del1;
			del2 = (dsp.etime - del2) / del1;
			tden.delta = tden.delta * dsp.isecs / del2;
			dsp.frames = 0; 
			del2 = dsp.etime;
			dsp.rtime = dsp.rtime + del1;
			t1 = t2; t2 = 0;
		}

			// do gravity for Earth, Moon, and Ship
	
		sGravOx(obja, tden.delta, 0);
		// DEBUGGING ONLY
		//	printf("\nGravOx call OK..");
		//	fflush(stdout);	

		N3D_Grav(&nav, tden.delta, 0); 
		// DEBUGGING ONLY
		//	printf("\nNav Grav call OK..");
		//	fflush(stdout);	
		
			// update positions of objects
		
		ObjMove(obja, &tden);
		// DEBUGGING ONLY
		//	printf("\nObjMove call OK..");
		//	fflush(stdout);	

			// Update observer position

		N3D_ShipEnv(&nav, &tden);
		// DEBUGGING ONLY
		//	printf("\nNav ShipEnv call OK..");
		//	fflush(stdout);	
		
			// Update gunsight data
			//   also updates dock data if dock is on
		if (nav.gflag) {
			N3D_UpdGunS(&nav);
		}

		dsp.etime = dsp.etime + tden.delta;	
		
		// Handle mouse events ....

		if (mouse_update()) {
			dsp.mx = mouse_getx() / smx;
			dsp.my = mouse_gety() / smy;
			if (!dsp.mb) dsp.mb = mouse_getbutton(); 
			S3D_DoMouse(&tden, &dsp, obja, &nav);
		}

			// Handle keyboard events

		dumi = vga_getkey();

		dsp.key = dumi;

		dumi = S3D_DoKeyb(&tden, &dsp, obja, &nav);

		Renorm3D(&tden); // Renormalize unit vectors

		if (dumi == 113) break; // 'q' for QUIT!
	}

	// cleanup memory and devices
	dumi = vga_getch();

	vga_setmode(TEXT);
	gl_freecontext(&dsp.phys);
	mouse_close();
	//  need stuff here to free() all the memory we're
	//   hogging...
	
	free(dsp.font);
	exit(0);
}
// END OF MAIN


// end of solex4L.c
