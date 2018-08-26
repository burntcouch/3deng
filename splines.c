//
// splines.c
//
//  05/13/03  -  functions for solving and generating splines
//


#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#ifndef _SPLINES
#define _SPLINES 1

#define _MATMAX 41

// Prototypes

int MatInvers(double *mat, double *cs, double *res, int msz);

int MatDump(double *mat, double *res, int msz); 

// Function definitions

int MatInvers(double *mat, double *cs, double *res, int msz)  {

	int i, j, k, ej, lz[_MATMAX][2], z;
	double fk, fm;

	for (i = 0; i < msz; i++) {
		res[i] = cs[i];
		lz[i][0] = 0;
		lz[i][1] = i;
	}
//
//	printf("\n");
//
// index any rows with leading zeros to the bottom
	for (i = 0; i < msz; i++) {
		if (mat[i*msz] == 0.0) {
			lz[i][0] = 1;
			for (j = 1; j < msz; j++) {
				if (mat[j+i*msz] == 0.0) lz[i][0]++;
				else break;
			}
		}	
//
//	printf("%d   lz0: %d \n", i, lz[i][0]);
//
	}
	
	i = 0; j = 0; k = 0;
//
//	printf("\n");
//

	while (1) {
		if (i == lz[j][0]) {lz[j][1] = k; k++;}
//
//	printf("%d/%d/%d   lz0: %d  lz1: %d\n", i, j, k, lz[j][0], lz[j][1]);
//
		j++;
		if (j == msz) {j = 0; i++;}
		if (k == msz) break;
	}
		// now do the inversion
	z = 0;
	for (i = 0; i < msz; i++) {
		fk = mat[i+lz[i][0]+lz[i][1]*msz];
		if (fk == 0.0) {z = lz[i][1] + 1; continue;}
		ej = lz[i][1]; 
		for (k = 0; k < msz; k++) {
			mat[k+ej*msz] = mat[k+ej*msz] / fk;
		}
		res[ej] = res[ej] / fk;
			// test - debug stuff
//			printf("\n i->%d fk->%9.5f lz(i)->%d\n", i, fk, lz[i][1]);
//			MatDump(mat, res, msz);
//			getchar(); getchar();
			// end of debug
		
		for (j = 0; j < msz; j++) {
			ej = lz[j][1];
			if (i != j) {
				fm = mat[i+lz[i][0]+ej*msz];
				for (k = 0; k < msz; k++) {
					mat[k+ej*msz] = mat[k+ej*msz]  - mat[k+lz[i][1]*msz] * fm;
				}
				res[ej] = res[ej] - res[lz[i][1]] * fm;
			}
			// test - debug stuff
//			printf("\n i->%d j->%d fk->%9.5f fm->%9.5f lz(i)->%d\n", 
//				i, j, fk, fm, lz[i][1]);
//			MatDump(mat, res, msz);
//			getchar(); getchar();
			// end of debug
		}
	}

	return z;  // if any rows had zeros in the diagonal then return last
		// (row + 1) that was so, else return 0 if okay...

}


int MatDump(double *mat, double *cs, int msz) {

	int i, j;
	char szt[6] = "%9.4f";
	if (msz < 16) {
		if (msz > 7) { szt[1] = 38; szt[3] = 33;} 
		if (msz > 8) { szt[1] = 37; szt[3] = 33;} 
		if (msz > 9) { szt[1] = 36; szt[3] = 32;} 
		if (msz > 10) { szt[1] = 35; szt[3] = 32;} 
		if (msz > 12) { szt[1] = 34; szt[3] = 31;} 
		if (msz > 14) { szt[1] = 33; szt[3] = 31;} 
			
		printf("\n\n");
		for (i = 0; i < msz; i++) {
			for (j = 0; j < msz; j++) {
				printf(szt, mat[i*msz+j]);
				printf(" ");
			}
			printf(":");
			printf(szt, cs[i]);
			printf("\n");
		}
		return 1;
	}
 	else return 0;

}


#endif
//	end of splines.c
