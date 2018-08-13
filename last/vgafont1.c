// 
// VGA FONT 1 - test a few things with vga
//       text drawing, page flipping, and keyboard reading
//       As well as mouse stuff..

#include <stdio.h>
#include <stdlib.h>
#include <vga.h>
#include <vgagl.h>
#include <vgakeyboard.h>
#include <vgamouse.h>
#include <time.h>

int main () {
	unsigned int dumi;
	unsigned char dumc;
	unsigned long int startt, endt;
	unsigned long int cticks = CLOCKS_PER_SEC;
	float frames, fsecs;
	int vHRES = 512, vVRES = 480;
	int cvmode = G512x480x64K;
	void *thefont;
	long int acol, bcol, ccol;	
	int mx, my, mbut;
	int omx, omy;
	int smx, smy;
	GraphicsContext phys;	


	smx = 32768 / vHRES;
	smy = 32768 / vVRES;

	vga_init();
	vga_setmode(cvmode);
	vga_setlinearaddressing();
	gl_setcontextvga(cvmode);
	gl_getcontext(&phys);

	ccol = vga_white();
	acol = gl_rgbcolor(100, 100, 50);
	bcol = gl_rgbcolor(50, 100, 200);
	thefont = malloc(8 * 8 * 256 * BYTESPERPIXEL); // <- VERY IMPORTANT!

	gl_expandfont(8, 8, ccol, gl_font8x8, thefont);
	gl_setfont(8, 8, thefont);
	
	gl_line(100, 100, 500, 100, acol); //Draw a ruling mark
	gl_line(100, 100, 100, 200, bcol);

	gl_printf(100,100,"This is a test, this is only a test \n");
	gl_colorfont(8, 8, bcol, thefont);
	gl_printf(-1,-1,"of the emergency vga screen text system \n");
	gl_colorfont(8, 8, acol, thefont);
	gl_printf(-1,-1,"  Colors = %d  %d  %d \n", acol, bcol, ccol);	
	gl_printf(-1,-1,"\n");
	dumi = vga_getch();
	dumi = 255; dumc = (char) dumi;
	while (dumc != 'q') {

		dumi = vga_getkey();
		dumc = (char) dumi;
		if (dumi) {
			 gl_printf(-1,-1,"%c ",dumc);
			 dumi = 0;
		}
	}
	dumi = vga_getch();
	// mouse stuff
	// gl_clearscreen(0);
//	mouse_init("/dev/mouse", MOUSE_MICROSOFT, 150);
	vga_setmousesupport(1);
	mbut = MOUSE_LEFTBUTTON;
	omx = 100; omy = 100;
	while (mbut != (MOUSE_LEFTBUTTON | MOUSE_RIGHTBUTTON))
	{
		if (mouse_update()) {
			mx = mouse_getx() / smx;
			my = mouse_gety() / smy;
			mbut = mouse_getbutton();
			gl_printf(100, 300,"  mouseX: %d    ", mx);
			gl_printf(100, 350,"  mouseY: %d    ", my);
			gl_line(omx-2, omy, omx-5, omy, 0);
			gl_line(omx+2, omy, omx+5, omy, 0);
			gl_line(omx, omy-2, omx, omy-5, 0);
			gl_line(omx, omy+2, omx, omy+5, 0);
		
			gl_line(mx-2, my, mx-5, my, acol);
			gl_line(mx+2, my, mx+5, my, acol);
			gl_line(mx, my-2, mx, my-5, acol);
			gl_line(mx, my+2, mx, my+5, acol);

			omx = mx; omy = my;
			switch (mbut) {
				case MOUSE_LEFTBUTTON:
					gl_printf(100,400, " LEFT  ");
					break;
				case MOUSE_RIGHTBUTTON:
					gl_printf(100,450, " RIGHT  ");
					break;
				default:
					gl_printf(100,400, "        ");
					gl_printf(100,450, "        ");
					break;
			}	
		}
		dumi = vga_getkey();
		if (dumi == 113) break;
	}
	mouse_close();
	dumi = vga_getch();
	dumi = 255;
	// now try some page flipping
	gl_clearscreen(0);
	gl_printf(100, 300, "Now hit a key to try some page flipping! \n");
	dumi = vga_getch();
	gl_setcontextvgavirtual(cvmode);
	mx = 320; my = 240;
	frames = 0;
	startt = clock();
	while (1) {
		gl_clearscreen(0);
		
		gl_circle(mx, my, 20, acol);
		mx = mx + (rand() % 16) - 8;
		my = my + (rand() % 16) - 8;
		if (mx > 600) mx = 600;
		if (mx < 40) mx = 40;
		if (my > 440) my = 440;
		if (my < 40) my = 40;
		gl_copyscreen(&phys);
		frames = frames + 1;
		dumi = vga_getkey();
		if (dumi == 113) break;
	}
	endt = clock();
	fsecs = (float) (endt - startt) / cticks;
	gl_setcontext(&phys);
	gl_printf(100,200,"Frame rate = %f \n" , frames / fsecs );
	dumi = vga_getch();
	vga_setmode(TEXT);
	gl_freecontext(&phys);
	free(thefont);
}
