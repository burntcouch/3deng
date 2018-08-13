
//
// sxtest2.c
//
// testing various sections of new linked-list memory model
// for 3D engine v0.46L
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vga.h>
#include <vgagl.h>

#include "mylib/vectors.c"
#include "mylib/parser.c"
#include "3ddata.c"
#include "3dobjm.c"
#include "3ddefs.c"
#include "3dext.c"


#DEFINE _3DCFGFILE "test.3dcfg"

int main(int argc, char *argv[]);

p3DDisp Init3DCfg(char *cfg); 

p3DEnv AddEnv3D(p3DDisp dsp, long ecp);

int main(int argc, char *argv[]) {

	p3DDisp dsp;
	p3DEnv env0;
	p3DObjA obja;
	p3DObjL obji;


	dsp = Init3DCfg(_3DCFGFILE);

	env0 = dsp->Env[0];
	obja = env0->local;
	obji = obja->fo;



	exit(0);
}


p3DDisp Init3DCfg(char *cfg) { 

	FILE *cfgp = NULL;

	char szLINE[100] = "\0";
	char szbuf[100] = "\0";
	char sztmp[100] = "\0";
	char szi[50], szj[50], szk[50];
	char szz = '\0';

	long i, j, k;
	long lp1, lp2, lp3, lcnt;
	long lpd, lpe, lpo;


	p3DDisp static disp = NULL;
	p3DEnv static env = NULL;
	p3DEnv e1, e2;
	p3DObjA soa;
	p3DObjA a1, lo, fo, no;
	p3DObjL o1, o2, *oarr;
	p3DFace f1, f2, *farr;
	p3DVert v1, v2, *varr;

	_3DCol c1, c1; 

	disp = (p3DDisp) malloc(sizeof(_3DDisp));

		// initialize diplay
	disp->chgmode = 0;
	disp->font = NULL;
	disp->phys = NULL;
	disp->ETop = 0;
	disp->Env[0] = env;
	disp->Env[1] = NULL;
	disp->Egrow[0] = 0;
	disp->vmode = G640x480x32K;
	disp->lmar = 20; 
	disp->rmar = 20; 
	disp->tmar = 20; 
	disp->bmar = 40;
	disp->Etxy[0].x = disp->lmar;		
	disp->Ebxy[0].x = 640 - disp->rmar;		
	disp->Etxy[0].y = disp->tmar;		
	disp->Ebxy[0].y = 480 - disp->bmar;		
	disp->isecs = 1.0;
	disp->frames = 0;
	disp->frcnt = 0;
	disp->fskip = 4;
	disp->etime = 0.0;			// elapsed disp. time
	disp->rtime = 0.0;			// elapsed real. time
	disp->Eptime = 2003.5;		// Epoch of game initialization
	disp->inc = 1;					// integer move increment
	disp->inca = 1.0;				// float move increment
	disp->mx = 100;				// last mouse x-coord.
	disp->my = 100;				// last mouse y-coors.
	disp->mb = 0;					// last mouse button (0 if handled)
	disp->key = 0;					// last key hit (0 if handled)
	disp->ccol = sLtGrey;		// text color
	disp->gcol = sRGrn;			// gunsight color
	disp->dcol = sRRed;			// docking tgt color
	disp->ncol = sRBlu;			// near faces color

	if (cfg == NULL) {
		env = (p3DEnv) malloc(sizeof(_3DEnv));
			// initialize environment
		env->vScrn = (disp->Ebxy[0].y - disp->Etxy[0].y) / 2;
		env->hScrn = (disp->Ebxy[0].x - disp->Etxy[0].x) / 2;
		env->scols = 32;
		env->vscal = 3.75 * env->hScrn;
		env->liteon = 0;
		env->dmode = 1;
		env->zscnt = 0;
		env->delta = disp->isecs;
		env->view = 0;
		env->lvc = 5.0;
		env->lvr = 32.0;

		env->opos = dNULLV;          // postion of observer
		env->ovel = dNULLV;   	  // velocity of observer
		env->oiv = dIunit;		  // 'forward' view
		env->ojv = dJunit;		  // 'left' view
		env->okv = dKunit;		  // 'up' view
		env->lo = NULL;      // pointer to lite object array
		env->nonum = NULL;	// tracked object list
		env->tno = NULL;		// pointer to tracked objects
		env->nface = NULL;	// closest faces of tracked objects
		env->nfp[0] = dNULLV;	// last position of near faces
		env->nfv[0] = dNULLV;	// velocity of near faces
		env->ncu[0] = dNULLV;	// center of object relative to opos

		soa = InitObjA3D(0);
		env->local = soa;
		env->neara = NULL;
		env->fara = NULL;
	}
	else {
		lcnt = 0;
		lpd = -1; lpe = -1; lpo = -1;
		cfgp = fopen(cfg, "r+");
		if (cfgp != NULL) {
			while (!feof(cfgp)) {  // find display section
				fgets(szLINE, 98, cfgp);
				lcnt++;
				if ((szLINE[0] != '#') && (szLINE[0] != ' ')) { 
					if (strncmp(szLINE, "DISPLA", 6) == 0) {
						lpd = lcnt; break; 
					}	
 				}
			}
			while (!feof(cfgp)) {  // find envirmonment section
				fgets(szLINE, 98, cfgp);
				lcnt++;
				if ((szLINE[0] != '#') && (szLINE[0] != ' ')) { 
					if (strncmp(szLINE, "ENVI", 4) == 0) {
						lpe = lcnt; break;
					}	
				}
			}
			while (!feof(cfgp)) {  // find object section
				fgets(szLINE, 98, cfgp);
				lcnt++;
				if ((szLINE[0] != '#') && (szLINE[0] != ' ')) { 
					if (strncmp(szLINE, "OBJECT", 6) == 0) {
						lpo = lcnt; break;
					}	
				}
			}
			rewind(cfgp);
			for (i = 0; i < lpd; i++) {
				fgets(szLINE,98,cfgp);
			}
			//
			// process 'DISPLAY' block 
			//
			while (!feof(cfgp)) { 
				fgets(szLINE, 98, cfgp);
				if ((szLINE[0] != '#') && (szLINE[0] != ' ')) { 
					fcnt = parse1(szLINE, szbuf, ' ');
					if (strncmp(szLINE, "ENVI", 4) == 0) break;
					else if (fcnt > 0) { 
						parse_pull(fcnt, szbuf, szi, 1);
						if (strncmp(szi, "VIDMOD", 6) == 0) { // vidmode  
							if (fcnt > 1) {
								parse_pull(fcnt, szbuf, szj, 2);
								disp->chgmode = atoi(szj);
							}
						}	
						if (strncmp(szi, "ISEC", 4) == 0) { // isecs  
							if (fcnt > 1) {
								parse_pull(fcnt, szbuf, szj, 2);
								disp->chgmode = atoi(szj);
							}

						}						
						if (strncmp(szi, "EPTIM", 5) == 0) { // EpTime  
							if (fcnt > 1) {
								parse_pull(fcnt, szbuf, szj, 2);
								disp->EpTime = strtod(szj);
							}

						}						
						if (strncmp(szi, "FSKI", 4) == 0) { // fskip  
							if (fcnt > 1) {
								parse_pull(fcnt, szbuf, szj, 2);
								disp->fskip = atoi(szj);
							}

						}						
						if (strncmp(szi, "FRON", 4) == 0) { // which window in front  
							if (fcnt > 1) {
								parse_pull(fcnt, szbuf, szj, 2);
								disp->EpTop = atoi(szj);
							}
						}						
						if (strncmp(szi, "PLACE", 5) == 0) { // place a window
							if (fcnt > 3) {
								parse_pull(fcnt, szbuf, szj, 2);
								j = atoi(szj);
								if (j < _3DENVMAX) {
									parse_pull(fcnt, szbuf, szj, 3);
									disp->Etxy[j].x = atoi(szj);
									parse_pull(fcnt, szbuf, szj, 4);
									disp->Etxy[j].y = atoi(szj);
									disp->Ebxy[j].x = -1;
									disp->Ebxy[j].y = -1;
								}
							}
						}
					}
				}
			}
				// more display section init
			if (disp->chgmode != 0) {
			 switch (disp->chgmode) {
				case 8: 
				case 7:
				case 6: disp->lmar = l = 32; 
					disp->rmar = r = 32; 
					disp->tmar = t = 32; 
					disp->bmar = b = 64;
					break;
				case 5:
				case 4:
				case 3: disp->lmar = l = 25; 
					disp->rmar = r = 25; 
					disp->tmar = t = 25; 
					disp->bmar = b = 50;
					break;
				case -2:
				case -1: disp->lmar = l = 18; 
					disp->rmar = r = 18; 
					disp->tmar = t = 18; 
					disp->bmar = b = 18;
					break;
				case 2:
				case 1:
				case 0:
				default: disp->lmar = l = 20; 
					disp->rmar = r = 20; 
					disp->tmar = t = 20; 
					disp->bmar = b = 40;
				break;
			 }

			 switch (disp->chgmode) {
				case 8: disp->vmode = G1024x768x16M;
					break;
				case 7: disp->vmode = G1024x768x64K;
					break;
				case 6: disp->vmode = G1024x768x32K;
					break;
				case 5: disp->vmode = G800x600x16M;
					break;
				case 4: disp->vmode = G800x600x64K;
					break;
				case 3: disp->vmode = G800x600x32K;
					break;
				case 2: disp->vmode = G640x480x16M;
					break;
				case 1: disp->vmode = G640x480x64K;
					break;
				case -2: disp->vmode = G512x480x64K;
					break;
				case -1: disp->vmode = G512x480x32K;
					break;
				case 0:
				default: disp->vmode = G640x480x32K;
					break;
			 }
			}
			i = 0;
			while (disp->Env[i] != NULL) {
				disp->Etxy[i].x = disp->Etxy[i].x + disp->lmar;
				disp->Etxy[i].y = disp->Etxy[i].y + disp->tmar;
				i++;
			}
			//
			// process 'ENVIR' block
			//	
			if (lpe != -1) {
				rewind(cfgp);
				for (i = 0; i < lpe; i++) {
					fgets(szLINE,98,cfgp);
				}
				while (!feof(cfgp)) { 
					fgets(szLINE, 98, cfgp);
					if ((szLINE[0] != '#') && (szLINE[0] != ' ')) { 
						fcnt = parse1(szLINE, szbuf, ' ');



					}
				}	
			}
			else { 
				env = (p3DEnv) malloc(sizeof(_3DEnv));
				disp->Env[0] = env;
				disp->ETop = 0;
				disp->Env[1] = NULL;
				disp->Etxy[0].x = disp->lmar;
				disp->Etxy[0].y = disp->tmar;
				switch (disp->chgmode) {
					case 8:
						env->scols = 256;
						disp->Ebxy[0].x = 1024 - disp->rmar;
						disp->Ebxy[0].y = 768 - disp->bmar;
						break;
					case 7:
						env->scols = 64;
						disp->Ebxy[0].x = 1024 - disp->rmar;
						disp->Ebxy[0].y = 768 - disp->bmar;
						break;
					case 6:
						env->scols = 32;
						disp->Ebxy[0].x = 1024 - disp->rmar;
						disp->Ebxy[0].y = 768 - disp->bmar;
						break;
					case 5:
						env->scols = 256;
						disp->Ebxy[0].x = 800 - disp->rmar;
						disp->Ebxy[0].y = 600 - disp->bmar;
						break;
					case 4:
						env->scols = 64;
						disp->Ebxy[0].x = 800 - disp->rmar;
						disp->Ebxy[0].y = 600 - disp->bmar;
						break;
					case 3:
						env->scols = 32;
						disp->Ebxy[0].x = 800 - disp->rmar;
						disp->Ebxy[0].y = 600 - disp->bmar;
						break;
					case 2:
						env->scols = 256;
						disp->Ebxy[0].x = 640 - disp->rmar;
						disp->Ebxy[0].y = 480 - disp->bmar;
						break;
					case 1:
						env->scols = 64;
						disp->Ebxy[0].x = 640 - disp->rmar;
						disp->Ebxy[0].y = 480 - disp->bmar;
						break;
					case -2:
						env->scols = 64;
						disp->Ebxy[0].x = 512 - disp->rmar;
						disp->Ebxy[0].y = 480 - disp->bmar;
						break;
					case -1:
						env->scols = 32;
						disp->Ebxy[0].x = 512 - disp->rmar;
						disp->Ebxy[0].y = 480 - disp->bmar;
						break;
					case 0:
					default:
						env->scols = 32;
						disp->Ebxy[0].x = 640 - disp->rmar;
						disp->Ebxy[0].y = 480 - disp->bmar;
						break;
				}

				env->vScrn = (disp->Ebxy[0].y - disp->Etxy[0].y) / 2;
				env->hScrn = (disp->Ebxy[0].x - disp->Etxy[0].x) / 2;
				env->vscal = 3.75 * env->hScrn;
				env->liteon = 0;
				env->dmode = 1;
				env->zscnt = 0;
				env->delta = disp->isecs;
				env->view = 0;
				env->lvc = 5.0;
				env->lvr = 32.0;

				env->opos = dNULLV;          // postion of observer
				env->ovel = dNULLV;   	  // velocity of observer
				env->oiv = dIunit;		  // 'forward' view
				env->ojv = dJunit;		  // 'left' view
				env->okv = dKunit;		  // 'up' view
				env->lo = NULL;      // pointer to lite object array
				env->nonum = NULL;	// tracked object list
				env->tno = NULL;		// pointer to tracked objects
				env->nface = NULL;	// closest faces of tracked objects
				env->nfp[0] = dNULLV;	// last position of near faces
				env->nfv[0] = dNULLV;	// velocity of near faces
				env->ncu[0] = dNULLV;	// center of object relative to opos

				env->local = NULL;
				env->neara = NULL;
				env->fara = NULL;
			}

			//
			// process 'OBJECTS' block
			//
			fclose(cfgp);
		}
		else {
			printf("\nERROR! Couldn't open file %s.\n", cfg);
			free(disp);
		}
	}
	return (disp);
}



