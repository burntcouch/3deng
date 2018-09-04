//
// 3ddisp.h
//
//
//  header appropriate stuff for 3ddisp.cpp
//
// protos?

class Env3D;
typedef Env3D *pEnv3D;
//
//*** Env3D
//
class Env3D {
	public:
		Env3D();
		~Env3D();

		DVector opos;						// observer position
		DVector ovel;						// observer velocity
		DVector oax;						// observer accel
		DVector oiv;						// obs. orientation frame. +i is 'forward'
		DVector ojv;						// +j is 'left'
		DVector okv;						// +k is 'up'

		ObjList *olist;					// object list

		Obj3D *lites;						// lights	list
		double lvc;		  				// lighting range scale: 5 typical
		double lvr;		  				// lighting range: 32 typical
		bool liteson;		  			// do lighting? if not, use Ghod shading
		int drawmode;		  			// 0 - wire frame, 1 - solid, 2 - shaded		

		SDL_Renderer *render;		// renderer struct
		SDL_Window *window;			// window struct

		XYCrd mouse;					// last mouse position
		int key;							// last key
		int kmod;							// last key modifier (shift, alt, etc.)

		int startTime;				// start ticks
		double delta;					// time scale
		int frcnt;						// frame count
		double frate;					// latest frame rate
		int fskip;						// how many frames to skip between moves

	protected:
		XYCrd winsize;					// dimensions of window
		int hScrn;	 	  				// horizontal view res. (clip region)
		int vScrn;	 	  			  // vertical view res. (clip region)
		double vscal;					  // pixels per meter -- 1.875 * 2 * hScrn 
};

//
// V2Screen  a 'result' object when rendering a face to a trigon
//
class V2Screen;
typedef V2Screen *pV2Screen;

class V2Screen {
	public:
		V2Screen();
		~V2Screen();
		
		pFace3D face;
		pTrig3D trig;
		long area;
		double behind;
		double toward;
		int offscrn;
};

#define ZSTART 100
#define ZGROW 100

// qsort compare function prototypes for z-sorting

int ZSCompS(const void * a, const void * b);  // the compare function

pV2Screen vec3D_to_screen(pEnv3D wenv, pFace3D f); // converting a face to a screen trigon

int vec_to_screen(pEnv3D wenv, DVector v, XYCrd *xy); // convert one object position vector to a pixel

int draw_Zsort(pEnv3D wenv);  // do the z-sort and draw

int draw_Wire(pEnv3D wenv);  // draw a wire frame of all

// end of 3ddisp.h
