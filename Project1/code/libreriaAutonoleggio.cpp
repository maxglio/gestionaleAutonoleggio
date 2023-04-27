#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>

#include "libreriaAutonoleggio.h"

#define NMACCHINE 2


using namespace std;

struct Automobile {
	string marca;
	string modello;
	string carburante;
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
	int prezzo;
};

struct Automobile garage[NMACCHINE];
struct Automobile garageLettura[NMACCHINE];

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
	bool filterControl = false;






	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_DisplayMode dm;



	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return 1;
	}

	int w, h;
	w = dm.w;
	h = dm.h;

	int hDIv2 = h / 2;

	printf("\n\nw=%d h=%d", w, h);

	cout << endl << "w / 2 = " << hDIv2;


#define HRECTSX (6/100.0)*h
	cout << endl << "HRECTSX = " << HRECTSX;
#define WRECTSX (5/100.0)*w
	cout << endl << "WRECTSX = " << WRECTSX;


	SDL_Rect hideRect;
	hideRect.x = WRECTSX;
	hideRect.y = 0;
	hideRect.w = WRECTSX;
	hideRect.h = HRECTSX;

	SDL_Rect exitRect;
	exitRect.x = 0;
	exitRect.y = 0;
	exitRect.w = WRECTSX;
	exitRect.h = HRECTSX;

	SDL_Rect sqrRect;
	sqrRect.x = (WRECTSX + WRECTSX);
	sqrRect.y = 0;
	sqrRect.w = WRECTSX;
	sqrRect.h = HRECTSX;

	SDL_Rect titleRect;
	titleRect.w = ((36 / 100.0) * w);
	titleRect.x = ((w / 2)-(titleRect.w/2));
	titleRect.y = 0;
	titleRect.h = (12/100.0)*h;

	SDL_Rect searchRect;
	searchRect.w = (41/100.0)*w;
	searchRect.x = ((w / 2) - (searchRect.w / 2));
	searchRect.y = (18/100.0)*h;
	searchRect.h = (12/100.0)*h;

	SDL_Rect searchIconRect;
	searchIconRect.w = (6.75 / 100.0) * w;
	searchIconRect.x = ((w / 2) - (searchRect.w / 2) - searchIconRect.w);
	searchIconRect.y = (18 / 100.0) * h;
	searchIconRect.h = (12 / 100.0) * h;

	SDL_Rect filterRect;
	filterRect.x = searchRect.x + searchRect.w + ((3 / 100.0)*w);
	filterRect.y = searchRect.y;
	filterRect.w = searchRect.h;
	filterRect.h = searchRect.h;

	SDL_Rect bigRect;
	bigRect.x = (1.5/100.0)*w;
	bigRect.y = (36/100.0)*h;
	bigRect.w = (w - bigRect.x)- bigRect.x;
	bigRect.h = (h - bigRect.y)- bigRect.x;


	SDL_Window* screen = SDL_CreateWindow("Autonoleggio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_POPUP_MENU);

	

	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	// Select the color for drawing. It is set to red here.
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);

	// Clear the entire screen to our selected color.
	SDL_RenderClear(renderer);

	// Up until now everything was drawn behind the scenes.
	// This will show the new contents of the window.
	SDL_RenderPresent(renderer);

	//start rectangle rendering
		
	//SQUARE BUTTON
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &sqrRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//LOW BUTTON
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &hideRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//EXIT BUTTON
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &exitRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//TITLE
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &titleRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//SEARCH BOX ICON
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(renderer, &searchIconRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//SEARCH BOX
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(renderer, &searchRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//FILTER
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &filterRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//BIG RECTANGLE
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &bigRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);


	//end rectangle rendering

	// WINDOW ICON
	SDL_Surface* IconSurface = IMG_Load("code/images/icon.png");
	SDL_SetWindowIcon(screen, IconSurface);

	// EXIT BUTTON RENDERING
	SDL_Surface* ExitSurface = IMG_Load("code/images/exit.png");
	SDL_Texture* ExitTexture = SDL_CreateTextureFromSurface(renderer, ExitSurface);
	SDL_FreeSurface(ExitSurface);

	//TITLE RENDERING
	TTF_Font* TitleFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 24);
	SDL_Color TitleColor = { 255, 255, 255 };
	SDL_Surface* TitleSurface = TTF_RenderText_Solid(TitleFont, "EM Autonoleggio", TitleColor);
	SDL_Texture* TitleTexture = SDL_CreateTextureFromSurface(renderer, TitleSurface);
	SDL_FreeSurface(TitleSurface);


	//SEARCH ICON RENDERING
	SDL_Surface* SearchIconSurface = IMG_Load("code/images/searchIcon.png");
	SDL_Texture* SearchIconTexture = SDL_CreateTextureFromSurface(renderer, SearchIconSurface);
	SDL_FreeSurface(SearchIconSurface);
	
	//FILTER ICON RENDERING
	SDL_Surface* FilterIconSurface = IMG_Load("code/images/filterIconON.png");
	SDL_Texture* FilterIconTexture = SDL_CreateTextureFromSurface(renderer, FilterIconSurface);
	SDL_FreeSurface(FilterIconSurface);

	

	while (!quit){

	//Get Mouse POS
		SDL_GetGlobalMouseState(&xMouse, &yMouse);
		printf("\ny = %d - x = %d", xMouse, yMouse);
		SDL_WaitEvent(&event);
	//--------------------------------------------------


	//EXIT
		if (check_click_in_rect(xMouse, yMouse, &exitRect) == 1) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					system("taskkill /IM gestionaleAutonoleggio.exe");
				}
			}
		}

	//HIDE
	//square

	//SEARCH ICON
		if (check_click_in_rect(xMouse, yMouse, &filterRect) == 1) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {

					switch (filterControl){
					case false:
						cout << "bbbbbbbbbbbbbbbbbbb";
						//COVER
						FilterIconSurface = IMG_Load("code/images/cover.png");

						FilterIconTexture = SDL_CreateTextureFromSurface(renderer, FilterIconSurface);

						SDL_RenderCopy(renderer, FilterIconTexture, NULL, &filterRect);

						SDL_RenderPresent(renderer);

						SDL_UpdateWindowSurface(screen);
						//COVER

						FilterIconSurface = IMG_Load("code/images/filterIconOFF.png");

						FilterIconTexture = SDL_CreateTextureFromSurface(renderer, FilterIconSurface);

						SDL_RenderCopy(renderer, FilterIconTexture, NULL, &filterRect);

						SDL_RenderPresent(renderer);

						SDL_UpdateWindowSurface(screen);

						
						filterControl = true;
						break;
					case true:
						cout << "aaaaaaaaaaaaaaaaaa";
						
						//COVER
						FilterIconSurface = IMG_Load("code/images/cover.png");

						FilterIconTexture = SDL_CreateTextureFromSurface(renderer, FilterIconSurface);

						SDL_RenderCopy(renderer, FilterIconTexture, NULL, &filterRect);

						SDL_RenderPresent(renderer);

						SDL_UpdateWindowSurface(screen);
						//COVER

						FilterIconSurface = IMG_Load("code/images/filterIconON.png");

						FilterIconTexture = SDL_CreateTextureFromSurface(renderer, FilterIconSurface);

						SDL_RenderCopy(renderer, FilterIconTexture, NULL, &filterRect);

						SDL_RenderPresent(renderer);

						SDL_UpdateWindowSurface(screen);
						filterControl = false;
						break;

					default:
						break;
					}
				}
			}
		}

	


		switch (event.type)
		{ 
		case SDL_QUIT:
			quit = true;
			system("taskkill /IM gestionaleAutonoleggio.exe");
		}

		SDL_RenderCopy(renderer, TitleTexture, NULL, &titleRect);

		SDL_RenderPresent(renderer);

		SDL_RenderCopy(renderer, ExitTexture, NULL, &exitRect);

		SDL_RenderPresent(renderer);

		SDL_RenderCopy(renderer, FilterIconTexture, NULL, &filterRect);

		SDL_RenderPresent(renderer);

		SDL_RenderCopy(renderer, SearchIconTexture, NULL, &searchIconRect);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyTexture(FilterIconTexture);

	SDL_DestroyTexture(ExitTexture);

	SDL_DestroyTexture(SearchIconTexture);

	SDL_DestroyTexture(TitleTexture);

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(screen);


	IMG_Quit();

	SDL_Quit();


	return 0;
}
//FINE INTERFACCIA GRAFICA

