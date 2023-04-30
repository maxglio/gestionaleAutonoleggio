#ifndef libreriaAutonoleggio
#define libreriaAutonoleggio

//INTERFACCIA GRAFICA
int generazioneFinestra();

//FINE INTERFACCIA GRAFICA

//DATE
void endDateCalculator(int inizio[], int fine[]);

void getCurrentDate(int data[]);

void getCurrentTime(int ora[]);

void cover(SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* texture, SDL_Rect* rect);

//FINE DATE

//FILE BINARI
void addCar();

void readCar();

void printStruct();
//FINE FILE BINARI
#endif
