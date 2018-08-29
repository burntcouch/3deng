//
// 3dbase.h
//

//
//*** XYCrd - convenience object for SDL rendering
//

class XYCrd {
	public:
		XYCrd();
		XYCrd(int ix, int iy);
		~XYCrd();
		int x;
		int y;
};

//
//*** Colora - convenience object to handle color in rendering
//

class Colora {
	public:
		Colora();
		Colora(unsigned int ir, unsigned int ig, unsigned int ib, unsigned int ia);
		~Colora();
		unsigned int r, g, b, a;

		void fade(int howmuch);
		void bright(int howmuch);
};
// Colora protos
//
void clear_to_Colora(SDL_Renderer * r, Colora c);

//
// *** Trig3D - convenience object for SDL rendering
//
class Trig3D {
	public:
		Trig3D();
		~Trig3D();
		void push(XYCrd * apoint);
		void move(int x, int y);
		
		Colora color;
		XYCrd pts[3];		
};

// Trig3D protos
void draw_Trig3D(SDL_Renderer * renderer, Trig3D *trid);  // a wrapper to draw this stuff easier

// Z-sort array declaration and qsort compare function


class ZS3D;
typedef ZS3D pZS3D;

class ZS3D {
	public:
		ZS3D();
		~ZS3D(); 
	
		double z;
		pFace3D zf;		

};

ZS3D::ZS3D() {}
ZS3D::~ZS3D() {}

long *ZSX;

// qsort compare function prototypes for z-sorting

int ZSCompS(pZS3D zarr, const void * a, const void * b);  // current Z-sort uses this one
int ZSCompS2(const void * a, const void * b);

// end of 3dbase.h
//


