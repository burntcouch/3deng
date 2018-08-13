//
// 3dsdl test program
//

#include <math.h>
#include <float.h>
// gotcha #1 -- this is just 'iostream', NOT 'iostream.h' anymore
#include <iostream>
#include <SDL2/SDL.h>

// gotcha #2 -- add this line to keep from having to use 'std::cout' below instead of just 'cout'
using namespace std;

// prototypes  - three different squares, and the compiler knows which one to use based on proto
//
int square(int i) ;
float square(float f);
double square(double d) ;

// definitions
int square(int i) 
{
	return i*i;
}
float square(float f) 
{
	return f*f;
}
double square(double d) 
{
	return d*d;
}

int main() 
{
	int    i = 2;
	float  f = 3.0;
	double d = 4.0;
	i = square(i);   //calls square for ints
	f = square(f);   //calls square for floats
	d = square(d);   //calls square for doubles
	
	cout << i << "\n" << f << "\n" << d << "\n\n";

	return 0;
}