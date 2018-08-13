//
// waveread.c
//
// 8/14/2001  -- Pat Struthers
// Wave file reader
//
// see end of this file for .wav format

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vga.h>
#include <vgagl.h>

struct _wave;

typedef struct _wave {
	char name[61];
	int wtype;  	//old wav = 16, new = 18, MSNA = 24, ADPCM = 50
	int nch;  	//# of channels (i.e. mono = 1, stereo = 2
	int bps;  	//bits per sample (usually 8 or 16)
	int sfq;  	//sample frequency (8000, 11000, 22000, 44000)
	long int nsmp; 	//# of samples
	unsigned char *wbuf; 	// size = nsmp * nch * bps/8
			     	// length in secs = (float) nsmp / sfq
} _Wave;

typedef _Wave *pWave;

int W_getwave(FILE *fptr, pWave thewave);

int W_plotwave(pWave tw, float secs, float soff, int ph, int pw, int chan);

long int hex2long(unsigned char *thehex, int hlen);

int main(int argc, char *argv[]) {

	unsigned char b1, b2;
	int i, j, k, dmpb, wtyp;
	long int bi, bj, brate;
	FILE *fptr;
	char fname[61];
	char fdesc[81];
	char dpath[20] = "/w95e/musicwav/";
	float wlen, twid, toff;

	// Graphics stuff
	int vmode;
	GraphicsContext gcont;
	void *font;
	long int ccolor;
	char dummy;

	_Wave awave;

	if (argc == 1) {
		printf("\n Syntax: waveread <filename> \n");
	}
	else {

		vga_init();
		vga_setmode(G800x600x32K);
		vga_setlinearaddressing();
		gl_setcontextvga(G800x600x32K);

		ccolor = vga_white();
		font = malloc(8 * 8 * 256 * BYTESPERPIXEL);
		gl_expandfont(8, 8, ccolor, gl_font8x8, font);
		gl_setfont(8, 8, font);

		gl_clearscreen(0);
		
		strcpy(fname, argv[1]);
		strcpy(awave.name, fname);
		strcpy(fdesc, dpath);
		strcat(fdesc, fname);
		fptr = fopen(fdesc, "r");
		if (fptr) {
			wtyp = W_getwave(fptr, &awave);
			brate = awave.sfq * awave.nch * awave.bps / 8;
			wlen = (float) awave.nsmp / awave.sfq;
			gl_printf(50, 500, "\nName--->  %s\n", awave.name);
			gl_printf(-1,-1, "Wave type   -> %d\n", awave.wtype);
			gl_printf(-1,-1, "# of chan.  -> %d\n", awave.nch);
			gl_printf(-1,-1, "Bits/chan.  -> %d\n", awave.bps);
			gl_printf(-1,-1, "Sample rate -> %d Hz\n", awave.sfq);
			gl_printf(-1,-1, "Bytes/sec   -> %d bps\n", brate);
			gl_printf(-1,-1, "# of smpls  -> %d\n", awave.nsmp);
			gl_printf(-1,-1, "Length      -> %4.4f secs\n", wlen);
			W_plotwave(&awave, wlen, 0.0, 400, 600, 0);
			twid = wlen;
			toff = 0.0;
			while (1) {
			  dummy = vga_getkey();
			  switch(dummy) {
				case 105: // zoom in
				  twid = twid / 2;
				  if (twid < 0.025) twid = 0.025;
				  W_plotwave(&awave, twid, toff, 400, 600, 0);
				  break;
				case 107: // zoom out
				  twid = twid * 2;
				  if (twid > wlen) twid = wlen;
				  W_plotwave(&awave, twid, toff, 400, 600, 0);
				  break;
				case 106: // move back
				  toff = toff - twid;
				  if (toff < 0) toff = 0;
				  W_plotwave(&awave, twid, toff, 400, 600, 0);
				  break;
				case 108: // move forward 
				  toff = toff + twid;
				  if (toff >= wlen) toff = wlen - twid;
				  W_plotwave(&awave, twid, toff, 400, 600, 0);
				  break;
			  }
			  if (dummy == 113) break;
			  dummy = 0;
			}
		}
		else { printf("\n Bad file name or file error.. \n");}
		fclose(fptr);
	}
	dummy = vga_getch();
	vga_setmode(TEXT);
//	gl_freecontext(&gcont);
//	mouse_close();
	free(font);
	exit(0);

}

