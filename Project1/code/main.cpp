#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "libreriaAutonoleggio.h"

#include "libreriaAutonoleggio.h"

using namespace std;

int main(int argc, char* argv[]) {
	string name = "ferrari";
	readCar();
	searchByName(name);
	stampDebug();
	//generazioneFinestra();
	//addCar();
	//printStruct();
	return 0;
}