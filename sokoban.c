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

int literaNaIndeks(char c) {
	return c - 'a'; 
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
				pozycja *pozSkrzyni = &skrzynie[literaNaIndeks(tolower(c))];
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

//====================================================================================================
//Implementacja planszy odwiedzin

void kopiujPlansze(plansza *P, plansza *odwiedzone) {
	int liczbaWierszy = P -> rozmiar;
	odwiedzone -> wiersze = (wiersz*)malloc(liczbaWierszy * sizeof(wiersz));
	odwiedzone -> rozmiar = liczbaWierszy;
	odwiedzone -> pojemnosc = liczbaWierszy;
	wiersz *wierszPlanszy = P -> wiersze;
	wiersz *wierszOdw = odwiedzone -> wiersze;

	for(int i = 0; i < liczbaWierszy; ++i) {
		int liczbaZnakow = wierszPlanszy -> rozmiar;
		wierszOdw -> znaki = (char*)malloc(liczbaZnakow * sizeof(char));
		wierszOdw -> rozmiar = liczbaZnakow;
		wierszOdw -> pojemnosc = liczbaZnakow;
		wierszPlanszy++;
		wierszOdw++;
	}
}

void zerujPlansze(plansza *odwiedzone) {
	int liczbaWierszy = odwiedzone -> rozmiar;
	wiersz *obecnyWiersz = odwiedzone -> wiersze;
	
	for(int i = 0; i < liczbaWierszy; ++i) {
		int liczbaZnakow = obecnyWiersz -> rozmiar;
		for(int j = 0; j < liczbaZnakow; ++j) {
			obecnyWiersz -> znaki[j] = 'O';
		}
		obecnyWiersz++;
	}
}

//====================================================================================================
//Sprawdzenie czy mozna dojsc z pola gracza do pola pchniecia

int wolnePole(char c) {
	return c == '-' || c == '+' || c == '@' || c == '*';
}

int bezpiecznePole(plansza *P, int i, int j) {
	int liczbaWierszy = P -> rozmiar;
	if (i >= 0 && i < liczbaWierszy && 
		j >= 0 && j < (P -> wiersze[i].rozmiar) && 
		wolnePole(P -> wiersze[i].znaki[j])) {
		return 1;
	}
	return 0;
}

int odwiedzone(plansza *odw, int i, int j) {
	return odw -> wiersze[i].znaki[j] == 'X';
}

int moznaDojsc(plansza *P, plansza *odw, int pWiersz, int pKol, pozycja pchniecie) {
	if(bezpiecznePole(P, pWiersz, pKol) && !odwiedzone(odw, pWiersz, pKol)) {

		(odw -> wiersze[pWiersz].znaki[pKol]) = 'X'; //zaznaczam odwiedzone pole

		if(pWiersz == pchniecie.nrWiersza && pKol == pchniecie.nrKolumny) return 1;

		int wPrawo = moznaDojsc(P, odw, pWiersz, pKol + 1, pchniecie);
		if(wPrawo) return 1;

		int wDol = moznaDojsc(P, odw, pWiersz + 1, pKol, pchniecie);
		if(wDol) return 1;

		int wLewo = moznaDojsc(P, odw, pWiersz, pKol - 1, pchniecie);
		if(wLewo) return 1;

		int wGore = moznaDojsc(P, odw, pWiersz - 1, pKol, pchniecie);
		if(wGore) return 1;
	}
	return 0;
}

//====================================================================================================
//Sprawdzenie czy mozna pchnac skrzynie

typedef enum Kierunek {
	DOL = 2,
	GORA = 8,
	LEWO = 4,
	PRAWO = 6
}kierunek;

int moznaPchnac(plansza *P, kierunek kier, pozycja skrzynia) {
	if(kier == LEWO || kier == PRAWO) {
		if(bezpiecznePole(P, skrzynia.nrWiersza, skrzynia.nrKolumny - 1) &&
		   bezpiecznePole(P, skrzynia.nrWiersza, skrzynia.nrKolumny + 1)) {
		   return 1;
		}
	}
	else {
		if(bezpiecznePole(P, skrzynia.nrWiersza - 1, skrzynia.nrKolumny) &&
		   bezpiecznePole(P, skrzynia.nrWiersza + 1, skrzynia.nrKolumny)) {
		   return 1;
		}
	}
	return 0;
}

//====================================================================================================
//Wykonaj ruch

typedef struct Ruch {
	char skrzynia;
	kierunek kier;
}ruch;

int poleDocelowe(char c) {
	return isupper(c) || c == '+' || c == '*';
}

void wykonajRuch(plansza *P, pozycja *postac, pozycja *skrzynia, ruch r) {
	int sWiersz = skrzynia -> nrWiersza;
	int sKol = skrzynia -> nrKolumny;
	int pWiersz = postac -> nrWiersza;
	int pKol = postac -> nrKolumny;
	int kierPchniecia = r.kier;
	char nazwaSkrzyni = r.skrzynia;

	int deltaPoz[4][2] = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}}; // dol lewo prawo gora
	int sNowyWiersz = sWiersz + deltaPoz[kierPchniecia / 2 - 1][0];
	int sNowaKol = sKol + deltaPoz[kierPchniecia / 2 - 1][1];

	if(poleDocelowe(P -> wiersze[sWiersz].znaki[sKol])) {
		P -> wiersze[sWiersz].znaki[sKol] = '*';
	}
	else {
		P -> wiersze[sWiersz].znaki[sKol] = '@';
	}
	if(poleDocelowe(P -> wiersze[pWiersz].znaki[pKol])) {
		P -> wiersze[pWiersz].znaki[pKol] = '+';
	}
	else {
		P -> wiersze[pWiersz].znaki[pKol] = '-';
	}
	if(poleDocelowe(P -> wiersze[sNowyWiersz].znaki[sNowaKol])) {
		P -> wiersze[sNowyWiersz].znaki[sNowaKol] = toupper(nazwaSkrzyni);
	}
	else {
		P -> wiersze[sNowyWiersz].znaki[sNowaKol] = nazwaSkrzyni;
	}
	skrzynia -> nrWiersza = sNowyWiersz;
	skrzynia -> nrKolumny = sNowaKol;
	postac -> nrWiersza = sWiersz;
	postac -> nrKolumny = sKol;
}

