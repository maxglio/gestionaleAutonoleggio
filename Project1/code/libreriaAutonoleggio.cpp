#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <fstream>
#include <ctime>
#include <cstdlib>

#include "libreriaAutonoleggio.h"

#define NMACCHINE 1

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
	int neopatentato;
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
	//HAHAHAHAHA
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

int check_click_in_rect(int x, int y, struct SDL_Rect* rect){
	/* Check X coordinate is within rectangle range */
	if (x >= rect->x && x < (rect->x + rect->w))
	{
		/* Check Y coordinate is within rectangle range */
		if (y >= rect->y && y < (rect->y + rect->h))
		{
			/* X and Y is inside the rectangle */
			return 1;
		}
	}

	/* X or Y is outside the rectangle */
	return 0;
}

int generazioneFinestra() {
	int xMouse, yMouse;
	bool quit = false;
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Point* p;


	SDL_Rect dstrect;

	dstrect.x = 10;
	dstrect.y = 10;
	dstrect.w = 83;
	dstrect.h = 32;


	//HAHAHAHAHAH
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);

	SDL_DisplayMode dm;


	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return 1;
	}

	int w, h;
	w = dm.w;
	h = dm.h;

	printf("\n\nw=%d h=%d", w, h);

	SDL_Window* screen = SDL_CreateWindow("Autonoleggio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);

	

	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	// Select the color for drawing. It is set to red here.
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);

	// Clear the entire screen to our selected color.
	SDL_RenderClear(renderer);

	// Up until now everything was drawn behind the scenes.
	// This will show the new, red contents of the window.
	SDL_RenderPresent(renderer);

	SDL_RenderDrawRect(renderer, &dstrect);

	SDL_Surface* ExitSurface = IMG_Load("exit.png");

	if (ExitSurface == NULL) {
		cout << "Error loading image: " << IMG_GetError();
		return 5;
	}

	SDL_Texture* ExitTexture = SDL_CreateTextureFromSurface(renderer, ExitSurface);

	if (ExitSurface == NULL) {
		cout << "Error creating texture";
		return 6;
	}

	SDL_FreeSurface(ExitSurface);

	

	while (!quit){

		SDL_GetGlobalMouseState(&xMouse, &yMouse);
		printf("\ny = %d - x = %d", xMouse, yMouse);
		SDL_WaitEvent(&event);

		if (check_click_in_rect(xMouse, yMouse, &dstrect) == 1) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					break;
				}
			}
			
		}

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, ExitTexture, NULL, &dstrect);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyTexture(ExitTexture);

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(screen);

	IMG_Quit();

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
	int a;
	
	errno_t error_code;
	FILE* pf;
	FILE* aa;
	//apro il file
	error_code = fopen_s(&pf, "cars.dat", "w");

	//prendo in input tutto
	for (int i = 0; i < NMACCHINE; i++) {
		cout << "inserire la marca: ";
		cin >> garage[i].marca;
		cout << "inserire il modello: ";
		cin >> garage[i].modello;
		cout << "inserire il carburante1: ";
		cin >> garage[i].carburante;
		cout << "inserire il km1: ";
		cin >> garage[i].km;
		cout << "inserire il annoMatricolazione1: ";
		cin >> garage[i].annoMatricolazione;
		cout << "inserire il potenza1: ";
		cin >> garage[i].potenza;
		cout << "inserire il cambio1: ";
		cin >> garage[i].cambio;
		cout << "inserire il fumatori1: ";
		cin >> garage[i].fumatori;
		cout << "inserire il usato1: ";
		cin >> garage[i].usato;
		cout << "inserire il posti1: ";
		cin >> garage[i].posti;
		cout << "inserire il porte1: ";
		cin >> garage[i].porte;
		if (garage[i].potenza > 95){
			garage[i].neopatentato = 0;
		}
		else if(garage[i].potenza <= 95){
			garage[i].neopatentato = 1;
		}
	}

	//se � andato tutto bene
	if (error_code == 0) {
		//scrivo
		fwrite(&garage, sizeof(garage), 1, pf);
		//chiudo il file
		fclose(pf);
	}
	
	cout << endl << "error code cars: " << error_code << endl;

	error_code = fopen_s(&aa, "cars.dat", "r");
	if (error_code == 0) {
		size_t success = fread(&garage, sizeof(garage), 1, aa);
		fclose(aa);
		cout << "success: " << success;
	}
}
//FINE FILE BINARI