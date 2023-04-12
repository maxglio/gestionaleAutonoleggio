#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <fstream>

#include "libreriaAutonoleggio.h"

using namespace std;

struct macchina {
	string marca;
	string modello;
	string carburante;
	int id;
	int km;
	int annoMatricolazione;
	int dataInizioNoleggio[3];
	int durataNoleggio;
	int potenza;
	int cambio;
	int fumatori;
	int usato;
	int posti;
	int porte;
};

struct id {
	int id;
};


//DATE
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
//FINE DATE




//INTERFACCIA GRAFICA
int generazioneFinestra(){
	bool quit = false;
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Init(SDL_INIT_VIDEO);

	SDL_DisplayMode dm;
	SDL_Window* screen = SDL_CreateWindow("Autonoleggio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	SDL_ShowSimpleMessageBox(0, "porocdio", SDL_GetError(),screen);

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return 1;
	}

	int w, h;
	w = dm.w;
	h = dm.h;

	printf("w=%d h=%d", w, h);

	SDL_Window *screen = SDL_CreateWindow("Autonoleggio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);

	SDL_ShowSimpleMessageBox(0, "aaaaaaaaaa", SDL_GetError(),screen);

	renderer = SDL_CreateRenderer(screen, -1, 0);
	// Select the color for drawing. It is set to red here.
	SDL_SetRenderDrawColor(renderer, 30, 30, 30 ,255);

	// Clear the entire screen to our selected color.
	SDL_RenderClear(renderer);

	// Up until now everything was drawn behind the scenes.
	// This will show the new, red contents of the window.
	SDL_RenderPresent(renderer);
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
//FINE INTERFACCIA GRAFICA

//FILE BINARI
void refreshID() {
	ofstream wf("/binaryFiles/id.dat", ios::out | ios::binary);
	id wstu[1];
	wstu[0].id = 0;
	wf.write((char*)&wstu[0], sizeof(id));
	wf.close();
	ifstream rf("/binaryFiles/id.dat", ios::out | ios::binary);
}
//FINE FILE BINARI