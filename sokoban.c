#include <stdio.h>
#include <stdlib.h>

#define MNOZNIK 3
#define DZIELNIK 2

//====================================================================================================
//Implementacja pojedynczego wiersza w planszy

typedef struct Wiersz {
	char *znaki;
	int rozmiar;
	int pojemnosc;
}wiersz;

void initWiersz(wiersz *W) {
	W -> znaki = NULL;
	W -> rozmiar = 0;
	W -> pojemnosc = 0;
}

void powiekszWiersz(wiersz *W) {
	W -> pojemnosc = 1 + (W -> pojemnosc) * MNOZNIK / DZIELNIK;
	W -> znaki = (char*)realloc(W -> znaki, (W -> pojemnosc) * sizeof(char));
}

void wstawZnak(wiersz *W, char c) {
	if(W -> rozmiar == W -> pojemnosc) {
		powiekszWiersz(W);
	}
	W -> znaki[W -> rozmiar] = c;
	(W -> rozmiar)++;
}

void wyczyscWiersz(wiersz *W) {
	free(W -> znaki);
}

//====================================================================================================
//Implementacja planszy do gry w Sokobana

typedef struct Plansza {
	wiersz *wiersze;
	int rozmiar;
	int pojemnosc;
}plansza;

void initPlansza(plansza *P) {
	P -> wiersze = NULL;
	P -> rozmiar = 0;
	P -> pojemnosc = 0;
}

void powiekszPlansze(plansza *P) {
	P -> pojemnosc = 1 + (P -> pojemnosc) * MNOZNIK / DZIELNIK;
	P -> wiersze = (wiersz*)realloc(P -> wiersze, (P -> pojemnosc) * sizeof(wiersz));
}

wiersz* wstawWiersz(plansza *P) {
	if(P -> rozmiar == P -> pojemnosc) {
		powiekszPlansze(P);
	}
	wiersz *obecnyWiersz = &(P -> wiersze[P -> rozmiar]);
	initWiersz(obecnyWiersz);
	(P -> rozmiar)++;
	return obecnyWiersz;
}

void wyczyscPlansze(plansza *P) {
	int liczbaWierszy = P -> rozmiar;
	wiersz *obecnyWiersz = P -> wiersze;
	for(int i = 0; i < liczbaWierszy; ++i) {
		wyczyscWiersz(obecnyWiersz);
		obecnyWiersz++;
	}
    free(P -> wiersze);
}

//====================================================================================================
//Wczytywanie planszy

void wczytajPlansze(plansza *P) {
    char c = 0;
    while((c = getchar()) != '\n') {
        wiersz *obecnyWiersz = wstawWiersz(P);
        do {
            wstawZnak(obecnyWiersz, c);
        }while((c = getchar()) != '\n');
    }
}

//====================================================================================================
//Rysowanie planszy

void rysujPlansze(plansza *P) {
    int liczbaWierszy = P -> rozmiar;
    wiersz *obecnyWiersz = P -> wiersze;

    for(int i = 0; i < liczbaWierszy; ++i) {
        int liczbaZnakow = obecnyWiersz -> rozmiar;
        for(int j = 0; j < liczbaZnakow; ++j) {
            putchar(obecnyWiersz -> znaki[j]);
        }
        putchar('\n');
        obecnyWiersz++;
    }
}

int main(void) {
    plansza P;
    initPlansza(&P);
    wczytajPlansze(&P);
    rysujPlansze(&P);
    wyczyscPlansze(&P);
    
    return 0;
}