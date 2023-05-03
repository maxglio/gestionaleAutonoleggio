#ifndef libreriaAutonoleggio
#define libreriaAutonoleggio

//INTERFACCIA GRAFICA
int generazioneFinestra();

//FINE INTERFACCIA GRAFICA

//DATE
void endDateCalculator(int inizio[], int fine[]);

void getCurrentDate(int data[]);

void getCurrentTime(int ora[]);

//FINE DATE

//FILE BINARI
void addCar();

void readCar();

void printStruct();
//FINE FILE BINARI

//RICERCA
int searchByName(string input);
//FINE RICERCA

//CASTING OPERATION

//FINE CASTING OPERATION
#endif
