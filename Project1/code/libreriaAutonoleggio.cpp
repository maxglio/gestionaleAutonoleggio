#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <fstream>

#include "libreriaAutonoleggio.h"

#define NMACCHINE 100

using namespace std;

struct Automobile {
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

struct Automobile garage[NMACCHINE];

//DATE
void endDateCalculator(int inizio[], int fine[]){
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

}

void getCurrentDate(int data[]) {
	time_t current_time = time(nullptr);
	tm* local_time = localtime(&current_time);
	data[0] = (local_time->tm_mday);
	data[1] = (local_time->tm_mon + 1);
	data[2] = (local_time->tm_year + 1900);
}

void getCurrentTime(int ora[]) {
	time_t current_time = time(nullptr);
	tm* local_time = localtime(&current_time);
	ora[0] = (local_time->tm_hour);
	ora[1] = (local_time->tm_min);
}
//FINE DATE




//INTERFACCIA GRAFICA
int generazioneFinestra(){
	bool quit = false;
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Init(SDL_INIT_VIDEO);

	SDL_DisplayMode dm;


	if (SDL_GetDesktopDisplayMode(0, &dm) != 0){
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return 1;
	}

	int w, h;
	w = dm.w;
	h = dm.h;

	printf("\n\nw=%d h=%d", w, h);

	SDL_Window *screen = SDL_CreateWindow("Autonoleggio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);

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
	int prova = 0;
	int a;
	errno_t error_code;
	FILE* pf;
	error_code = fopen_s(&pf, "id.dat", "w+");
	if (error_code == 0) {
		fwrite(&prova, sizeof(int), 1, pf);
		fclose(pf);
	}
	
	cout << endl << "error code id: " << error_code << endl;
	error_code = fopen_s(&pf, "id.dat", "r");
	fread(&a, sizeof(int), 1, pf);
	fclose(pf);
	cout << "risultato id:" << a;
}

void addCar(){
	int prova = 0;
	int a;
	errno_t error_code;
	FILE* pf;
	error_code = fopen_s(&pf, "cars.dat", "a");
	if (error_code == 0) {
		fwrite(&prova, sizeof(int), 1, pf);
		fclose(pf);
	}

	cout << endl << "error code cars: " << error_code << endl;
	error_code = fopen_s(&pf, "cars.dat", "r");
	fread(&a, sizeof(int), 1, pf);
	fclose(pf);
	cout << "risultato cars:" << a;
}
//FINE FILE BINARI