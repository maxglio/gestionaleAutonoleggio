#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <fstream>
#include <ctime>
#include <cstdlib>

#include "libreriaAutonoleggio.h"

using namespace std;

const Uint8 Button = SDL_BUTTON_LEFT; /*We know that we are going to use this button only*/

/*A boolean to check if the object intersects with the mouse*/
bool intersects(int objectX, int objectY, int objectW, int objectH, int mouseX, int mouseY)
{
	if (mouseX < objectX || mouseY < objectY)
	{
		return false;
	}
	else if (mouseX > objectX + objectW || mouseY > objectY + objectH)
	{
		return false;
	}
	/*Returns true only if the mouse is hovering the object*/
	return true;
}

/*enum of gamestates*/
enum gamestate
{
	Intro,
	Main,
	Exit,
};

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

	printf("w=%d h=%d", w, h);

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
	ofstream wf("/binaryFiles/id.dat", ios::out | ios::binary);
	id wstu[1];
	wstu[0].id = 0;
	wf.write((char*)&wstu[0], sizeof(id));
	wf.close();
	ifstream rf("/binaryFiles/id.dat", ios::out | ios::binary);
	id rstu[1];
	rf.read((char*)&rstu[0], sizeof(id));
	rf.close();
	cout << "id: " << wstu[0].id;
}
//FINE FILE BINARI