//====================================================================================================
//Implementacja stosu ruchow

typedef struct stosRuchow {
	char nazwaSkrzyni;
	pozycja postac;
	struct stosRuchow *nast;
}stosR;

void initStos(stosR **glowa) {
	*glowa = NULL;
}

void wstawNaStos(stosR **glowa, char nazwaSkrzyni, pozycja postac) {
	stosR *nowyElement = (stosR*)malloc(sizeof(stosR));
	nowyElement -> nazwaSkrzyni = nazwaSkrzyni;
	nowyElement -> postac = postac;
	nowyElement -> nast = *glowa;
	*glowa = nowyElement;
}

int pustyStos(stosR *glowa) {
	return glowa == NULL;
}

void zdejmijZeStosu(stosR **glowa) {
	stosR *nowaGlowa = (*glowa) -> nast;
	free(*glowa);
	*glowa = nowaGlowa;
}

void wyczyscStos(stosR **glowa) {
	if(!pustyStos(*glowa)) {
		wyczyscStos(&((*glowa) -> nast));
		free(*glowa);
		*glowa = NULL;
	}
}

//====================================================================================================
//Cofanie ruchow

void cofnijRuch(plansza *P, pozycja *postac, pozycja skrzynie[], stosR *glowa) {
	int pWiersz = postac -> nrWiersza;
	int pKol = postac -> nrKolumny;
	char nazwaSkrzyni = glowa -> nazwaSkrzyni;
	int sWiersz = skrzynie[literaNaIndeks(nazwaSkrzyni)].nrWiersza;
	int sKol = skrzynie[literaNaIndeks(nazwaSkrzyni)].nrKolumny;
	int pNowyWiersz = glowa -> postac.nrWiersza;
	int pNowaKol = glowa -> postac.nrKolumny;

	if(poleDocelowe(P -> wiersze[pWiersz].znaki[pKol])) {
		P -> wiersze[pWiersz].znaki[pKol] = toupper(nazwaSkrzyni);
	}
	else {
		P -> wiersze[pWiersz].znaki[pKol] = nazwaSkrzyni;
	}
	if(poleDocelowe(P -> wiersze[sWiersz].znaki[sKol])) {
		P -> wiersze[sWiersz].znaki[sKol] = '+';
	}
	else {
		P -> wiersze[sWiersz].znaki[sKol] = '-';
	}
	if(poleDocelowe(P -> wiersze[pNowyWiersz].znaki[pNowaKol])) {
		P -> wiersze[pNowyWiersz].znaki[pNowaKol] = '*';
	}
	else {
		P -> wiersze[pNowyWiersz].znaki[pNowaKol] = '@';
	}
	
	skrzynie[literaNaIndeks(nazwaSkrzyni)].nrWiersza = pWiersz;
	skrzynie[literaNaIndeks(nazwaSkrzyni)].nrKolumny = pKol;
	postac -> nrWiersza = pNowyWiersz;
	postac -> nrKolumny = pNowaKol;
}