p3DEnv AddEnv3D(p3DDisp dsp, long ecp, _XYCrd tl, _XYCrd br) {

	p3DEnv static nenv = NULL;
	p3DEnv ce;

	long i = 0;

	if ((ecp >= 0) && (ce < _3DENVMAX)) { // COPY env stuff from another
		ce = dsp->Env[ecp];
		while(1) { 
			if (dsp->Env[i] == NULL) break;
			i++;
			if (i == _3DENVMAX) break; 
		}
		if (i < _3DENVMAX) {
			nenv = (p3DEnv) malloc(sizeof(_3DEnv));
			dsp->Env[i] = nenv;
			if ((i + 1) < _3DENVMAX) dsp->Env[i+1] = NULL;
			if (ce != NULL) {
				dsp->Etxy[i] = tl; 
				dsp->Ebxy[i] = br;
				dsp->Egrow[i] = 0;
					// initialize environment
				nenv->vScrn = (dsp->Ebxy[i].y - dsp->Etxy[i].y) / 2;
				nenv->hScrn = (dsp->Ebxy[i].x - dsp->Etxy[i].x) / 2;
				nenv->scols = ce->scols;
				nenv->vscal = 3.75 * nenv->hScrn;
				nenv->liteon = ce->liteon;
				nenv->dmode = 1;
				nenv->zscnt = 0;
				nenv->delta = ce->delta;
				nenv->view = 0;
				nenv->lvc = 5.0;
				nenv->lvr = 32.0;
	
				nenv->opos = dNULLV;          // postion of observer
				nenv->ovel = dNULLV;   	  // velocity of observer
				nenv->oiv = dIunit;		  // 'forward' view
				nenv->ojv = dJunit;		  // 'left' view
				nenv->okv = dKunit;		  // 'up' view

				i = 0;
				while (ce->lo[i] != NULL) {
					nenv->lo[i] = ce->lo[i];   // pointer to lite object array
					i++;
				}
				nenv->lo[i] = 0;

				nenv->nonum = NULL;	// tracked object list
				nenv->tno = NULL;		// pointer to tracked objects
				nenv->nface = NULL;	// closest faces of tracked objects
				nenv->nfp[0] = dNULLV;	// last position of near faces
				nenv->nfv[0] = dNULLV;	// velocity of near faces
				nenv->ncu[0] = dNULLV;	// center of object relative to opos

				nenv->local = ce->local;
				nenv->neara = ce->neara;
				nenv->fara = cd->fara;
			}
		}
	}
	else {
		while(1) { 
			if (dsp->Env[i] == NULL) break;
			i++;
			if (i == _3DENVMAX) break; 
		}
		if (i < _3DENVMAX) {
			nenv = (p3DEnv) malloc(sizeof(_3DEnv));
			dsp->Env[i] = nenv;
			if ((i + 1) < _3DENVMAX) dsp->Env[i+1] = NULL;
			dsp->Etxy[i] = tl; 
			dsp->Ebxy[i] = br;
			dsp->Egrow[i] = 0;
			
				// initialize environment
			nenv->vScrn = (dsp->Ebxy[i].y - dsp->Etxy[i].y) / 2;
			nenv->hScrn = (dsp->Ebxy[i].x - dsp->Etxy[i].x) / 2;
			switch (dsp->chgmode) {
				case 8:
				case 5:
				case 2:
				default: {
					nenv->scols = 256;
					break;
				}
				case 7:
				case 4:
				case 1:
				case -1:
				default: {
					nenv->scols = 64;
					break;
				}

				case 6:
				case 3:
				case 0:
				case -2:
				default: {
					nenv->scols = 32;
					break;
				}
			}	
			nenv->vscal = 3.75 * nenv->hScrn;
			nenv->liteon = 0;
			nenv->dmode = 1;
			nenv->zscnt = 0;
			nenv->delta = dsp->isecs;
			nenv->view = 0;
			nenv->lvc = 5.0;
			nenv->lvr = 32.0;
	
			nenv->opos = dNULLV;          // postion of observer
			nenv->ovel = dNULLV;   	  // velocity of observer
			nenv->oiv = dIunit;		  // 'forward' view
			nenv->ojv = dJunit;		  // 'left' view
			nenv->okv = dKunit;		  // 'up' view
			nenv->lo = NULL;      // pointer to lite object array
			nenv->nonum = NULL;	// tracked object list
			nenv->tno = NULL;		// pointer to tracked objects
			nenv->nface = NULL;	// closest faces of tracked objects
			nenv->nfp[0] = dNULLV;	// last position of near faces
			nenv->nfv[0] = dNULLV;	// velocity of near faces
			nenv->ncu[0] = dNULLV;	// center of object relative to opos

			soa = InitObjA3D(0);
			nenv->local = soa;
			nenv->neara = NULL;
			nenv->fara = NULL;
		}
	}	
	return (nenv);
}


//			TEXTFILE reading stuff
// lcnt = 0;
//	cfg = fopen(_BBCONFIG, "r+");
//	if (cfg != NULL) {
//		while (!feof(cfg)) {
//			fgets(szLINE, 98, cfg);
//			lcnt++;
//			if (strncmp(szLINE, "GINX", 4) == 0) {
//				fcnt = parse1(szLINE, szbuf, szz);
//				if (fcnt > 1) { parse_pull(fcnt, szbuf, sztmp, 2);
//								ogx = atol(sztmp);
//				}
//				break;
//			}
//		}
//		if (gx == 0) ogx = ogx + 1;
//		else ogx = gx + 1;
//		rewind(cfg);
//		for (i = 0; i < (lcnt - 1); i++) {
//			fgets(szLINE,98,cfg);
//		}
//		fseek(cfg, 0L, SEEK_CUR);
//		fprintf(cfg, "GINX %6ld\n", ogx);
//		fclose(cfg);
//		return(1);
//	}
//	else {
//		printf("Couldn't open config file 'bbcfg'.\n");
//		return(0);
//	}



// end of sxtest2.c

