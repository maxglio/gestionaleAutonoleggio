#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>

#include "libreriaAutonoleggio.h"

#define NMACCHINE 100


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
struct Automobile garageRicerca[NMACCHINE];

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
	int page = 0;
	
	char content[100] = {'s','b','c','d'};


	

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


#define HRECTSX (6/100.0)*h
#define WRECTSX (5/100.0)*w
#define PERC10W ((0.8 / 100.0) * w)
#define PERC10H ((0.9 / 100.0) * h)


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
	titleRect.w = ((46 / 100.0) * w);
	titleRect.x = ((w / 2)-(titleRect.w/2));
	titleRect.y = 0;
	titleRect.h = (12/100.0)*h;

	SDL_Rect searchRect;
	searchRect.w = (41/100.0)*w;
	searchRect.x = ((w / 2) - (searchRect.w / 2));
	searchRect.y = (15/100.0)*h;
	searchRect.h = (12/100.0)*h;

	SDL_Rect searchIconRect;
	searchIconRect.w = (6.75 / 100.0) * w;
	searchIconRect.x = ((w / 2) - (searchRect.w / 2) - searchIconRect.w);
	searchIconRect.y = searchRect.y;
	searchIconRect.h = (12 / 100.0) * h;

	SDL_Rect bigRect;
	bigRect.x = (5/100.0)*w;
	bigRect.y = (29/100.0)*h;
	bigRect.w = (w - bigRect.x)- bigRect.x;
	bigRect.h = (h - bigRect.y) - ((1.5 / 100.0) * w);

	SDL_Rect leftArrRect;
	leftArrRect.w = (3 / 100.0)*w;
	leftArrRect.x = PERC10W;
	leftArrRect.y = (bigRect.y + (bigRect.h / 2)) - (leftArrRect.w / 2);
	leftArrRect.h = (6 / 100.0) * h;

	SDL_Rect rightArrRect;
	rightArrRect.w = (3 / 100.0) * w;
	rightArrRect.x = w - rightArrRect.w - PERC10W;
	rightArrRect.y = (bigRect.y + (bigRect.h / 2)) - (leftArrRect.w / 2);
	rightArrRect.h = (6 / 100.0) * h;

	SDL_Rect listRect;
	listRect.w = (w - bigRect.x) - bigRect.x - (leftArrRect.w) ;
	listRect.x = (w / 2)-(listRect.w/2);
	listRect.y = bigRect.y + PERC10H;
	listRect.h = (12.7/100.0)*h;

	SDL_Rect nomeRect;
	nomeRect.x = listRect.x + PERC10W;
	nomeRect.y = listRect.y + PERC10H;
	nomeRect.w = 220;
	nomeRect.h = (listRect.h - (PERC10W * 3)) / 3;

	SDL_Rect marcaRect;
	marcaRect.x = (listRect.x + PERC10W) + nomeRect.w + 6;
	marcaRect.y = listRect.y + PERC10H;
	marcaRect.w = 220;
	marcaRect.h = (listRect.h - (PERC10W * 3.0)) / 3.0;

	SDL_Rect chilometriRect;
	chilometriRect.x = (listRect.x + PERC10W);
	chilometriRect.y = (listRect.y + PERC10H) + (nomeRect.h + PERC10H);
	chilometriRect.w = 220;
	chilometriRect.h = (listRect.h - (PERC10W*3.0)) / 3.0;

	SDL_Rect immaRect;
	immaRect.x = (listRect.x + PERC10W);
	immaRect.y = (listRect.y + PERC10H) + (nomeRect.h + PERC10H) + (chilometriRect.h + PERC10H);
	immaRect.w = 220;
	immaRect.h = (listRect.h - (PERC10W * 3.0)) / 3.0;

	SDL_Rect potenzaRect;
	potenzaRect.x = nomeRect.x+(nomeRect.w*3);
	potenzaRect.y = nomeRect.y;
	potenzaRect.w = 220;
	potenzaRect.h = (listRect.h - (PERC10W * 3.0)) / 3.0;

	SDL_Rect postiRect;
	postiRect.x = nomeRect.x + (nomeRect.w * 3);
	postiRect.y = chilometriRect.y;
	postiRect.w = 220;
	postiRect.h = (listRect.h - (PERC10W * 3.0)) / 3.0;

	SDL_Rect prezzoRect;
	prezzoRect.x = nomeRect.x + (nomeRect.w * 3);
	prezzoRect.y = immaRect.y;
	prezzoRect.w = 220;
	prezzoRect.h = (listRect.h - (PERC10W * 3.0)) / 3.0;

	SDL_Rect redRect;
	redRect.x = nomeRect.x + (nomeRect.w * 5);
	redRect.y = nomeRect.y;
	redRect.w = 400;
	redRect.h = (listRect.h - PERC10W);

	SDL_Rect redRectI;
	redRectI.x = (nomeRect.x + (nomeRect.w * 5))+PERC10W;
	redRectI.y = nomeRect.y+ PERC10H;
	redRectI.w = 200;
	redRectI.h = (listRect.h - (PERC10W * 3.0)) / 3.0;

	SDL_Rect calcRect;
	calcRect.x = (nomeRect.x + (nomeRect.w * 5)) + PERC10W + (PERC10W/2);
	calcRect.y = chilometriRect.y + PERC10H;
	calcRect.w = 200-PERC10W;
	calcRect.h = ((listRect.h - (PERC10W * 3.0)) / 3.0)-PERC10H;

	SDL_Rect prenotaRect;
	prenotaRect.x = calcRect.x + calcRect.w + PERC10W + PERC10W;
	prenotaRect.y = redRectI.y;
	prenotaRect.w = (redRect.w - calcRect.w - (PERC10W * 3))-PERC10W;
	prenotaRect.h = redRect.h - (PERC10H *2);

	SDL_Rect redRectO;
	redRectO.x = (nomeRect.x + (nomeRect.w * 5)) + PERC10W;
	redRectO.y = prezzoRect.y;
	redRectO.w = 200;
	redRectO.h = (listRect.h - (PERC10W * 3.0)) / 3.0;





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


	//BIG RECTANGLE
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &bigRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//LEFT ARROW
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(renderer, &leftArrRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

	//RIGHT ARROW
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &rightArrRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);


		TTF_Font* carFont;
		SDL_Color carColor;
		SDL_Surface* carSurface;
		SDL_Texture* carTexture;

		for (int f = 0; f < 5; f++) {

			
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &listRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &nomeRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &marcaRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &chilometriRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &immaRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &potenzaRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &postiRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &prezzoRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &redRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &redRectI);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &calcRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &redRectO);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawRect(renderer, &prenotaRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, stringToChar(garage[(page * 5) + f].marca), carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &nomeRect);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, stringToChar(garage[(page * 5) + f].modello), carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &marcaRect);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].km), carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &chilometriRect);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].annoMatricolazione), carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &immaRect);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].potenza), carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &potenzaRect);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].posti), carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &postiRect);
			
			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].prezzo), carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &prezzoRect);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, "numero mesi: ", carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &redRectI);

			carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
			carColor = { 255, 255, 255 };
			carSurface = TTF_RenderText_Solid(carFont, "calcola prezzo", carColor);
			carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
			SDL_FreeSurface(carSurface);
			SDL_RenderCopy(renderer, carTexture, NULL, &calcRect);


			listRect.y = listRect.y + (listRect.h + PERC10H);
			nomeRect.y += listRect.h+ PERC10H;
			marcaRect.y += listRect.h + PERC10H;
			chilometriRect.y += listRect.h + PERC10H;
			immaRect.y += listRect.h + PERC10H;
			potenzaRect.y += listRect.h + PERC10H;
			postiRect.y += listRect.h + PERC10H;
			prezzoRect.y += listRect.h + PERC10H;
			redRect.y += listRect.h + PERC10H;
			redRectI.y += listRect.h + PERC10H;
			redRectO.y += listRect.h + PERC10H;
			calcRect.y += listRect.h + PERC10H;
			prenotaRect.y += listRect.h + PERC10H;

		}
		SDL_RenderPresent(renderer);
		
	//end rectangle rendering

	// WINDOW ICON
	SDL_Surface* IconSurface = IMG_Load("code/images/icon.png");
	SDL_SetWindowIcon(screen, IconSurface);

	// EXIT BUTTON RENDERING
	SDL_Surface* ExitSurface = IMG_Load("code/images/exit.png");
	SDL_Texture* ExitTexture = SDL_CreateTextureFromSurface(renderer, ExitSurface);
	SDL_FreeSurface(ExitSurface);

	int proiva = 1;
	string stringa = to_string(proiva);

	//TITLE RENDERING
	TTF_Font* TitleFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
	SDL_Color TitleColor = { 255, 255, 255 };
	SDL_Surface* TitleSurface = TTF_RenderText_Solid(TitleFont, "AUTONOLEGGIO", TitleColor);
	SDL_Texture* TitleTexture = SDL_CreateTextureFromSurface(renderer, TitleSurface);
	SDL_FreeSurface(TitleSurface);


	//SEARCH ICON RENDERING
	SDL_Surface* SearchIconSurface = IMG_Load("code/images/searchIcon.png");
	SDL_Texture* SearchIconTexture = SDL_CreateTextureFromSurface(renderer, SearchIconSurface);
	SDL_FreeSurface(SearchIconSurface);
	


	//LIST RENDERING
	SDL_Surface* ListSurface = nullptr;
	SDL_Texture* ListTexture = SDL_CreateTextureFromSurface(renderer, ListSurface);
	SDL_FreeSurface(ListSurface);
	SDL_Surface* CoverSurface = IMG_Load("code/images/cover.png");
	SDL_Texture* CoverTexture = SDL_CreateTextureFromSurface(renderer, CoverSurface);

	





	while (!quit){

	//Get Mouse POS
		SDL_GetGlobalMouseState(&xMouse, &yMouse);
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


		if (check_click_in_rect(xMouse, yMouse, &rightArrRect) == 1) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					page += 1;
					

					if (page > (NMACCHINE / 5) - 1) {
						page = 0;
					}
					cout << page << endl;
					//COVER
					SDL_RenderCopy(renderer, CoverTexture, NULL, &bigRect);
					SDL_RenderPresent(renderer);
					SDL_UpdateWindowSurface(screen);
					//COVER
					listRect.y = bigRect.y + PERC10H;
					nomeRect.y = listRect.y + PERC10H;
					marcaRect.y = listRect.y + PERC10H;
					chilometriRect.y = (listRect.y + PERC10H) + (nomeRect.h + PERC10H);
					immaRect.y = (listRect.y + PERC10H) + (nomeRect.h + PERC10H) + (chilometriRect.h + PERC10H);
					potenzaRect.y = nomeRect.y;
					postiRect.y = chilometriRect.y;
					prezzoRect.y = immaRect.y;
					redRect.y = nomeRect.y;
					redRectI.y = nomeRect.y + PERC10H;
					calcRect.y = chilometriRect.y + PERC10H;
					prenotaRect.y = redRectI.y;
					redRectO.y = prezzoRect.y;

					for (int f = 0; f < 5; f++) {
						
						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &listRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &nomeRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &marcaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &chilometriRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &immaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &potenzaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &postiRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &prezzoRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
						SDL_RenderDrawRect(renderer, &redRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &redRectI);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &calcRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &redRectO);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &prenotaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, stringToChar(garage[(page * 5) + f].marca), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &nomeRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, stringToChar(garage[(page * 5) + f].modello), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &marcaRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].km), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &chilometriRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].annoMatricolazione), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &immaRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].potenza), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &potenzaRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].posti), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &postiRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].prezzo), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &prezzoRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, "numero mesi: ", carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &redRectI);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, "calcola prezzo", carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &calcRect);


						listRect.y = listRect.y + (listRect.h + PERC10H);
						nomeRect.y += listRect.h + PERC10H;
						marcaRect.y += listRect.h + PERC10H;
						chilometriRect.y += listRect.h + PERC10H;
						immaRect.y += listRect.h + PERC10H;
						potenzaRect.y += listRect.h + PERC10H;
						postiRect.y += listRect.h + PERC10H;
						prezzoRect.y += listRect.h + PERC10H;
						redRect.y += listRect.h + PERC10H;
						redRectI.y += listRect.h + PERC10H;
						redRectO.y += listRect.h + PERC10H;
						calcRect.y += listRect.h + PERC10H;
						prenotaRect.y += listRect.h + PERC10H;
					}
					
				}
			}
		}
		if (check_click_in_rect(xMouse, yMouse, &leftArrRect) == 1) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					page -= 1;
					

					if (page < 0) {
						page = (NMACCHINE / 5)-1;
					}
					cout << page << endl;
					//COVER
					SDL_RenderCopy(renderer, CoverTexture, NULL, &bigRect);
					SDL_RenderPresent(renderer);
					SDL_UpdateWindowSurface(screen);
					//COVER
					listRect.y = bigRect.y + PERC10H;
					listRect.y = bigRect.y + PERC10H;
					nomeRect.y = listRect.y + PERC10H;
					marcaRect.y = listRect.y + PERC10H;
					chilometriRect.y = (listRect.y + PERC10H) + (nomeRect.h + PERC10H);
					immaRect.y = (listRect.y + PERC10H) + (nomeRect.h + PERC10H) + (chilometriRect.h + PERC10H);
					potenzaRect.y = nomeRect.y;
					postiRect.y = chilometriRect.y;
					prezzoRect.y = immaRect.y;
					redRect.y = nomeRect.y;
					redRectI.y = nomeRect.y + PERC10H;
					calcRect.y = chilometriRect.y + PERC10H;
					prenotaRect.y = redRectI.y;
					redRectO.y = prezzoRect.y;

					for (int f = 0; f < 5; f++) {

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &listRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &nomeRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &marcaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &chilometriRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &immaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &potenzaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &postiRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &prezzoRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
						SDL_RenderDrawRect(renderer, &redRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &redRectI);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &calcRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &redRectO);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(renderer, &prenotaRect);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, stringToChar(garage[(page * 5) + f].marca), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &nomeRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, stringToChar(garage[(page * 5) + f].modello), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &marcaRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].km), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &chilometriRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].annoMatricolazione), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &immaRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].potenza), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &potenzaRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].posti), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &postiRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, intToChar(garage[(page * 5) + f].prezzo), carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &prezzoRect);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, "numero mesi: ", carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &redRectI);

						carFont = TTF_OpenFont("code/font/Roboto-Regular.ttf", 128);
						carColor = { 255, 255, 255 };
						carSurface = TTF_RenderText_Solid(carFont, "calcola prezzo", carColor);
						carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
						SDL_FreeSurface(carSurface);
						SDL_RenderCopy(renderer, carTexture, NULL, &calcRect);


						listRect.y = listRect.y + (listRect.h + PERC10H);
						nomeRect.y += listRect.h + PERC10H;
						marcaRect.y += listRect.h + PERC10H;
						chilometriRect.y += listRect.h + PERC10H;
						immaRect.y += listRect.h + PERC10H;
						potenzaRect.y += listRect.h + PERC10H;
						postiRect.y += listRect.h + PERC10H;
						prezzoRect.y += listRect.h + PERC10H;
						redRect.y += listRect.h + PERC10H;
						redRectI.y += listRect.h + PERC10H;
						redRectO.y += listRect.h + PERC10H;
						calcRect.y += listRect.h + PERC10H;
						prenotaRect.y += listRect.h + PERC10H;
					}

				}
			}
		}

		if (check_click_in_rect(xMouse, yMouse, &calcRect) == 1) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					
					if (yMouse < 500) {

					}

				}
			}
		}

		cout << yMouse  << endl;


		switch (event.type){ 
		case SDL_QUIT:
			quit = true;

			system("taskkill /IM gestionaleAutonoleggio.exe");
		}

		SDL_RenderCopy(renderer, TitleTexture, NULL, &titleRect);
		SDL_RenderCopy(renderer, ExitTexture, NULL, &exitRect);
		SDL_RenderCopy(renderer, SearchIconTexture, NULL, &searchIconRect);

		SDL_RenderPresent(renderer);

	}

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

		garage[i].dataInizioNoleggio[0] = 9;
		garage[i].dataInizioNoleggio[1] = 9;
		garage[i].dataInizioNoleggio[2] = 9;

		garage[i].durataNoleggio = 0;
		cout << "----------------------------" << endl;

		//scrivo tuttto sul file
		fileWrite << garage[i].marca << "\t";
		fileWrite << garage[i].modello << "\t";
		fileWrite << garage[i].carburante << "\t";
		fileWrite << garage[i].km << "\t";
		fileWrite << garage[i].annoMatricolazione << "\t";
		fileWrite << garage[i].potenza << "\t";
		fileWrite << garage[i].cambio << "\t";
		fileWrite << garage[i].fumatori << "\t";
		fileWrite << garage[i].usato << "\t";
		fileWrite << garage[i].posti << "\t";
		fileWrite << garage[i].porte << "\t";
		fileWrite << garage[i].neopatentato << "\t";
		fileWrite << garage[i].prezzo << "\t";
		fileWrite << garage[i].dataInizioNoleggio[0] << "\t";
		fileWrite << garage[i].dataInizioNoleggio[1] << "\t";
		fileWrite << garage[i].dataInizioNoleggio[2] << "\t";
		fileWrite << garage[i].durataNoleggio << "\t";
		fileWrite << "\n";
	}
	fileWrite.close();
}

