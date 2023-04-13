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
int generazioneFinestra() {
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Failed to initialize" << SDL_GetError() << std::endl;
		return 1;
	}
	std::atexit(&SDL_Quit);
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
	SDL_Surface* button = SDL_LoadBMP("newgame.bmp");
	SDL_Rect cButton;
	cButton.x = 220;
	cButton.y = screen->h / 2;
	cButton.w = button->w;
	cButton.h = button->h;
	/*Set the current gamestate, in this case its Intro*/
	gamestate current = Intro;
	while (current != Exit) /*Game Loop*/
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (current) /*Switch the current state*/
			{
			case Intro:
				if (event.type == SDL_QUIT)
				{
					current = Exit;
				}
				/*Here we check if the mouse is being clicked on the button*/
				if ((event.button.button & Button) == Button && intersects(cButton.x, cButton.y, cButton.w, cButton.h, event.button.x, event.button.y))
				{
					current = Main; /*If it is true we change the current state*/
				}
				break;
			case Main:
				if (event.type == SDL_QUIT)
				{
					current = Exit;
				}
				break;
			}
		}
		switch (current)
		{
		case Intro:
			SDL_FillRect(screen, &screen->clip_rect, 000000);
			SDL_BlitSurface(button, NULL, screen, &cButton); /*We draw the icon*/
			break;
		case Main:
			SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
			break;
		}
		SDL_Flip(screen);
	}
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

	//se è andato tutto bene
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