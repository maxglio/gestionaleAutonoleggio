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

string intTostring(int input);

char* intToChar(int input);
//FINE CASTING OPERATION

//AGGIUNTA PEZZI STRINGA
char* addKm(int input);

char* addImmatricolazione(int input);

char* addPotenza(int input);

char* addPosti(int input);

char* addPrezzo(int input);
//FINE AGGIUNTA PEZZI STRINGA

void stampDebug();
#endif