void readCar() {
	ifstream infile("cars.txt");

	int i = 0, j = 0;

	string line;
	string delimiter = "\t";
	string token;

	size_t pos = 0;

	//per scorre le linee
	while (getline(infile, line)) {
		while ((pos = line.find(delimiter)) != string::npos) {
			token = line.substr(0, pos);
			line.erase(0, pos + delimiter.length());

			//per scegliere dove metterle
			if (j == 0) {
				garage[i].marca = token;
			}else if (j == 1) {
				garage[i].modello = token;
			}else if (j == 2) {
				garage[i].carburante = token;
			}else if (j == 3) {
				garage[i].km = stoi(token);
			}else if (j == 4) {
				garage[i].annoMatricolazione = stoi(token);
			}else if (j == 5) {
				garage[i].potenza = stoi(token);
			}else if (j == 6) {
				garage[i].cambio = stoi(token);
			}else if (j == 7) {
				garage[i].fumatori = stoi(token);
			}else if (j == 8) {
				garage[i].usato = stoi(token);
			}else if (j == 9) {
				garage[i].posti = stoi(token);
			}else if (j == 10) {
				garage[i].porte = stoi(token);
			}else if (j == 11) {
				garage[i].neopatentato = stoi(token);
			}else if (j == 12) {
				garage[i].prezzo = stoi(token);
			}else if (j == 13) {
				garage[i].dataInizioNoleggio[0] = stoi(token);
			}else if (j == 14) {
				garage[i].dataInizioNoleggio[1] = stoi(token);
			}else if (j == 15) {
				garage[i].dataInizioNoleggio[2] = stoi(token);
			}else if (j == 16) {
				garage[i].durataNoleggio = stoi(token);
			}
			j++;
		}
		i++;
		j = 0;
	}
}
void printStruct() {
	int i;
	for (i = 1; i < NMACCHINE; i++) {
		cout << "marca: "              << garage[i].marca << "  ";
		cout << "modello: "            << garage[i].modello << "  ";
		cout << "carburante: "         << garage[i].carburante << "  ";
		cout << "km: "                 << garage[i].km << "  ";
		cout << "annoMatricolazione: " << garage[i].annoMatricolazione << "  ";
		cout << "potenza: "            << garage[i].potenza << "  ";
		cout << "cambio: "             << garage[i].cambio << "  ";
		cout << "fumatori: "           << garage[i].fumatori << "  ";
		cout << "usato: "              << garage[i].usato << "  ";
		cout << "posti: "              << garage[i].posti << "  ";
		cout << "porte: "              << garage[i].porte << "  ";
		cout << "neopatentato: "       << garage[i].neopatentato << "  ";
		cout << "prezzo: "             << garage[i].prezzo << "\n";
		cout << "-------------------\n";
	}
}
//FINE FILE BINARI

