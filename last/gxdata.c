//
// gxdata.c
// Star database conversion & utilites for GalEx 
//
// based on Hipparchos data conversion work
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

#include "vectors.c"
#include "3ddata.c"

struct _hipdat;

typedef struct _hipdat {
	long int HIP;
	char Name[26];
	double wMv;
	double wDist;
	double wMass;
	dVECTOR GxP;
	dVECTOR GxV;
	char wSpec[16];
	_3DCol Spcol;		// r-g-b equivalent of MK class
} _HipDat;

typedef _HipDat *pHipDat;

// Function prototypes
//----------------------
	// Loads data into _HipDat array structure...
	// 	Typically you should define the array like so ->
	//
	//	pHipDat HRarr;
	//
	//	HRarr = (pHipDat)calloc(8000, sizeof(_HipDat));
	//
	// 	and call GetHipDat(filename, HRarr);
	//
int GetHipDat(char *fname, pHipDat hip);

	// Writes the _HipDat array built with GetHipDat to disk
int HipWrite(char *fname, pHipDat hip, int recs, int apf);

	// Reads the _HipDat array from disk
int HipRead(char *fname, pHipDat hip);

	// Generates an r-g-b set for a given spectrum

_3DCol gxSpecToRGB(char *spec);




// Function definitions

int HipRead(char *fname, pHipDat hip) {

	int i, rcnt;
	FILE *pFile = NULL;

	pFile = fopen(fname, "r");

	if (pFile == NULL)
	{
	 	printf("\nERROR: File '%s' couldn't be opened.\n", fname);
	 	getchar();
	}
	else {
		rcnt = 0;
		while(!feof(pFile)) {
			fread(&hip[rcnt], sizeof(_HipDat), 1, pFile);
			rcnt++;
		} 
	}

	fclose(pFile);

	return (rcnt - 1);
}

int HipWrite(char *fname, pHipDat hip, int recs, int apf) {
	int i;
	FILE *pFile = NULL;

	if (!apf)
		pFile=fopen(fname,"w");
	else
		pFile=fopen(fname,"a");

	if (pFile == NULL)
	{
	 	printf("\nERROR: File '%s' couldn't be opened.\n", fname);
	 	getchar();
	}

	for (i = 0; i < recs; i++) {
		fwrite(&hip[i], sizeof(_HipDat), 1, pFile); 
	}

	fclose(pFile);
	return 1;
}

int GetHipDat(char *fname, pHipDat hip)
{  
	FILE *pFile = NULL;
	char szLINE[440] = "\0";
	char szT[27] = "\0";
	char szz = '\0';

	int i,j,k;
	int lcnt;
	int RECLEN = 437;
	
	
	pFile=fopen(fname,"r");
	if (pFile == NULL)
	{
	 	printf("\nERROR: File '%s' couldn't be opened.\n", fname);
	 	getchar();
	}
    	else {
		lcnt = 0;
    		while (!feof(pFile))
    		{
	    		fgets(szLINE,RECLEN+1,pFile);
			k = strlen(szLINE);
			if (k >= RECLEN) {

				// HIP Catalog #
			for (i = 1, j = 0; i < 12; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			hip[lcnt].HIP = atoi(szT);

				// Absolute magnitude
			for (i = 221, j = 0; i < 234; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].wMv = atof(szT);
			else hip[lcnt].wMv = 0.0;

				// Distance
			for (i = 206, j = 0; i < 218; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].wDist = atof(szT);
			else hip[lcnt].wDist = 0.0;

				// Mass
			for (i = 237, j = 0; i < 251; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].wMass = atof(szT);
			else hip[lcnt].wMass = 0.0;

				// Name
			for (i = 61, j = 0; i < 85; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			strcpy(hip[lcnt].Name, szT);

				// Spectrum 
			for (i = 188, j = 0; i < 203; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			strcpy(hip[lcnt].wSpec, szT);

			hip[lcnt].Spcol = gxSpecToRGB(hip[lcnt].wSpec);

				// Galactic coords 
			for (i = 254, j = 0; i < 267; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].GxP.vx = atof(szT);
			else hip[lcnt].GxP.vx = 0.0;
			for (i = 270, j = 0; i < 283; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].GxP.vy = atof(szT);
			else hip[lcnt].GxP.vy = 0.0;
			for (i = 286, j = 0; i < 299; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].GxP.vz = atof(szT);
			else hip[lcnt].GxP.vz = 0.0;

				// Space velocity 
			for (i = 302, j = 0; i < 314; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].GxV.vx = atof(szT);
			else hip[lcnt].GxV.vx = 0.0;
			for (i = 317, j = 0; i < 329; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].GxV.vy = atof(szT);
			else hip[lcnt].GxV.vy = 0.0;
			for (i = 332, j = 0; i < 344; i++, j++) {
				szT[j] = szLINE[i];
			}
			szT[j] = szz;
			if (strlen(szT) > 0) hip[lcnt].GxV.vz = atof(szT);
			else hip[lcnt].GxV.vz = 0.0;

			lcnt++;
			} // end of (if) block
		}
    	}
    	
	fclose(pFile);
	return lcnt;
}

_3DCol gxSpecToRGB(char *spec) {

	_3DCol scol;
	int i, j;
	float st;
	char ca;
	unsigned char cb;

	st = 0.0;
	ca = spec[0];
	switch (ca) {
		case 'O': { st = 0.0; break;}
		case 'B': { st = 10.0; break;}
		case 'A': { st = 20.0; break;}
		case 'F': { st = 30.0; break;}
		case 'G': { st = 40.0; break;}
		case 'K': { st = 50.0; break;}
		case 'M': { st = 60.0; break;}
		case 'L':
		case 'S':
		case 'C':
		case 'N': { st = 75.0; break;}
		case 'D': 
		default: { st = 90.0; break;}
	}
	cb = (unsigned char)spec[1];
	if ((cb < 48) || (cb > 57)) st = st + 5.0;
	else if (st < 70.0) {
		st = st + ((unsigned char)spec[1] - 48);
		if (spec[2] == '.')
			st = st + ((unsigned char)spec[3] - 48) / 10.0; 
	}

	if (st < 45) scol.r = (int)(135 - (45 - st) * 0.75);
	else scol.r = (int)(135 + (st - 45) * 1.0);
	if (st < 45) scol.g = (int)(135 - (45 - st) * 0.75);
	else scol.g = (int)(135 - (st - 45) * 1.0);
	if (st < 45) scol.b = (int)(90 + (45 - st) * 1.5);
	else scol.b = (int)(90 - (st - 45) * 0.5);
	if (st >= 90.0) { scol.r = 120; scol.g = 120; scol.b = 120;}
	
	return (scol);
}

// end of gxdata.c 
