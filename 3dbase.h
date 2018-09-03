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

XYCrd::XYCrd() {
	x = 0;
	y = 0;
}

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
		Trig3D(XYCrd p1, XYCrd p2, XYCrd p3);
		~Trig3D();
		void push(XYCrd apoint);
		void move(int x, int y);
		void pixelize(int p);
		long area();
		bool pixel();

		Colora color;
		XYCrd pts[3];		
};

// Trig3D protos
void draw_Trig3D(SDL_Renderer * renderer, Trig3D *trid);  // a wrapper to draw this stuff easier

long Trig3D::area();  // calc the area in pixels of the trigon

bool Trig3D::pixel(); // is this just one pixel?

void Trig3D::pixelize(int p); // convert to one pixel

// end of 3dbase.h
//
