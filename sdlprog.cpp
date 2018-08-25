//
// 3dsdl test program template
//

#include <math.h>
#include <float.h>
// gotcha #1 -- this is just 'iostream', NOT 'iostream.h' anymore
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>  // for, uh, drawing primitives.  filledTrigonRGBA for one...
//  and don't forget the -lSDL2_gfx library, either...

#include "vectoro.cpp"   // to test c++ reworking of vectors.c to .cpp

// gotcha #2 -- add this line to keep from having to use 'std::cout' below instead of just 'cout'
using namespace std;

// prototypes
int main( int argc, char* args[]);

void vdump(char *caption, DVector *v);
DVector *vrand(void);

//
// SDL stuff
//
//Screen dimension constants 
const int SCREENW = 1024; 
const int SCREENH = 600;

// init any class variables that need it
//
int DVector::debug = 0;  // set the debug flag  NOTICE THAT THIS IS OUTSIDE main() !!

int main( int argc, char* args[]) 
{
			// vectoro tests

	DVector v1, v4;
	DVector v2(1.0, 2.0, 3.0);
	DVector v3('x');
	DVector *pv5;

	double dv, vmag;
	char buf[100] = "\0";

	srand(time(NULL));  // yes, there are better ways to do this, but...

	v2.set(1.0, 2.0, 3.0);
	v1 = v2.add(v3);

	// try some on the generic vectors...
	v4 = DVector('r').add(DVector('z'));

	dv = v2.dot(v3);
	vmag = v1.mag();
	cout << dv << "\n" << vmag << "\n\n";
	vdump("V2      : ", &v2);
	vdump("V3      : ", &v3);
	vdump("V2 + V3 : ", &v1); 
	cout << "\nV2 . V3 : " << v2.dot(v3) << "\n";
	cout << "V1 mag  : " << v1.mag() << "\n";
	vdump("\narbitrary unit : ", &v4);
	cout << "\n\n";
	
	pv5 = vrand();
	vdump("V5 random: ", pv5);
	cout << "\n\n";

	// rotation tests
	for (double theta = 0.0; theta < 360.0; theta += 30.0) {
		char zstr[30];
		DVector *pz = new DVector();

		*pz = pv5->rot('x', theta);
		sprintf(zstr, "Rot %4.1f deg abt. x of:", theta);
		vdump(zstr, pz);
	}
	for (double theta = 0.0; theta < 360.0; theta += 30.0) {
		char zstr[30];
		DVector *pz = new DVector();

		*pz = pv5->rot('y', theta);
		sprintf(zstr, "Rot %4.1f deg abt. y of:", theta);
		vdump(zstr, pz);
	}
	for (double theta = 0.0; theta < 360.0; theta += 30.0) {
		char zstr[30];
		DVector *pz = new DVector();

		*pz = pv5->rot('z', theta);
		sprintf(zstr, "Rot %4.1f deg abt. z of:", theta);
		vdump(zstr, pz);
	}

	char dummy;
	cout << "Hit key to continue...\n";
	cin >> dummy;

	// SDL stuff
	//
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
      SDL_Window* window = NULL;
      SDL_Renderer* renderer = NULL;

      if (SDL_CreateWindowAndRenderer(SCREENW, SCREENH, 0, &window, &renderer) == 0) {
          SDL_bool done = SDL_FALSE;

          while (!done) {
              SDL_Event event;

              SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
              SDL_RenderClear(renderer);

              SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
              SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
              SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
              SDL_RenderDrawLine(renderer, 340, 240, 320, 200);

							// now let's see...
							int px1 = 20, py1 = 20;
							int px2 = 500, py2 = 200;
							int px3 = 100, py3 = 300;
              SDL_SetRenderDrawColor(renderer, 100, 55, 55, SDL_ALPHA_OPAQUE);
              SDL_RenderDrawLine(renderer, px1, py1, px2, py2);
              SDL_RenderDrawLine(renderer, px2, py2, px3, py3);
              SDL_RenderDrawLine(renderer, px3, py3, px1, py1);

							filledTrigonRGBA(renderer, px1, py1, px2, py2, px3, py3, 55, 100, 100, SDL_ALPHA_OPAQUE);

              SDL_RenderPresent(renderer);

              while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) {
                      done = SDL_TRUE;
                  }
              }
          }
      }

      if (renderer) {
          SDL_DestroyRenderer(renderer);
      }
      if (window) {
          SDL_DestroyWindow(window);
      }
  }
  SDL_Quit();


	return 0;
}

DVector *vrand(void) {
	double x, y, z;
	DVector *pv;

	x = (((double) rand()) / RAND_MAX - 0.5);
	y = (((double) rand()) / RAND_MAX - 0.5);
	z = (((double) rand()) / RAND_MAX - 0.5);

	pv = new DVector(x, y , z);
	*pv = pv->unit();

	return pv;
}

void vdump(char *caption, DVector *v) {
	char buf[100] = "\0";
	
	v->show(buf);
	cout << caption << buf << "\n";
}