int W_getwave(FILE *fptr, pWave thewave) {
	unsigned char *whdr;
	long int brate, fsz, bi, bj;
	int ssiz;
	
	whdr = (char *)malloc(58);
	
	fread(whdr, 1, 58, fptr);
	thewave->wtype = (int) hex2long(whdr + 16, 1);
	fsz = hex2long(whdr + 4, 4) + 8;
	thewave->nch = hex2long(whdr + 22, 2);
	thewave->bps = hex2long(whdr + 34, 2);
	thewave->sfq = hex2long(whdr + 24, 4);
	brate = hex2long(whdr + 28, 4);
	if (thewave->wtype == 16) thewave->nsmp = hex2long(whdr + 40, 4);
	else thewave->nsmp = hex2long(whdr + 54, 4);
	thewave->nsmp = thewave->nsmp / (thewave->nch * (thewave->bps / 8));

	if ((thewave->wtype == 18) || (thewave->wtype = 16)) {
		if (thewave->wtype == 18) bj = 58;
		else bj = 44;
		fseek(fptr, bj, SEEK_SET);
		ssiz = thewave->bps / 8;
		bi = thewave->nsmp * thewave->nch * ssiz;
		thewave->wbuf = (char *)malloc(bi);
		fread(thewave->wbuf, 1, bi, fptr);
	}
	else thewave->wbuf = NULL;
	free(whdr);

	return(thewave->wtype);
};

int W_plotwave(pWave tw, float secs, float soff, int ph, int pw, int chan) {

	long int sampR, sampL, bi, bj, bk, br, psmp, poff, ccolor;
	int ssiz, wtyp, sx, sy;
	float wlen;
	float vscal, hscal;


	ccolor = vga_white();
	wlen = (float) tw->nsmp / tw->sfq;
	psmp = (long)((float) tw->nsmp * secs / wlen); 
	poff = (long)(soff * tw->sfq);
	if (psmp+poff > tw->nsmp) psmp = tw->nsmp - poff;

	hscal = (float) psmp / pw;
	if (tw->bps == 8) br = 256;
	else br = 65536;
	gl_fillbox(0, 0, pw + 45, ph + 45, 0);
	switch(chan) {
		case 0: vscal = (float) br * 2 / (ph - 20);
			gl_printf(10, 15 + ph / 4, "Ch1");
			gl_printf(10, 25 + 3 * ph / 4, "Ch2");
			gl_line(35, 20+ph/2, 35+pw, 20+ph/2, ccolor);
			gl_line(35, 10+ph/2, 35, 30+ph/2, ccolor);
			gl_line(35+pw/2, 10+ph/2, 35+pw/2, 30+ph/2, ccolor);
			gl_line(35+pw, 10+ph/2, 35+pw, 30+ph/2, ccolor);
			gl_printf(35, 16+ph/2, "%4.4f", soff);
			gl_printf(35+pw-40, 16+ph/2, "%4.4f", soff+secs);
			break;
		case 1:	vscal = (float) br / ph;
			gl_printf(10, 15 + ph / 2, "Ch1");
			gl_line(35, 25+ph, 35+pw, 25+ph, ccolor);
			gl_line(35, 25+ph, 35, 35+ph, ccolor);
			gl_line(35+pw/2, 25+ph, 35+pw/2, 35+ph, ccolor);
			gl_line(35+pw, 25+ph, 35+pw, 35+ph, ccolor);
			gl_printf(35, 21+ph, "%4.4f", soff);
			gl_printf(35+pw-40, 21+ph, "%4.4f", soff+secs);
			break;
		case 2:	vscal = (float) br * ph;
			gl_printf(10, 15 + ph / 2, "Ch2");
			gl_line(35, 25+ph, 35+pw, 25+ph, ccolor);
			gl_line(35, 25+ph, 35, 35+ph, ccolor);
			gl_line(35+pw/2, 25+ph, 35+pw/2, 35+ph, ccolor);
			gl_line(35+pw, 25+ph, 35+pw, 35+ph, ccolor);
			gl_printf(35, 21+ph, "%4.4f", soff);
			gl_printf(35+pw-40, 21+ph, "%4.4f", soff+secs);
			break;
	}

	if ((tw->wtype == 18) || (tw->wtype = 16)) {
		gl_printf(20, 20, "PCM type %d ", tw->wtype);
		ssiz = tw->bps / 8;
		for (bi = poff; bi < psmp + poff; bi++) {
			sampL = 0;
			sx = (int)((bi-poff) / hscal) + 40;
			bj = bi * ssiz * tw->nch;
			bk = bj + ssiz;
			sampR = hex2long(tw->wbuf + bj, ssiz);
			if (tw->nch == 2) sampL =hex2long(tw->wbuf + bk, ssiz); 
			if (ssiz == 1) {
				sampR = sampR - 128;
				sampL = sampL - 128;
			}
			else { if (sampR > 32767) sampR = sampR - 65536;
				if (sampL > 32767) sampL = sampL - 65536;
			}
			switch(chan) {
			  case 1:  // plot channel 1 pixel
				sy = 20 + (int)((br/2 - sampR) / vscal);
				gl_setpixel(sx, sy, ccolor);
				break;
			  case 0: // plot both channels
				sy = 20 + (int)((br/2 - sampR) / vscal);
				gl_setpixel(sx, sy, ccolor);
			  case 2: // plot channel 2 pixel
				if (tw->nch == 2) { sy = 40 +
				  ph / 2 + (int)((br/2 - sampL) / vscal);
				  gl_setpixel(sx, sy, ccolor);
				}
				break;
			}
		}
	}
	else {
		gl_printf(20, 20, "This file is not a PCM...!\n");
	}

};


