#include <stdio.h>
#include <stdlib.h>

#define MNOZNIK 3
#define DZIELNIK 2

typedef struct Wiersz {
    int rozmiar;
    int maxRozmiar;
    char* kolumna;
}wiersz;

typedef struct Plansza {
    int rozmiar;
    int maxRozmiar;
    wiersz* wiersz;
}plansza;

void iniPlansza(plansza* p) {
    p -> rozmiar = 0;
    p -> maxRozmiar = 0;
    p -> wiersz = NULL;
}

void dodajWiersze(plansza* p) {
    (p -> maxRozmiar) = 1 + (p -> maxRozmiar) * MNOZNIK / DZIELNIK;
    (p -> wiersz) = (wiersz*)realloc((p -> wiersz), (p -> maxRozmiar) * sizeof(wiersz));
}

void dodajKolumny(wiersz* w) {
    (w -> maxRozmiar) = 1 + (w -> maxRozmiar) * MNOZNIK / DZIELNIK;
    (w -> kolumna) = (char*)realloc((w -> kolumna), (w -> maxRozmiar) * sizeof(char));
}

int main(void) {
    plansza p;
    iniPlansza(&p);
    
    return 0;
}