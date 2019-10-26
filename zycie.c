#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

struct lista_wierszy {
    int wiersz;
    struct lista_zywych_komorek *kom;
    struct lista_wierszy *nast;
};
typedef struct lista_wierszy wiersze;

struct lista_zywych_komorek {
    int kolumna;
    struct lista_zywych_komorek *nast;
    struct lista_zywych_komorek *poprz;
};
typedef struct lista_zywych_komorek zywe;

/* Funkcja dodaje wiersz za wskazanym wierszem */
void dodaj_wiersz_za(wiersze *wsk, int nr_wiersza) {
    wiersze *pom = (wiersze *) malloc(sizeof(wiersze));
    pom->wiersz = nr_wiersza;
    pom->kom = NULL;
    pom->nast = NULL;
    wsk->nast = pom;
}

/* Funkcja dodaje kolumne za wskazana kolumna */
void dodaj_kolumne_za(zywe *wsk, int nr_kolumny) {
    zywe *pom = (zywe *) malloc(sizeof(zywe));
    pom->kolumna = nr_kolumny;
    pom->nast = NULL;
    pom->poprz = wsk;
    wsk->nast = pom;
}

/* Funkcja usuwa atrape wiersza */
void usun_atrape_wiersza(wiersze **wsk, bool jest_wiersz) {
    wiersze *pom = *wsk;
    *wsk = (*wsk)->nast;
    free(pom);
    if (!jest_wiersz) *wsk = NULL;
}

/*Funkcja usuwa atrape kolumny */
void usun_atrape_kolumny(zywe **wsk, bool jest_zywa) {
    zywe *pom = *wsk;
    *wsk = (*wsk)->nast;
    free(pom);
    if (jest_zywa) (*wsk)->poprz = NULL;
    else *wsk = NULL;
}

/* Funkcja wczytuje zerowe pokolenie z wejscia */
wiersze *wczytaj(int *liczba_w, int *liczba_k, int *liczba_zywych, int *pocz_w, int *pocz_k, int *kon_w, int *kon_k) {
    wiersze *plansza = malloc(sizeof(wiersze));
    wiersze *pom_w = plansza;
    char c;
    int i = 1, j;
    bool jest_zywa, jest_wiersz = false;
    while ((c = getchar()) != '\n') {
        j = 1;
        jest_zywa = false;
        if (!jest_wiersz) jest_wiersz = true;
        dodaj_wiersz_za(pom_w, i);
        pom_w->nast->kom = (zywe *) malloc(sizeof(zywe));
        zywe *pom_z = pom_w->nast->kom;
        while (c != '\n') {
            if (c == '0') {
                dodaj_kolumne_za(pom_z, j);
                pom_z = pom_z->nast;
                if (*pocz_w == INT_MAX) *pocz_w = i;
                *pocz_k = (j < *pocz_k) ? j : *pocz_k;
                *kon_w = (i > *kon_w) ? i : *kon_w;
                *kon_k = (j > *kon_k) ? j : *kon_k;
                *liczba_zywych += 1;
                if (!jest_zywa) jest_zywa = true;
            }
            c = getchar();
            *liczba_k = (j > *liczba_k) ? j : *liczba_k;
            j++;
        }
        usun_atrape_kolumny(&pom_w->nast->kom, jest_zywa);
        if (pom_w->nast->kom == NULL) {
            free(pom_w->nast);
            pom_w->nast = NULL;
        } else {
            pom_w = pom_w->nast;
        }
        *liczba_w = (i > *liczba_w) ? i : *liczba_w;
        i++;
    }
    usun_atrape_wiersza(&plansza, jest_wiersz);
    return plansza;
}

/* Funkcja wypisuje pokolenie */
void wypisz_plansze(wiersze *plansza, int a, int b, int c, int d) {
    while (plansza != NULL && plansza->wiersz < a) plansza = plansza->nast;
    for (int i = a; i <= c; i++) {
        if (plansza != NULL && plansza->wiersz == i) {
            zywe *pom = plansza->kom;
            while (pom != NULL && pom->kolumna < b) pom = pom->nast;
            for (int j = b; j <= d; j++) {
                if (pom != NULL && pom->kolumna == j) {
                    printf("0");
                    pom = pom->nast;
                }
                else printf(".");
            }
            plansza = plansza->nast;
        } else {
            for (int j = b; j <= d; j++) {
                printf(".");
            }
        }
        printf("\n");
    }
}

/* Funkcja dodaje wiersz na poczatku listy */
void dodaj_wiersz_na_poczatku(wiersze **wsk) {
    wiersze *pom = (wiersze *) malloc(sizeof(wiersze));
    pom->wiersz = (*wsk)->wiersz - 1;
    pom->kom = NULL;
    pom->nast = *wsk;
    *wsk = pom;
}

/* Funkcja dodaje wiersz na koncu listy */
void dodaj_wiersz_na_koncu(wiersze *wsk) {
    while (wsk->nast != NULL) wsk = wsk->nast;
    wiersze *pom = (wiersze *) malloc(sizeof(wiersze));
    pom->wiersz = wsk->wiersz + 1;
    pom->kom = NULL;
    pom->nast = NULL;
    wsk->nast = pom;
}