//====================================================================================================
//Czytanie polecen i wykonywanie ruchow

void gdziePchniecie(kierunek kierPchniecia, pozycja pozSkrzyni, pozycja *pozPchniecia) {
	int deltaPoz[4][2] = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}}; // dol lewo prawo gora
	pozPchniecia -> nrWiersza = pozSkrzyni.nrWiersza - deltaPoz[kierPchniecia / 2 - 1][0];
	pozPchniecia -> nrKolumny = pozSkrzyni.nrKolumny - deltaPoz[kierPchniecia / 2 - 1][1];
}

void czytajPolecenia(plansza *P, pozycja skrzynie[], pozycja *postac) {
	char c = 0;
	ruch ruchPostaci;
	plansza odw;
	kopiujPlansze(P, &odw);
	stosR *ruchyGracza;
	initStos(&ruchyGracza);

	while((c = getchar()) != '.') {
		if(czySkrzynia(c)) {
			ruchPostaci.skrzynia = c;
			c = getchar(); //kierunek pchniecia
			ruchPostaci.kier = c - '0';
			pozycja pozPchniecia;
			pozycja *pozSkrzyni = &skrzynie[ruchPostaci.skrzynia - 'a'];
			gdziePchniecie(ruchPostaci.kier, *pozSkrzyni, &pozPchniecia);

			if(moznaPchnac(P, ruchPostaci.kier, *pozSkrzyni)) {
				zerujPlansze(&odw);
				if(moznaDojsc(P, &odw, postac -> nrWiersza, postac -> nrKolumny, pozPchniecia)) {
					wstawNaStos(&ruchyGracza, ruchPostaci.skrzynia, *postac);
					wykonajRuch(P, postac, pozSkrzyni, ruchPostaci);
				}
			}
		}
		else {
			if(!pustyStos(ruchyGracza)) {
				cofnijRuch(P, postac, skrzynie, ruchyGracza);
				zdejmijZeStosu(&ruchyGracza);
			}
		}
		getchar(); //pozbycie się znaku nowej linii
		rysujPlansze(P);
	}
	wyczyscPlansze(&odw);
	wyczyscStos(&ruchyGracza);
}

int main(void) {
	
    plansza P;
	pozycja skrzynie[26];
	pozycja postac;
    initPlansza(&P);
    wczytajPlansze(&P, skrzynie, &postac);
    rysujPlansze(&P);
	czytajPolecenia(&P, skrzynie, &postac);
    wyczyscPlansze(&P);
	
    
    return 0;
}