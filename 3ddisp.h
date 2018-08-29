//
// 3ddisp.h
//
//
//  header appropriate stuff for 3ddisp.cpp
//
// protos?

class Env3D;
class Disp3D;

typedef Env3D *pEnv3D;
typedef Disp3D *pDisp3D;
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

		ObjList *olist;				// object list
		Obj3D *lites;				// lights	list
		double lvc;		  				// lighting range scale: 5 typical
		double lvr;		  				// lighting range: 32 typical
		bool liteson;		  			// do lighting? if not, use Ghod shading
		int drawmode;		  			// 0 - wire frame, 1 - solid, 2 - shaded		

	protected:

};

//
//*** Disp3D
//
class Disp3D {
	public:
		Disp3D();
		~Disp3D();

		XYCrd winsize;							// dimensions of window
		double delta;		  // time scale
		int hScrn;	 	  // horizontal view res. (clip region)
		int vScrn;	 	  // vertical view res. (clip region)
		double vscal;		  // pixels per meter -- 1.875 * 2 * hScrn 

		SDL_Renderer *render;		// renderer struct
		SDL_Window *window;			// window struct

		XYCrd mouse;							// last mouse position
		int key;							// last key
		int kmod;							// last key modifier (shift, alt, etc.)

		int startTime;				// start ticks
		double tscale;				// time scale
		int frcnt;						// frame count
		double frate;					// latest frame rate
		int fskip;						// how many frames to skip between moves
	
	protected:

};

// end of 3ddisp.h