/* Funkcja oblicza ile zywych komorek znajduje sie dookola */
int ile_zywych(zywe *wsk, int j) {
    int zywi = 0;
    if (wsk != NULL) {
        if (wsk->poprz != NULL) {
            if (wsk->poprz->kolumna >= j - 1 && wsk->poprz->kolumna <= j + 1) zywi++;
            if (wsk->poprz->poprz != NULL && wsk->poprz->poprz->kolumna >= j - 1 && wsk->poprz->poprz->kolumna <= j + 1) zywi++;
        }
        if (wsk->kolumna >= j - 1 && wsk->kolumna <= j + 1) zywi++;
        if (wsk->nast != NULL) {
            if (wsk->nast->kolumna >= j - 1 && wsk->nast->kolumna <= j + 1) zywi++;
            if (wsk->nast->nast != NULL && wsk->nast->nast->kolumna >= j - 1 && wsk->nast->nast->kolumna <= j + 1) zywi++;
        }
    }
    return zywi;
}

/* Funkcja sprawdza czy komorka o danej kolumnie bedzie zywa w nastepnym
pokoleniu */
bool czy_zywa(zywe *gorna, zywe *srodkowa, zywe *dolna, int j) {
    bool zywa_komorka = false;
    int zywi = 0;
    zywi += ile_zywych(gorna, j);
    zywi += ile_zywych(srodkowa, j);
    zywi += ile_zywych(dolna, j);
    if (srodkowa != NULL && srodkowa->nast != NULL && srodkowa->nast->kolumna == j) srodkowa = srodkowa->nast;
    if (srodkowa != NULL && srodkowa->kolumna == j) {
        zywi--;
        if (zywi == 2 || zywi == 3) zywa_komorka = true;
    } else if (zywi == 3) zywa_komorka = true;
    return zywa_komorka;
}

/* Funkcja zwalnia liste zywych komorek */
void zwolnij_liste_zywych(zywe *wsk) {
    zywe *pom;
    while (wsk != NULL) {
        pom = wsk->nast;
        free(wsk);
        wsk = pom;
    }
}

/* Funkcja zwalnia liste wierszy */
void zwolnij_liste_wierszy(wiersze *wsk) {
    wiersze *pom;
    while (wsk != NULL) {
        pom = wsk->nast;
        zwolnij_liste_zywych(wsk->kom);
        free(wsk);
        wsk = pom;
    }
}

/* Funkcja tworzy nowe pokolenie */
wiersze *nowe_pokolenie(wiersze *plansza, int *pocz_w, int *pocz_k, int *kon_w, int *kon_k, int *liczba_zywych) {
    *liczba_zywych = 0;
    wiersze *nowe = (wiersze *) malloc(sizeof(wiersze));
    wiersze *pom_w = nowe;
    bool jest_zywa;
    int i = *pocz_w - 1, j;
    int ostatni_wiersz = *kon_w + 1, pierwsza_kolumna = *pocz_k - 1, ostatnia_kolumna = *kon_k + 1;
    *pocz_w = INT_MAX; *kon_w = INT_MIN; *pocz_k = INT_MAX; *kon_k = INT_MIN;
    dodaj_wiersz_na_poczatku(&plansza);
    dodaj_wiersz_na_poczatku(&plansza);
    dodaj_wiersz_na_koncu(plansza);
    dodaj_wiersz_na_koncu(plansza);
    wiersze *pom = plansza;
    while (i <= ostatni_wiersz) {
        wiersze *gorny = NULL, *srodkowy = NULL, *dolny = NULL;
        if (pom->nast->wiersz < i) pom = pom->nast;
        if (pom->wiersz == i - 1) gorny = pom;
        if (pom->nast->wiersz == i) srodkowy = pom->nast;
        if (pom->nast->nast->wiersz == i + 1) dolny = pom->nast->nast;
        else if (dolny == NULL && pom->nast->wiersz == i + 1) dolny = pom->nast;
        if (gorny != NULL || srodkowy != NULL || dolny != NULL) {
            j = pierwsza_kolumna;
            jest_zywa = false;
            dodaj_wiersz_za(pom_w, i);
            pom_w->nast->kom = (zywe *) malloc(sizeof(zywe));
            zywe *pom_z = pom_w->nast->kom;
            while (j <= ostatnia_kolumna) {
                zywe *gorna = NULL, *srodkowa = NULL, *dolna = NULL;
                if (gorny != NULL) gorna = gorny->kom;
                if (srodkowy != NULL) srodkowa = srodkowy->kom;
                if (dolny != NULL) dolna = dolny->kom;
                while (gorna != NULL && gorna->kolumna < j - 1) gorna = gorna->nast;
                while(srodkowa != NULL && srodkowa->kolumna < j - 1) srodkowa = srodkowa->nast;
                while (dolna != NULL && dolna->kolumna < j - 1) dolna = dolna->nast;
                if (czy_zywa(gorna, srodkowa, dolna, j)) {
                    if (*pocz_w == INT_MAX) *pocz_w = i;
                    *pocz_k = (j < *pocz_k) ? j : *pocz_k;
                    *kon_w = (i > *kon_w) ? i : *kon_w;
                    *kon_k = (j > *kon_k) ? j : *kon_k;
                    *liczba_zywych += 1;
                    dodaj_kolumne_za(pom_z, j);
                    pom_z = pom_z->nast;
                    if (!jest_zywa) jest_zywa = true;
                }
                j++;
            }
            usun_atrape_kolumny(&pom_w->nast->kom, jest_zywa);
            if (pom_w->nast->kom == NULL) {
                free(pom_w->nast);
                pom_w->nast = NULL;
            } else {
                pom_w = pom_w->nast;
            }
        }
        i++;
    }
    usun_atrape_wiersza(&nowe, true);
    zwolnij_liste_wierszy(plansza);
    return nowe;
}

