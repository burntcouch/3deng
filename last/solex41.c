//
// SOLEX41.C 
//
// Version 0.41 of Earth/Moon navigator 
// 1/13/01  - Pat Struthers
// 1/15/01  - Adding mouse support and gunsighting 
// 	 - and LOTS of hacking up into peices..
// 1/17/01  - Finished hacking, double precision conversion
// 1/18/01  - The dynamically allocated memory version...
// 1/19/01  - The above is finally working right
// 3/06/02  - Recompiled with engine v0.46 and svgalib 1.4.3
//	some minor changes in controls
// 3/07/02  - Degnerate objects work!  Added 15000 random stars..
// 3/18/02  - Added menus and replace random stars w/real Hipparchos
//  data.  Some other minor cleaning up but..  wait a few.        
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
#include "vmenus.c"
#include "gxdata.c"

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

	_3DEnv tden;  	// The drawing window
	_3DObjL obja; 	// Basic object
	p3DObjEX objx; 	// extra object data instance
	p3DObjR objr;	// object vert/face instance
	_3DDisp dsp; 	// display stuff
	_3DNav nav;  	// ship and navigation structure

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

	pHipDat HRarr;

	char szf[60] = "data/HOut2.txt";

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
	nav.gflag = 0; nav.gonum = -1; nav.gface = -1;
	nav.dflag = 0; nav.donum = -1; nav.dface = -1;
	nav.dockf = 0;
	nav.nearf = 0;
	nav.gtp = dNULLV; nav.ogtp = dNULLV;  nav.gtv = dNULLV;
	nav.dp = dNULLV; nav.odp = dNULLV;  nav.dv = dNULLV;
	nav.omass[0] = 0; nav.omass[1] = 1; nav.omass[2] = 2;
	nav.omass[3] = -1;
	nav.ca = dNULLV;

	// Now I want an Earth, Moon, Sun

	obja.nexto = 0;

		// SETUP FOR EARTH *********
		//
	vm1 = SX_earthr;	 // Center vector for Earth = OBJ# 0

	mi = SphereObj3D(&obja, Rearth, 5, vm1, 1, 0.4, 1.5);
	objr = obja.odat[mi];

	obja.vel[mi] = SX_earthv;
	obja.rot[mi].vz = 0.0000729212 * 180 / PI; 
	dva = dVecRot3D(obja.riv[mi], obja.rjv[mi], obja.rkv[mi], -23.44);
	dvb = dVecRot3D(obja.rkv[mi], obja.rjv[mi], obja.rkv[mi], -23.44);
	obja.riv[mi] = dva; obja.rkv[mi] = dvb;
	obja.refl[mi] = 0.39;

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
	HtPaint3D(objr, &map1);

		// Smooth it back to a sphere...
	HtSmooth3D(objr, Rearth, 0.0005);
	
		// SETUP FOR MOON *********
		//
	vm1 = SX_lunar; 	// Center vector for Moon = OBJ# 1

	mi = SphereObj3D(&obja, Rluna, 4, vm1, 1, 0.5, 2);
	objr = obja.odat[mi];

	obja.vel[mi] = SX_lunav;
	obja.rot[mi].vz = 0.000002661696 * 180 / PI; 
	dva = dVecRot3D(obja.riv[mi], obja.rjv[mi], obja.rkv[mi], 5.145);
	dvb = dVecRot3D(obja.rkv[mi], obja.rjv[mi], obja.rkv[mi], 5.145);
	obja.riv[mi] = dva; obja.rkv[mi] = dvb;
	obja.refl[mi] = 0.11;

	map2.gradf = 0;
	map2.lcnt = 100;
	for (mi = 0; mi < 100; mi++) {
		map2.lvl[mi] = (double) (99 - mi) / 100.0;
		map2.hcol[mi].r = 128 - mi;
		map2.hcol[mi].g = 128 - mi;
		map2.hcol[mi].b = 128 - mi;
	}

	HtPaint3D(objr, &map2);

		// Smooth it back to a sphere...
	HtSmooth3D(objr, Rluna, 0.002);

		// SETUP FOR SUN *********
		//
	vm1 = SX_solr; 		// Center vector for Sun = OBJ# 2

	mi = SphereObj3D(&obja, Rsol, 2, vm1, 0, 0, 0);
	obja.vel[mi] = SX_solv;
	obja.lumin[mi] = -26.7;
	obja.df[mi] = 2;
	obja.lflag[mi] = 1;
	obja.dsize[mi] = Rsol;
	obja.dcol[mi] = sYellow;

		// SETUP FOR FRIENDLY NEIGHBORHOOD ASTEROID ****
		//
	vm1 = tden.opos; // Center vector for your friendly asteroid
	vm1.vz = vm1.vz + 1000; vm1.vy = vm1.vy + 10000;

	mi = SphereObj3D(&obja, 1000, 5, vm1, 1, 1.9, 2.5);
	obja.vel[mi] = tden.ovel;
	obja.rot[mi].vz = -.001; obja.rot[3].vx = 0.00001;
	obja.refl[mi] = .25;
	obja.lumin[mi] = -999;

		// NOW, add some stars out in the background..
		//   below will read in 7425 stars from my prepped
		//   database.
	

	HRarr = (pHipDat)calloc(8000, sizeof(_HipDat));

	// add .hd extension to filename
	strcat(szf, ".hd");

	mk = HipRead(szf, HRarr);

	for (mj = 1; mj < mk; mj++) {
		vm1.vx = HRarr[mj].GxP.vx  * _parsec;
		vm1.vy = HRarr[mj].GxP.vy  * _parsec;
		vm1.vz = HRarr[mj].GxP.vz  * _parsec;
		strad = Rsol; 	
		mi = DegenObj3D(&obja, strad, vm1);
		obja.lumin[mi] = HRarr[mj].wMv + 5 - 26.7;
		obja.dcol[mi] = HRarr[mj].Spcol;
		obja.lflag[mi] = 0;
	}	

	printf( "WHew.. done with basic shapes..\n");

	// Setup masses and so on for Earth/Moon/Sun	
	obja.xdat[0] = (p3DObjEX)malloc(sizeof(_3DObjEX));
	obja.xdat[1] = (p3DObjEX)malloc(sizeof(_3DObjEX));
	obja.xdat[2] = (p3DObjEX)malloc(sizeof(_3DObjEX));
	obja.xdat[3] = (p3DObjEX)malloc(sizeof(_3DObjEX));
 
	objx = obja.xdat[0];   // the Earth
	objx->onum = 0;	
	objx->mass = Mearth * UGC; objx->ax = dNULLV;
	objx->amass[0] = 1; objx->amass[1] = 2; objx->amass[2] = -1;

	objx = obja.xdat[1];	// the Moon	
	objx->onum = 1;
	objx->mass = Mluna * UGC; objx->ax = dNULLV;
	objx->amass[0] = 0; objx->amass[1] = 2; objx->amass[2] = -1;

	objx = obja.xdat[2];   // the Sun
	objx->onum = 2;
	objx->mass = Msol * UGC; objx->ax = dNULLV;
	objx->amass[0] = -1; 

	objx = obja.xdat[3];   // an Asteroid	
	objx->onum = 3;	
	objx->mass = 0.0; objx->ax = dNULLV;
	objx->amass[0] = 0; objx->amass[1] = 1; objx->amass[2] = 2;
	objx->amass[3] = -1;

		// turn on lighting

	GetLites3D(&tden, &obja, 1);
	tden.liteon = 1;
	
		// initialize video

	S3D_ChgView(&tden, &obja, &nav, 1);

		printf("1st ChgView OK \n");

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

	// initialize mouse stuff

