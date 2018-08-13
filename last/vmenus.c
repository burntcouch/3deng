//
// vmenus.c -- pull down menus and dialog boxes using svgalib
//
//  Pat Struthers 3/9/2002
//
//  3/14/2002 -- working on display and mode change
//  3/17/2002 -- basic menu stuff working ok..  fleshing out
//  	some extra features and start on dialogs & ctrls
//

// Menu item structure _VMenu
//
// gave up on linked list blather for now...  simple 10x10 array of
// menu elements...
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3ddata.c"  // for color structs and vga/vgagl stuff

#define _VMSIZE 9

struct _vmenu;
struct _vmitem;

typedef struct _vmitem {
	int mID; 		// index # of this element
	int sbin;		// number of items beneath
	int mode;		// -1 - hidden, 0 - closed, 1 - open
	int enbl;		// 0 - disabled, 1 - enabled
	char title[20];		// string to appear in menu
	void *diag;		// ptr to dialog box called w/ this item
} _VMItem;

typedef struct _vmenu {
	char title[20];		// name of menu struct
	int sbin;			// # of menu headings
	int mode;			// 0 - hidden, 1 - visible 
	_VMItem mi[_VMSIZE][_VMSIZE]; 
						// menu items - row 0 are headings 
	_3DCol hcol;		// highlight color bkgrnd
	_3DCol htxt;		// highlight color text
	_3DCol ncol;		// normal color bkgrnd
	_3DCol ntxt;		// normal color text
	_3DCol tbord;		// top border color
	_3DCol mbord;		// menu border color
	_3DCol dtxt;		// disabled text color
	int width;		// width of menu items in pixels
	int hite;		// height of menu items in pixels
	float swait;		// # secs menu stays active waiting for select
	float timer;		// last time stamp for wait
	void *font;		// ptr to special font (use std. 8x8 if NULL)
} _VMenu;

typedef _VMenu *pVMenu;
typedef _VMItem *pVMItem;

// Dialog boxes -- needs some work, just skeleton below
// 	also, controls for the dialog boxes:
//		_VMButton  -- plain old button
//		_VMSlider  -- slider control that returns a double
//		_VMLabel   -- text display box w/line wrap
//		_VMInput   -- text input field (modal)
//		_VMVector  -- a 2D slider, returns radius & angle	
//		_VMScrPad  -- a bitmap drawing pad
//		_VMOptList -- option list
//		_VMRecList -- display/select scrollable records

struct _vdiag;

typedef struct _vdiag {
	int dID;			// index # of this dialog box
	_XYCrd tl;			// top left coord. of box
	_XYCrd wh;			// width & hite
	_3DCol bkgrd;		// background color
	_3DCol tcol;		// title text color
	_3DCol bcol;		// border color 
	int mode;			// 0 - non-modal, 1 - modal
	int thite;			// title bar hite
	char title[15];		// title bar text
	void *ctrl;			// link to first control
} _VDiag;

typedef _VDiag *pVDiag;

//
// Funtion prototypes for building and manipulating the above stuff...
//

int VM_PtInRect(_XYCrd tl, _XYCrd wh, _XYCrd pt); 
	// self-explanatory?

int VM_BuildMenu(char *fname, pVMenu tmenu);
	// Reads a resource file from disk and builds menu structure
	// see menuexpl.mrsc for a prototype resource file 

int VM_InitMenu(pVMenu tmenu);
	// sets font and enables top menu

int VM_ShowMenu(pVMenu tmenu, _XYCrd topleft);
	// draws menu

int VM_ChgMenu(pVMenu tmenu, _XYCrd topleft, _XYCrd mxy, int click);
	// handles menu flags and returns choice

int VM_ClrMenu(pVMenu tmenu, int wf);
	// de-activates menus and items

int VMBuildDiag(char *fname, pVDiag tdiag);
	// Reads a res. file and builds an array of dialog boxes

pVDiag VMGetDiag(pVDiag adiag, int mid);
	// returns ptr to dialog box w/ given index number
	//  -- adiag is assumed to be an array pVDiag, not just one.
	//  use this strictly for building links to menus

int VM_ShowDiag(pVDiag tdiag, _XYCrd offset);
	// draws dialog box (does NOT hide it again unlike ShowMenu above!)