//FILE BINARI

void addCar() {
	ofstream fileWrite;
	//apro/creo il file
	fileWrite.open("cars.txt", ios::app);
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
		cout << "0 manuale - 1 semiautomatica - 2 automatico" << endl;
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
		cout << "inserire il prezzo: ";
		cin >> garage[i].prezzo;
		if (garage[i].potenza > 95) {
			garage[i].neopatentato = 0;
		}
		else if (garage[i].potenza <= 95) {
			garage[i].neopatentato = 1;
		}
		cout << "----------------------------" << endl;

		//scrivo tuttto sul file
		fileWrite << garage[i].marca << "\n";
		fileWrite << garage[i].modello << "\n";
		fileWrite << garage[i].carburante << "\n";
		fileWrite << garage[i].km << "\n";
		fileWrite << garage[i].annoMatricolazione << "\n";
		fileWrite << garage[i].potenza << "\n";
		fileWrite << garage[i].cambio << "\n";
		fileWrite << garage[i].fumatori << "\n";
		fileWrite << garage[i].usato << "\n";
		fileWrite << garage[i].posti << "\n";
		fileWrite << garage[i].porte << "\n";
		fileWrite << garage[i].neopatentato << "\n";
		fileWrite << garage[i].prezzo << "\n";
		fileWrite << "ÿ";
	}
	fileWrite.close();
}

void readCar() {
	ifstream fileRead("cars.txt");

	string line;

	if (fileRead.is_open()) {
		while (getline(fileRead, line))
		{
			cout << line << '\n';
		}
	}
	else {
		cout << "Errore nell'apertura del file";
	}
}
//FINE FILE BINARI