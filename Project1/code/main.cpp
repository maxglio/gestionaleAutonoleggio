#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "libreriaAutonoleggio.h"

using namespace std;



int main(int argc, char* argv[]) {
	int data[3];
	int ora[2];
	getCurrentDate(data);
	getCurrentTime(ora);
	return 0;
}