int VM_DoDiag(pVDiag tdiag);
	// handle dialog boxes and returns stuff

int VM_HideDiag(pVDiag tdiag, _XYCrd offset);

int VM_BuildMenu(char *fname, pVMenu tmenu) {

	FILE *pFile = NULL;
	char szLINE[81] = "\0";
	char szT[20] = "\0";

	char szz = '\0';

	int i,j,k,m;
	int lcnt;
	int RECLEN = 80;
	
	
	pFile=fopen(fname,"r");
	if (pFile == NULL)
	{
	 	printf("\nERROR: File '%s' couldn't be opened.\n", fname);
	 	lcnt = -1;
	}
   	else {
		lcnt = 0;
			// init menu array 
		for (i = 0; i < _VMSIZE; i++)
			for (j = 0; j < _VMSIZE; j++) {
				tmenu->mi[j][i].sbin = 0;
				tmenu->mi[j][i].mode = 0;
				tmenu->mi[j][i].enbl = 0;
				tmenu->mi[j][i].title[0] = szz;
				tmenu->mi[j][i].diag = NULL;
		}
			// read in .msrc file
   		while (!feof(pFile))
   		{
    		fgets(szLINE,RECLEN+1,pFile);
			k = strlen(szLINE);
			if (k > 4) { 
				if (szLINE[0] == 'M') {
					sscanf(szLINE + 1, "%s %i", tmenu->title, &i);
					tmenu->sbin = i;
					if (tmenu->sbin > _VMSIZE) tmenu->sbin = _VMSIZE; 
					tmenu->mode = 0;
					for (j = 0; j < i; j++) {
						fgets(szLINE, RECLEN+1, pFile);
						sscanf(szLINE, "%s %i %i", 
							tmenu->mi[0][j].title, &tmenu->mi[0][j].mID,
							&tmenu->mi[0][j].sbin);
						if (tmenu->mi[0][j].sbin > _VMSIZE - 1)
							tmenu->mi[0][j].sbin = _VMSIZE - 1;
						tmenu->mi[0][j].enbl = 1;
						lcnt++;
						for (m = 1; m < tmenu->mi[0][j].sbin + 1; m++) {
							fgets(szLINE, RECLEN+1, pFile);
							sscanf(szLINE, "%s %i %i", 
								tmenu->mi[m][j].title, &tmenu->mi[m][j].mID,
								&tmenu->mi[m][j].sbin);
							tmenu->mi[m][j].enbl = 1;
							lcnt++;
						}
					}
					tmenu->hcol = sMGrey;	// highlight color bkgrnd
					tmenu->htxt = sRRed;	// highlight color text
					tmenu->ncol = sLBlue;	// normal color bkgrnd
					tmenu->ntxt = sYellow;	// normal color text
					tmenu->tbord = sRBlu;	// top border color
					tmenu->mbord = sRGrn;	// menu border color
					tmenu->dtxt = sMGrey;	// disabled text color
					tmenu->width = 85;		// width of menu items in pixels
					tmenu->hite = 14;	// height of menu items in pixels
					tmenu->swait = 2.5;	// # secs menu waits for select
					tmenu->timer = 0;
					tmenu->font = NULL;  // ptr to font - NULL is std.
					lcnt++;
				}
				else if (szLINE[0] == 'O') {
					lcnt++;
				}
			}
		} // end of while - line reading block
   	}
    	
	fclose(pFile);
	return lcnt;
}

int VM_InitMenu(pVMenu tmenu) {

	void static *thefont;
	int ccol;

	thefont = malloc(8 * 8 * 256 * BYTESPERPIXEL); // <- VERY IMPORTANT!
	ccol = gl_rgbcolor(tmenu->ntxt.r, tmenu->ntxt.g, tmenu->ntxt.b);

	gl_expandfont(8, 8, ccol, gl_font8x8, thefont);
	gl_setfont(8, 8, thefont);
	tmenu->font = thefont;
	tmenu->mode = 1;

	return (1);
}

