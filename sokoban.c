#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MNOZNIK 3
#define DZIELNIK 2

typedef enum KierunekPchniecia {
    DOL = 2,
    LEWO = 4,
    PRAWO = 6,
    GORA = 8
}kierunek;

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

void iniPlansza(plansza *p) {
    p -> rozmiar = 0;
    p -> maxRozmiar = 0;
    p -> wiersz = NULL;
}

void iniWiersz(wiersz *w) {
    w -> rozmiar = 0;
    w -> maxRozmiar = 0;
    w -> kolumna = NULL;
}

void dodajWiersze(plansza *p) {
    p -> maxRozmiar = 1 + (p -> maxRozmiar) * MNOZNIK / DZIELNIK;
    p -> wiersz = (wiersz*)realloc(p -> wiersz, (p -> maxRozmiar) * sizeof(wiersz));
}

void dodajKolumny(wiersz *w) {
    w -> maxRozmiar = 1 + (w -> maxRozmiar) * MNOZNIK / DZIELNIK;
    w -> kolumna = (char*)realloc(w -> kolumna, (w -> maxRozmiar) * sizeof(char));
}

void wczytajPlansze(plansza *p, int gdzieSkrzynie[][2], int gdziePostac[]) {
    int znak = 0;
    int poprzedni = 0;
    dodajWiersze(p);
    wiersz *obecnyWiersz = p -> wiersz;
    iniWiersz(obecnyWiersz);

    while((znak = getchar()) != '\n' || poprzedni != '\n') {
        if(znak == '\n') {
            (p -> rozmiar)++;
            if(p -> rozmiar == p -> maxRozmiar) {
                dodajWiersze(p);
            }
            obecnyWiersz = &(p -> wiersz[p -> rozmiar]);
            iniWiersz(obecnyWiersz);
        }
        else {
            if((znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z')) {
                int malaLitera = tolower(znak);
                gdzieSkrzynie[malaLitera - 'a'][0] = p -> rozmiar;
                gdzieSkrzynie[malaLitera - 'a'][1] = obecnyWiersz -> rozmiar;
            }
            else if(znak == '@' || znak == '*') {
                gdziePostac[0] = p -> rozmiar;
                gdziePostac[1] = obecnyWiersz -> rozmiar;
            }

            if(obecnyWiersz -> rozmiar == obecnyWiersz -> maxRozmiar) {
                dodajKolumny(obecnyWiersz);
            }
            obecnyWiersz -> kolumna[obecnyWiersz -> rozmiar] = znak;
            (obecnyWiersz -> rozmiar)++;
        }
        poprzedni = znak;
    }
}

void wypiszPlansze(plansza *p) {
    int ileWierszy = p -> rozmiar;
    int ileKolumn = 0;
    wiersz *obecnyWiersz = NULL;

    for(int i = 0; i < ileWierszy; ++i) {
        obecnyWiersz = &(p -> wiersz[i]);
        ileKolumn = obecnyWiersz -> rozmiar;
        for(int j = 0; j < ileKolumn; ++j) {
            printf("%c", obecnyWiersz -> kolumna[j]);
        }
        printf("\n");
    }
}

void zwolnijPamiec(plansza *p) {
    int ileWierszy = p -> rozmiar;
    wiersz *obecnyWiersz = NULL;

    for(int i = 0; i < ileWierszy; ++i) {
        obecnyWiersz = &(p -> wiersz[i]);
        free(obecnyWiersz -> kolumna);
    }
    free(p -> wiersz);
}

int main(void) {
    plansza p;
    int gdzieSkrzynie[26][2] = {0};
    int gdziePostac[2] = {0};

    iniPlansza(&p);
    wczytajPlansze(&p, gdzieSkrzynie, gdziePostac);
    wypiszPlansze(&p);
    zwolnijPamiec(&p);
    
    return 0;
}