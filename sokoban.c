#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

int pustyWiersz(wiersz *W) {
	return (W -> rozmiar) == 0;
}

int ostatniZnak(wiersz *W) {
	if(pustyWiersz(W)) return __INT_MAX__;
	return W -> znaki[(W -> rozmiar) - 1];
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

typedef struct Pozycja {
	int nrWiersza;
	int nrKolumny;
}pozycja;

int czySkrzynia(char c) {
	return isalpha(c);
}

int czyPostac(char c) {
	return c == '@' || c == '*';
}

void ustawPozycje(pozycja *poz, int numerWiersza, int numerKolumny) {
	poz -> nrWiersza = numerWiersza;
	poz -> nrKolumny = numerKolumny;
}

void wczytajPlansze(plansza *P, pozycja skrzynie[], pozycja *postac) {
    char c = 0;
    while((c = getchar()) != '\n') {
        wiersz *obecnyWiersz = wstawWiersz(P);
        do {
			wstawZnak(obecnyWiersz, c);
			if(czySkrzynia(c)) {
				pozycja *pozSkrzyni = &skrzynie[tolower(c) - 'a'];
				ustawPozycje(pozSkrzyni, P -> rozmiar - 1, obecnyWiersz -> rozmiar - 1);
			}
			else if(czyPostac(c)) {
				ustawPozycje(postac, P -> rozmiar - 1, obecnyWiersz -> rozmiar - 1);
			}
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
	pozycja skrzynie[26];
	pozycja postac;
    initPlansza(&P);
    wczytajPlansze(&P, skrzynie, &postac);
    rysujPlansze(&P);
    wyczyscPlansze(&P);
    
    return 0;
}