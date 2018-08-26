//
// 3dsdl test program template, ver 2
//
// 8/25/2018
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


int main( int argc, char* args[]);

//
// SDL stuff
//
//Screen dimension constants 
const int SCREENW = 1024; 
const int SCREENH = 600;

// init any class variables that need it
//
int DVector::debug = 0;  // set the debug flag  NOTICE THAT THIS IS OUTSIDE main() !!

int main( int argc, char* args[]) {

	char buf[100] = "\0";
	Uint32 startTime = 0;
	long int frcnt = 0;
	double ftime = 0.0;

	srand(time(NULL));  // yes, there are better ways to do this, but...

	// SDL stuff
	//
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_CreateWindowAndRenderer(SCREENW, SCREENH, 0, &window, &renderer) == 0) {
      SDL_bool done = SDL_FALSE;

			// now let's see...define three points
			int px1 = 20, py1 = 20;
			int px2 = 500, py2 = 200;
			int px3 = 100, py3 = 300;

			startTime = SDL_GetTicks();
      while (!done) {
        SDL_Event event;

				while (SDL_PollEvent(&event)) {  // handle the event
          if (event.type == SDL_QUIT) {
              done = SDL_TRUE;
          }
						//User presses a key 
					else if( event.type == SDL_KEYDOWN ) { 
						//Select surfaces based on key press 
						cout << "elapsed time (ms): " << SDL_GetTicks() - startTime << "\n";

						switch( event.key.keysym.sym ) { 


							case SDLK_UP: cout << "up\n"; py1 -= 4; py2 -= 4; py3 -=4;
								break; 
							case SDLK_DOWN: cout << "down\n"; py1 += 4; py2 += 4; py3 +=4;
								break; 
							case SDLK_LEFT: cout << "left\n"; px1 -= 4; px2 -= 4; px3 -=4;
								break; 
							case SDLK_RIGHT: cout << "right\n"; px1 += 4; px2 += 4; px3 +=4;
								break; 
							default: ; break; 
						} 
					}
						// User does something with mouse
					else if( event.type == SDL_MOUSEMOTION || 
						event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ) {
						//Get mouse position 
						int x, y; 
						cout << "elapsed time (ms): " << SDL_GetTicks() - startTime << "\n";

						SDL_GetMouseState( &x, &y );
						// check to see if motion is inside test area
						cout << "mouse is at" << x << ":" << y << "\n";
						switch (event.type) {									
							case SDL_MOUSEMOTION: cout << "mouse is moving\n";
								break;
							case SDL_MOUSEBUTTONDOWN: cout << "button down\n";
								break;
							case SDL_MOUSEBUTTONUP: cout << "button up\n";
								break;
						}
					}
				} // end of event handler

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // set to black and full alpha
        SDL_RenderClear(renderer);  // clear screen
					//
					// ** below here, DRAW stuff

        SDL_SetRenderDrawColor(renderer, 100, 55, 55, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, px1, py1, px2, py2);
        SDL_RenderDrawLine(renderer, px2, py2, px3, py3);
        SDL_RenderDrawLine(renderer, px3, py3, px1, py1);

				filledTrigonRGBA(renderer, px1, py1, px2, py2, px3, py3, 55, 100, 100, SDL_ALPHA_OPAQUE);

					// and then flip it onto the screen
				SDL_RenderPresent(renderer);

				frcnt++;
				if(frcnt > 100) {
					frcnt = 0;
					ftime = 1000.0 / (SDL_GetTicks() - startTime);
					startTime = SDL_GetTicks();
					cout << "Frame / sec = " << 100.0 * ftime << "\n";
				}
				
			}
    }  // and we are 'done' (user quit)
		if (renderer) SDL_DestroyRenderer(renderer);
		if (window) SDL_DestroyWindow(window);
  }

  SDL_Quit();

	return 0;
}
