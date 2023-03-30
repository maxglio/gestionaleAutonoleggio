#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

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
	SDL_Window* window = nullptr;
	SDL_Surface* windowSurface = nullptr;
	SDL_Surface* imageSurface = nullptr;


	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "video initializzation Error: " << SDL_GetError() << std::endl;
	}
	else {
		window = SDL_CreateWindow("gestionale autonoleggio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1800, 1000, SDL_WINDOW_SHOWN);
		SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (window == NULL) {
			std::cout << "Window creatiuon error: " << SDL_GetError() << std::endl;
		}
		else {

			windowSurface = SDL_GetWindowSurface(window);
			imageSurface = SDL_LoadBMP("coolHue-FD6585-0D25B9.bmp");

			if (imageSurface == NULL) {
				std::cout << "image loading error: " << SDL_GetError() << std::endl;
			}
			else {
				SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
				SDL_UpdateWindowSurface(window);
				SDL_Delay(10000);
			}


		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}