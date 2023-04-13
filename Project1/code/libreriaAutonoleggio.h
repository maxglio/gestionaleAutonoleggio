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
void refreshID();

void addCar();
//FINE FILE BINARI
#endif