//RICERCA
int searchByName(string input) {
	int i, j = 0;
	for (i = 0; i < NMACCHINE; i++) {
		if (garage[i].marca.compare(input) == 0) {
			garageRicerca[j].marca                 = garage[i].marca;
			garageRicerca[j].modello               = garage[i].modello;
			garageRicerca[j].carburante            = garage[i].carburante;
			garageRicerca[j].km                    = garage[i].km;
			garageRicerca[j].annoMatricolazione    = garage[i].annoMatricolazione;
			garageRicerca[j].potenza               = garage[i].potenza;
			garageRicerca[j].cambio                = garage[i].cambio;
			garageRicerca[j].fumatori              = garage[i].fumatori;
			garageRicerca[j].usato                 = garage[i].usato;
			garageRicerca[j].posti                 = garage[i].posti;
			garageRicerca[j].porte                 = garage[i].porte;
			garageRicerca[j].neopatentato          = garage[i].neopatentato;
			garageRicerca[j].prezzo                = garage[i].prezzo;
			garageRicerca[j].dataInizioNoleggio[0] = garage[i].dataInizioNoleggio[0];
			garageRicerca[j].dataInizioNoleggio[1] = garage[i].dataInizioNoleggio[1];
			garageRicerca[j].dataInizioNoleggio[2] = garage[i].dataInizioNoleggio[2];
			garageRicerca[j].durataNoleggio        = garage[i].durataNoleggio;
			j++;
		}
	}
	return j;	
}
//FINE RICERCA