// Jon's mouse
	//mouse_init("/dev/mouse", MOUSE_IMPS2, 150);

// Pat's mouse
	mouse_init("/dev/mouse", MOUSE_MICROSOFT, 150);
// PS2 mouse
	//mouse_init("/dev/mouse", MOUSE_PS2, 150);
	omx = 100; omy = 100;

	mj = dsp.lmar + tden.hScrn * 2;
	mk = dsp.tmar + tden.vScrn * 2;
	
	del2 = 0;
	ftime = 0.0; 
	//t1 = time(NULL);
	t1 = clock();
	t2 = 0;


		printf("All init OK\n");

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
			gl_printf(-1,-1," Target#: %d-%d\n ", nav.gonum, 
				nav.gface);
			gl_printf(-1,-1,"GT rng-> %6.5f km ",
				dvMag(nav.gtp)/1000);
			gl_printf(-1,-1," GT vel-> %3.5f km/sec  ", 
				 dvMag(nav.gtv) /1000);
			gl_printf(-1,-1, "  VIEW: %d      ", tden.view);

			gl_enableclipping();
			gl_setclippingwindow(dsp.lmar, dsp.tmar, mj, mk); 

			if (!tden.dmode) DrawWire3D(&tden, &obja);
			else {
				DrawZS3D(&tden, &obja);
			}

			// Draw gun-sighted triangle if selected

			if (nav.nearf) {
				N3D_DrawNear(&tden, &dsp, &obja);
			}
 			if (nav.gflag) {
				N3D_DrawGunS(&tden, &dsp, &obja, &nav);
			}
			if (nav.dflag) {
				N3D_DrawDock(&tden, &dsp, &obja, &nav);
			}

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

		S3D_ChgView(&tden, &obja, &nav, 0);

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
	
		sGravOx(&obja, tden.delta, 0);

		N3D_Grav(&obja, &nav, tden.delta, 0); 
		
			// update positions of objects
		
		ObjMove(&obja, &tden);

			// Update observer position

		N3D_ShipEnv(&nav, &tden);
		
			// Update gunsight data

		if (nav.gflag) {
			N3D_UpdGunS(&obja, &nav);
		}

		dsp.etime = dsp.etime + tden.delta;	
		
		// Handle mouse events ....

		if (mouse_update()) {
			dsp.mx = mouse_getx() / smx;
			dsp.my = mouse_gety() / smy;
			if (!dsp.mb) dsp.mb = mouse_getbutton(); 
			S3D_DoMouse(&tden, &dsp, &obja, &nav);
		}

			// Handle keyboard events

		dumi = vga_getkey();

		dsp.key = dumi;

		dumi = S3D_DoKeyb(&tden, &dsp, &obja, &nav);

		Renorm3D(&tden); // Renormalize unit vectors

		if (dumi == 113) break; // 'q' for QUIT!
	}

	// cleanup memory and devices
	dumi = vga_getch();

	vga_setmode(TEXT);
	gl_freecontext(&dsp.phys);
	mouse_close();
	for (mi = 0; mi <= obja.nexto; mi++) { 
		FreeObj3D(&obja, mi);
	}
	free(HRarr);
	obja.nexto = 0;
	free(dsp.font);
	exit(0);
}
// END OF MAIN


// end of solex3.c