/* Funkcja wypisuje status pokolenia */
void wypisz_status(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j) {
    if (b == 0) {
        c = 0;
        d = 0;
        e = 0;
        f = 0;
    }
    printf("%d {%d} (%d:%d) (%d:%d) [%d:%d] [%d:%d]\n", a, b, c, d, e, f, g, h, i, j);
}

/* Funkcja zapisuje liczbe z wejscia */
void zapisz_liczbe(char *c, int *x, int *licznik) {
    if (*c != '\n') {
        scanf("%d", x);
        *licznik += 1;
        *c = getchar();
    }
}

/* Funkcja aktualizuje wspolrzedne okna */
void aktualizuj_wspolrzedne_okna(int *g, int *h, int *i, int *j, int w, int x) {
    int wiersze = *i - *g, kolumny = *j - *h;
    *g = w;
    *h = x;
    *i = *g + wiersze;
    *j = *h + kolumny;
}

/* Funkcja wczytuje polecenia z wejscia i wykonuje je */
void rozgrywka(wiersze *plansza, int *pocz_w, int *pocz_k, int *kon_w, int *kon_k, int *liczba_zywych, int liczba_w, int liczba_k) {
    wiersze *nowe = NULL;
    int nr_generacji = 0, licznik;
    int g = 1, h = 1, i = liczba_w, j = liczba_k;
    char c;
    wypisz_plansze(plansza, g, h, i, j);
    wypisz_status(nr_generacji, *liczba_zywych, *pocz_w, *pocz_k, *kon_w, *kon_k, g, h, i, j);
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            nowe = nowe_pokolenie(plansza, pocz_w, pocz_k, kon_w, kon_k, liczba_zywych);
            nr_generacji++;
            plansza = nowe;
            wypisz_plansze(plansza, g, h, i, j);
            wypisz_status(nr_generacji, *liczba_zywych, *pocz_w, *pocz_k, *kon_w, *kon_k, g, h, i, j);
        } else {
            ungetc(c, stdin);
            int w = 0, x = 0, y = 0, z = 0;
            licznik = 0;
            zapisz_liczbe(&c, &w, &licznik);
            zapisz_liczbe(&c, &x, &licznik);
            zapisz_liczbe(&c, &y, &licznik);
            zapisz_liczbe(&c, &z, &licznik);
            if (licznik == 1) {
                for (int i = 0; i < w; i++) {
                    nowe = nowe_pokolenie(plansza, pocz_w, pocz_k, kon_w, kon_k, liczba_zywych);
                    nr_generacji++;
                    plansza = nowe;
                }
                wypisz_plansze(plansza, g, h, i, j);
                wypisz_status(nr_generacji, *liczba_zywych, *pocz_w, *pocz_k, *kon_w, *kon_k, g, h, i, j);
            } else if (licznik == 2) {
                aktualizuj_wspolrzedne_okna(&g, &h, &i, &j, w, x);
                wypisz_plansze(plansza, g, h, i, j);
                wypisz_status(nr_generacji, *liczba_zywych, *pocz_w, *pocz_k, *kon_w, *kon_k, g, h, i, j);
            } else if (licznik == 4) {
                g = w;
                h = x;
                i = y;
                j = z;
                wypisz_plansze(plansza, g, h, i, j);
                wypisz_status(nr_generacji, *liczba_zywych, *pocz_w, *pocz_k, *kon_w, *kon_k, g, h, i, j);
            }
        }
    }
    zwolnij_liste_wierszy(plansza);
}

int main(void) {
    int liczba_w = 0, liczba_k = 0;
    int pocz_w = INT_MAX, pocz_k = INT_MAX, kon_w = INT_MIN, kon_k = INT_MIN; /* pocz_w - pierwszy wiersz zawierajacy zywa komorke, kon_w - ostatni taki wiersz (odpowiednio dla kolumn) */
    int liczba_zywych = 0;
    wiersze *plansza = wczytaj(&liczba_w, &liczba_k, &liczba_zywych, &pocz_w, &pocz_k, &kon_w, &kon_k);
    rozgrywka(plansza, &pocz_w, &pocz_k, &kon_w, &kon_k, &liczba_zywych, liczba_w, liczba_k);
}
