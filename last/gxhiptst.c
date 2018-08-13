//
// Star database conversion & utilites for GalEx 
//
// based on Hipparchos data
//
// 02/05/2001 - Pat Struthers
// 02/07/2001 - display functions
//
// 03/08/2002 - stealing my own code for GalEx purposes...
//
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <vga.h>
#include <vgagl.h>


#include "vectors.c"
#include "gxdata.c"

int main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	pHipDat HRarr;
	
	int i, j, k;
	int hcnt;

	double mv, dist, vt;

	char szf[60] = "../data/HOut2.txt";

	HRarr = (pHipDat)calloc(8000, sizeof(_HipDat));

	if (argc > 1) strcpy(szf, argv[1]);

	hcnt = GetHipDat(szf, HRarr);

	printf("\n%d records read...\n", hcnt);

	// add .hd extension
	strcat(szf, ".hd");

	HipWrite(szf, HRarr, hcnt, 0);

	hcnt = HipRead(szf, HRarr);

	printf("\n%d records read...from %s\n", hcnt, szf);
	i = 0; 
	k = 1;
	for (i = 1; i < hcnt; i++) {	
	
		mv = HRarr[i].wMv;
		dist = HRarr[i].wDist;
		printf("\n\n-----------------------------------\n");
		printf("---  Filename: %s ---- Index# %d --\n\n", szf, i); 
		printf("HIP# %d   Name : %s \n",
			HRarr[i].HIP, HRarr[i].Name);
		printf("\nDist.: %5.2f pc    Mv: %3.2f    Mass: %3.2f\n",
			dist, mv, HRarr[i].wMass);
		printf("\nSpectrum    : %s\n", HRarr[i].wSpec);
		printf("\nRGB Spec    : r-%d g-%d b-%d\n", HRarr[i].Spcol.r,
			HRarr[i].Spcol.g, HRarr[i].Spcol.b);
		j = getchar();
		if (j == 113) break;
     		j = 0; 
	}

	free(HRarr);
	exit (0);
}

// end of hipdata.c 