int VM_ShowMenu(pVMenu tmenu, _XYCrd tl) {
		// This ONLY draws the active menu appearance - VM_ChgMenu 
		//  does the actual manipulation via mouse/keyb actions
		//
		// note - assumes graphics is initialized, including fonts!
		// also assumes is drawing on a blank page or over something
		// that is supposed to be covered (i.e. page flipping is
		// expected).

	int i, j, k;
	int fi, fj, fk;
	int acol;

	fi = tmenu->mode;

	if (tmenu->mode > 0) {  // if menu is not hidden
		gl_setfont(8, 8, tmenu->font);  // use MY font, not someone else's!
		 		// draw top row bkgrnd
		gl_fillbox(tl.x, tl.y, tmenu->width * tmenu->sbin,
			tmenu->hite, gl_rgbcolor(
			tmenu->ncol.r, tmenu->ncol.g, tmenu->ncol.b)); 
				// draw top row border and divider marks
		acol = gl_rgbcolor(tmenu->tbord.r, tmenu->tbord.g, tmenu->tbord.b);
		gl_line(tl.x, tl.y, tl.x + tmenu->width * tmenu->sbin, tl.y, acol);
		gl_line(tl.x, tl.y, tl.x, tl.y + tmenu->hite, acol);
		gl_line(tl.x, tl.y + tmenu->hite, tl.x + tmenu->width * tmenu->sbin, 
			tl.y + tmenu->hite, acol);
		gl_line(tl.x + tmenu->width * tmenu->sbin, tl.y, 
			tl.x + tmenu->width * tmenu->sbin, tl.y + tmenu->hite, acol);
		for (i = 0; i < tmenu->sbin; i++) {
			gl_line(tl.x + i * tmenu->width, tl.y, tl.x + i * tmenu->width,
				tl.y + tmenu->hite, acol);
		}
			// draw text labels of menus
		for (i = 0; i < tmenu->sbin; i++) {
			acol = gl_rgbcolor(tmenu->ntxt.r, tmenu->ntxt.g, tmenu->ntxt.b);
			if (tmenu->mi[0][i].enbl == 0)
				acol = gl_rgbcolor(tmenu->dtxt.r, tmenu->dtxt.g, tmenu->dtxt.b);
			if (tmenu->mi[0][i].mode == 1) {
				gl_fillbox(tl.x + 1 + tmenu->width * i, tl.y + 1,
					tmenu->width - 2, tmenu->hite - 2
					, gl_rgbcolor(tmenu->hcol.r,tmenu->hcol.g,tmenu->hcol.b)); 
				acol = gl_rgbcolor(tmenu->htxt.r, tmenu->htxt.g, tmenu->htxt.b);
			}
			gl_colorfont(8, 8, acol, tmenu->font);
			gl_printf(tl.x + 2 + i * tmenu->width, tl.y + 2, 
				"%s", tmenu->mi[0][i].title);
		}
			// if menus are pulled down, draw them too. 
		for (i = 0; i < tmenu->sbin; i++) {
			if (tmenu->mi[0][i].mode && tmenu->mi[0][i].enbl) {
						// draw bkgrd for menu
				gl_fillbox(tl.x + i * tmenu->width, 
					tl.y + 1 + tmenu->hite, tmenu->width,
					tmenu->hite * tmenu->mi[0][i].sbin,
					gl_rgbcolor(tmenu->ncol.r,tmenu->ncol.g,tmenu->ncol.b));
						// draw border for menu
				gl_line(tl.x + i * tmenu->width,
					tl.y + 1 + tmenu->hite, 
					tl.x + i * tmenu->width,
					tl.y + 1 + tmenu->hite * (tmenu->mi[0][i].sbin + 1),
					gl_rgbcolor(tmenu->mbord.r,tmenu->mbord.g,tmenu->mbord.b));
				gl_line(tl.x + (i + 1) * tmenu->width,
					tl.y + 1 + tmenu->hite, 
					tl.x + (i + 1) * tmenu->width,
					tl.y + 1 + tmenu->hite * (tmenu->mi[0][i].sbin + 1),
					gl_rgbcolor(tmenu->mbord.r,tmenu->mbord.g,tmenu->mbord.b));
				gl_line(tl.x + i * tmenu->width,
					tl.y + 1 + tmenu->hite * (tmenu->mi[0][i].sbin + 1), 
					tl.x + (i + 1) * tmenu->width,
					tl.y + 1 + tmenu->hite * (tmenu->mi[0][i].sbin + 1),
					gl_rgbcolor(tmenu->mbord.r,tmenu->mbord.g,tmenu->mbord.b));
				for (j = 1; j < tmenu->mi[0][i].sbin + 1; j++) {
					acol = gl_rgbcolor(tmenu->ntxt.r, tmenu->ntxt.g, 
						tmenu->ntxt.b);
					if (tmenu->mi[j][i].enbl == 0)
						acol = gl_rgbcolor(tmenu->dtxt.r, tmenu->dtxt.g,
							tmenu->dtxt.b);
						// if item is active, highlight it
					if (tmenu->mi[j][i].mode == 1) {
						gl_fillbox(tl.x + 1 + tmenu->width * i, 
							tl.y + 1 + j * tmenu->hite,
							tmenu->width - 2, tmenu->hite - 2,
							gl_rgbcolor(tmenu->hcol.r,tmenu->hcol.g,
								tmenu->hcol.b)); 
						acol = gl_rgbcolor(tmenu->htxt.r, tmenu->htxt.g, 
							tmenu->htxt.b);
					}
					gl_colorfont(8, 8, acol, tmenu->font);
					gl_printf(tl.x + 2 + i * tmenu->width, 
						tl.y + 2 + j * tmenu->hite, 
						"%s", tmenu->mi[j][i].title);
				}
			}
		}
	}	
	return (fi);
}

