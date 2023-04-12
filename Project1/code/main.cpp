#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "libreriaAutonoleggio.h"

using namespace std;

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

int main(int argc, char* argv[]) {
	generazioneFinestra();
	return 0;
}