long int hex2long(unsigned char *thehex, int hlen) { 
	int fi;
	long int hnum = 0, fj;

	for (fi = 0, fj = 1; fi < hlen; fi++, fj = fj*256) {
		hnum += thehex[fi] * fj;
	}
	return (hnum);
}

// NOTES:  '.wav' formats
//
//  (type '10') .wav file format 
//    -> all multibyte values are in LSB/MSB order <-
//  Byte(s)	Description
//  0-3		'RIFF'
//  4-7		<file size in bytes - 8>
//  8-15	'wavefmt '
//  16		10 (file type?)
//  17-21 	00 00 00 01 00  (???)
//  22-23	<number of channels> e.g. mono = 01 00
//  24-27	sample rate/sec   e.g. 8000Hz = 40 1F 00 00
//  28-31	byte rate/sec  e.g. 8000Hz 16bit stereo = 00 7D 00 00
//  32-33	bytes/sample e.g. 16bit stereo = 04 00
//  34-35	bits (not bytes!) per channel
//  36-39	'data' 
//  40-43	# of bytes of data
//  44		first data byte (8bit mono)  0 = 80, -127 = 00
//  44-45	first data word (16bit mono) 0 = 00 00, -65535 = 00 80
//  44/45	L/R channels first sample (8bit stereo)
//  44-45/46-47 L/R channels first sample (16-bit stereo)
//
// new (type '18') .wav file format 
//    -> all multibyte values are in LSB/MSB order <-
//  Byte(s)	Description
//  0-3		'RIFF'
//  4-7		<file size in bytes - 8>
//  8-15	'wavefmt '
//  16		12 (file type?)
//  17-21	00 00 00 01 00  (???)
//  22-23	<number of channels> e.g. mono = 01 00
//  24-27	sample rate/sec   e.g. 8000Hz = 40 1F 00 00
//  28-31	byte rate/sec  e.g. 8000Hz 16bit stereo = 00 7D 00 00
//  32-33	bytes/sample e.g. 16bit stereo = 04 00
//  34-35	bits (not bytes!) per channel
//  36-45	00 00 'fact' 04 00 00 00
//  46-49	# of samples
//  50-53	'data'
//  54-57	# of bytes of data
//  58		first data byte (8bit mono)  0 = 80, -127 = 00
//  58-59	first data word (16bit mono) 0 = 00 00, -65535 = 00 80
//  58/59	L/R channels first sample (8bit stereo)
//  58-59/60-61 L/R channels first sample (16-bit stereo)
//
// end of waveread.c 