int VM_ChgMenu(pVMenu tmenu, _XYCrd tl, _XYCrd mxy, int click) {
	// handles everything else with the menu, including the 
	// return ID of the select item if everything else is 
	// kosher.  Doesn't do any drawing, just manipulates the
	// various flags.

	int retID = 0;
	int i, j, k, gx, gy, me, te;
	_XYCrd pa;

	pa.x = (_VMSIZE + 1) * tmenu->width;
	pa.y = (_VMSIZE + 1) * tmenu->hite;

	if (VM_PtInRect(tl, pa, mxy) != 0) {
		gx = (mxy.x - tl.x)/tmenu->width;
		gy = (mxy.y - tl.y)/tmenu->hite;
		te = tmenu->mi[gy][gx].enbl;
		me = tmenu->mi[0][gx].enbl;

		if ((click > 0) && (tmenu->mi[gy][gx].mode == 1)) {
			VM_ClrMenu(tmenu, -2);
			if (tmenu->mi[gy][gx].sbin == 0) {
				retID = tmenu->mi[gy][gx].mID;
			}
			else {
				retID = 0;
				tmenu->mi[0][gx].mode = 1 && me;
			}		
		}
		else {
			retID = 0;
			if (gy > 0) {
				if (tmenu->mi[0][gx].mode == 1) {
					VM_ClrMenu(tmenu, gx);
					if (gy <= tmenu->mi[0][gx].sbin) {
						tmenu->mi[0][gx].mode = 1 && me;
						tmenu->mi[gy][gx].mode = 1 && te;
					}
				}
				else VM_ClrMenu(tmenu, -2);
			}
			else if (gy == 0) {
				VM_ClrMenu(tmenu, -2);
				if ((gx > -1) && (gx < tmenu->sbin)) {
					tmenu->mi[0][gx].mode = 1 && me;
					tmenu->mode = 1;
				}	
			}
			else VM_ClrMenu(tmenu, -2);
		}
	}

	else VM_ClrMenu(tmenu, -2);

	return (retID);
}

int VM_ClrMenu(pVMenu tm, int wf) {
	int i, j, w = wf;

	if (w > tm->sbin) w = -1;
	switch (w) {
		case -2: tm->mode = 0;
		case -1: {
			for (i = 0; i < tm->sbin; i++)
				for (j = 0; j < tm->mi[0][i].sbin + 1; j++)
					tm->mi[j][i].mode = 0;
			break;
		}
		default: {
			for (j = 0; j < tm->mi[0][w].sbin + 1; j++)
					tm->mi[j][w].mode = 0;
			break;
		}
	}
	return(1);
}

int VM_PtInRect(_XYCrd tl, _XYCrd wh, _XYCrd pt) {
	int i = 0;

	if (pt.x >= tl.x) i++;
	if (pt.x <= tl.x + wh.x) i++;
	if (pt.y >= tl.y) i++;
	if (pt.y <= tl.y + wh.y) i++;

	if (i == 4) return (1);
	else return(0);
}

// end of vmenu.c
