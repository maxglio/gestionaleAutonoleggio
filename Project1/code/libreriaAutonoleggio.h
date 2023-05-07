#ifndef libreriaAutonoleggio
#define libreriaAutonoleggio

using namespace std;

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

//PREZZO
int prezzoPerMese(int km, int immatricolazione, int fumatori, int prezzo);
//FINE PREZZO

//CASTING OPERATION
char* stringToChar(string input);
//FINE CASTING OPERATION

void stampDebug();
#endif
