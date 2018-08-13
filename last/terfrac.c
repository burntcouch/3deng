//
// TERFRAC.C
// A fractal distortion program...
//
// 1/5/2001
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vga.h>
#include <vgagl.h>

#define _TWID 257  
#define _TLEN 257
#define ARRSIZ 256  

int htarr[_TWID][_TLEN];

int rndi(int ir);
 
int htrand(int rarg, int varf);

int fracht(int vf1, int vf2, int vf3, int vf4, int vf5, int vf6); 
   // Corner Scale 	vf1
   // Countdown Scale 	vf2
   // Countup Scale 	vf3
   // Partition Scale 	vf4
   // Up-Down Offset	vf5
   // Smoothing Scale 	vf6

int main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	int i, j;
	int wid, len;
	int hlo, hhi;

	int f1, f2, f3, f4, f5, f6;
	int water;

	float hsc;
	GraphicsContext phys;
	long int gcol;
	int gx, gy, gpix, sx, sy;

	wid = _TWID; len = _TLEN;
	sx = 700/_TWID; 
	sy = 550/_TLEN;

	f1 = 4;
   	f2 = 0;
   	f3 = 0;
   	f4 = 2;
   	f5 = 1;
   	f6 = 2;
	water = 0;

	switch (argc) {
		case 8: water = atoi(argv[7]);
		case 7: f6 = atoi(argv[6]);
		case 6: f5 = atoi(argv[5]);
		case 5: f4 = atoi(argv[4]);
		case 4: f3 = atoi(argv[3]);
		case 3: f2 = atoi(argv[2]);
		case 2: f1 = atoi(argv[1]);
		default: break;
	}

	srand(time(NULL));

	for (i = 0; i < wid; i++)
		for (j = 0; j < len; j++)
		htarr[i][j] = 0;

	hhi = -1;
	hlo = 1;

   	// Defaults for fracht(): 
	// Corner Scale 	vf1 = 4
   	// Countdown Scale 	vf2 = 0
   	// Countup Scale 	vf3 = 0 
   	// Partition Scale 	vf4 = 2
   	// Up-Down Offset	vf5 = 1
   	// Smoothing Scale 	vf6 = 2

	fracht(f1, f2, f3, f4, f5, f6);
	
	for (i = 0; i < wid; i++)
		for (j = 0; j < len; j++) {
			if (htarr[i][j] > hhi) hhi = htarr[i][j];
			if (htarr[i][j] < hlo) hlo = htarr[i][j];
	};
	hsc = 250.0 / (hhi - hlo);

	vga_init();
	vga_setmode(G800x600x64K);
	vga_setlinearaddressing();
	gl_setcontextvga(G800x600x64K);
	gl_getcontext(&phys);


	for (i = 0; i < wid; i++) {
		for (j = 0; j < len; j++) {
			gx = i * sx + 50;
			gy = j * sy + 25;
			gpix = (int)((htarr[i][j] - hlo) * hsc) + 1;
			gcol = gl_rgbcolor(gpix, gpix, 0);
			if (htarr[i][j] < water)
				 gcol = gl_rgbcolor(0, 0, 255 - gpix);
			gl_fillbox(gx, gy, sx, sy, gcol);
		}
		// printf("\n");
	}

	i = vga_getch();
	vga_setmode(TEXT);
	gl_freecontext(&phys);

	printf("\nCorner Scale    vf1 - 4 \n");
   	printf("Countdown Scale	vf2 - 0 \n");
   	printf("Countup Scale 	vf3 - 0 \n"); 
   	printf("Partition Scale	vf4 - 2 \n");
   	printf("Up-Down Offset  vf5 - 1 \n");
   	printf("Smoothing Scale vf6 - 2 ");
	printf("\n%d  %d  %d  %d  %d  %d\n", f1, f2, f3, f4, f5, f6);
	printf("Hi : %d    Lo: %d   Water: %d \n", hhi, hlo, water);
	exit(0);
}

int rndi(int ir) {
	float fr;

	fr = ((float) ir) * rand() / RAND_MAX;
	return (int) fr;
}

int htrand(int rarg, int varf)
{   
	int tempf,tempg;
	
	tempf = rand() - (RAND_MAX/2);
	if (rarg<1) rarg = 1;
	if (varf<1) varf = 1;
	tempg = RAND_MAX/(rarg * varf * 2);
	return (tempf/tempg);	
}

int fracht(int vf1, int vf2, int vf3, int vf4, int vf5, int vf6) { 

   // Corner Scale 	vf1
   // Countdown Scale 	vf2
   // Countup Scale 	vf3
   // Partition Scale 	vf4
   // Up-Down Offset	vf5
   // Smoothing Scale 	vf6

   int Pt = ARRSIZ;
   int Tdz, Tdv, Tdd;
   int x, y, Ht, peak, vall;
   int inc, i, j;
   int hi, lo, noplot, water;

   water = 0;
  
   peak = -RAND_MAX; vall = RAND_MAX;

   htarr[0][0] = htrand(vf1 * ARRSIZ/8,1);
   htarr[0][Pt] = htrand(vf1 * ARRSIZ/8,1);
   htarr[Pt][0] = htrand(vf1 * ARRSIZ/8,1);
   htarr[Pt][Pt] = htrand(vf1 * ARRSIZ/8,1);


   for (hi=1, lo=0; hi<=ARRSIZ/2; hi=hi*2, lo++);
   hi = lo * vf2 + 1; 
   lo = 1; 

   while (Pt>=2)
   {
   	Pt = Pt / 2; hi = hi - vf2; lo = lo + vf3;
   	for (x = Pt; x <= ARRSIZ-Pt; x = x + 2 * Pt)
   	{
   		for (y = Pt; y <= ARRSIZ-Pt; y = y + 2 * Pt)
		{
	   		Ht = htarr[x-Pt][y-Pt];
	   		Ht += htarr[x-Pt][y+Pt];
	   		Ht += htarr[x+Pt][y-Pt];
	   		Ht += htarr[x+Pt][y+Pt];
	   		Ht = Ht/4;
	   		if (Pt>=vf6) htarr[x][y] = Ht +
				 htrand(vf4 * Pt/8, hi + lo - vf5);
	   		else htarr[x][y] = Ht;
	   		if (htarr[x][y] > peak) peak = htarr[x][y];
           		if (htarr[x][y] < vall) vall = htarr[x][y];	   		    
	   	}
   	}
   	for (x = 0; x <= ARRSIZ; x += Pt)
   	{
   		if (!(x % (Pt * 2))) Tdz = Pt;
   		else Tdz = 0;
   		for (y = Tdz; y <= ARRSIZ; y += 2*Pt)
   		{
   			Ht = 0; Tdd = 0; Tdv = 4;
   			if (x!=0) 	Ht += htarr[x-Pt][y]; else Tdd++;
   			if (x!=ARRSIZ)  Ht += htarr[x+Pt][y]; else Tdd++;
   			if (y!=0) 	Ht += htarr[x][y-Pt]; else Tdd++;
   			if (y!=ARRSIZ)  Ht += htarr[x][y+Pt]; else Tdd++;
   			if (Tdd) Tdv=3;
   			if (Pt >= vf6) htarr[x][y] = (Ht/Tdv)
				+ htrand(vf4 * Pt/8, hi + lo - vf5);
   			else htarr[x][y] = (Ht/Tdv);
   			if (htarr[x][y] > peak) peak = htarr[x][y];
           		if (htarr[x][y] < vall) vall = htarr[x][y];
   		}
   	}
   }
   
   if (water<vall) water=vall+(peak-vall)/10;
   return (1);
}

// end of TERFRAC.C
