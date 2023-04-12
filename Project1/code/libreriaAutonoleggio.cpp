#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>

#include "libreriaAutonoleggio.h"

using namespace std;

int endDateCalculator(int inizio[], int fine[]){
	int durataMesi;
	//prendo in input il numero di mesi del noleggio;
	do{
		cout << "Quanti mesi vuoi che duri il noleggio[min: 3 - max: 18]: ";
		cin >> durataMesi;

	}while (durataMesi < 3 || durataMesi > 18);

	//il numero del giorno non cambia mai
	fine[0] = inizio[0];

	//calcolo il numero di mesi
	fine[1] = inizio[1] + durataMesi;
	//aggiungo gli anni
	fine[2] = inizio[2];
	//per sistemare gli anni
	if(fine[1] > 12 && fine[1] < 25){
		fine[2] += 1;
	}
	else if(fine[1] >= 25){
		fine[2] += 2;
	}

	fine[1] %= 12;

	return 	durataMesi;
}





int generazioneFinestra(){
	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* screen = SDL_CreateWindow("Autonoleggio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	SDL_ShowSimpleMessageBox(0, "porocdio", SDL_GetError(),screen);

	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
	}

	SDL_Quit();

	return 0;
}