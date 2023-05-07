#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "libreriaAutonoleggio.h"

#include "libreriaAutonoleggio.h"

using namespace std;

int main(int argc, char* argv[]) {
	char* prova;
	int i = 4;

	prova = intToChar(i);
	cout<< prova;

	readCar();
	//searchByName(name);
	//stampDebug();
	generazioneFinestra();
	//addCar();
	//readCar();
	//printStruct();
	return 0;
}