//PREZZO
int prezzoPerMese(int km, int immatricolazione, int fumatori, int prezzo) {
	int prezzoXMese;
	int anno;
	int data[3];
	int dAnni;

	getCurrentDate(data);
	anno = data[2];

	prezzoXMese = prezzo;

	if (fumatori != 0) {
		prezzoXMese = prezzoXMese - ((prezzoXMese / 100) * 10);
	}

	dAnni = anno - immatricolazione;

	prezzoXMese -= (dAnni * 4);

	if (km > 200000) {
		prezzoXMese = prezzoXMese - ((prezzoXMese / 100) * 50);
	}
	else if (km > 150000 && km < 200000) {
		prezzoXMese = prezzoXMese - ((prezzoXMese / 100) * 35);
	}
	else if (km > 100000 && km < 150000) {
		prezzoXMese = prezzoXMese - ((prezzoXMese / 100) * 30);
	}
	else if (km > 50000 && km < 100000) {
		prezzoXMese = prezzoXMese - ((prezzoXMese / 100) * 20);
	}
	else if (km <= 50000) {
		prezzoXMese = prezzoXMese - ((prezzoXMese / 100) * 10);
	}
	return prezzoXMese;
}
//FINE PREZZO


//CASTING OPERATION
char* stringToChar(string input) {
	char* result = strcpy((char*)malloc(input.length() + 1), input.c_str());
	return result;
}

string intTostring(int input) {
	string result = to_string(input);
	return result;
}

char* intToChar(int input) {
	char* result;
	string stringa = intTostring(input);
	result = stringToChar(stringa);
	return result;

}
//FINE CASTING OPERATION

void stampDebug() {
	cout << garageRicerca[2].modello;
}