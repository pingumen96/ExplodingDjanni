#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "giocatore.h"

Giocatore creaGiocatore() {
    Giocatore nuovoGiocatore;

    printf("Inserisci il nome del giocatore:\n");
    getchar();
    scanf("%24[^\n]s", nuovoGiocatore.nome);
    printf("Che tipo di giocatore vuoi che sia?\n");
    printf("0. Umano\n1. CPU\n");
    do {
        scanf("%u", &(nuovoGiocatore.tipo));
        /*getchar();*/
    } while(nuovoGiocatore.tipo != UMANO && nuovoGiocatore.tipo != CPU);

    /* si settano i dati di default */
    nuovoGiocatore.carteInMano = 0;
    nuovoGiocatore.inGioco = false;
    nuovoGiocatore.mano = NULL;

    return nuovoGiocatore;
}

Giocatore pescaggioPrimoTurno(NodoCarta **meooowRimossi, Giocatore *giocatore, Mazzo *mazzo) {
    riceviCarte(giocatore, 4, mazzo);

    giocatore->mano = (Carta *) realloc(giocatore->mano, (giocatore->carteInMano + 1) * sizeof(Carta));
    if(giocatore->mano == NULL) {
        exit(-1);
    }
    giocatore->mano[giocatore->carteInMano] = prendiCarta(meooowRimossi, 0);

    giocatore->carteInMano += 1;
    return *giocatore;
}


/* funzione che permette di far pescare al giocatore un determinato numero di carte */
void riceviCarte(Giocatore *giocatore, unsigned short numeroCarte, Mazzo *mazzo) {
    unsigned short i, carteManoPrecedente = giocatore->carteInMano;

    giocatore->carteInMano += numeroCarte;
    giocatore->mano = (Carta *) realloc(giocatore->mano, giocatore->carteInMano * sizeof(Carta));
    if(giocatore->mano == NULL) {
        exit(-1);
    }


    for(i = 0; i < numeroCarte; i++) {
        giocatore->mano[carteManoPrecedente + i] = prendiCarta(&(mazzo->listaCarte), 0);
    }
}



void stampaMano(Giocatore *giocatore) {
    unsigned short i;

    printf("Mano di %s:\n", giocatore->nome);
    for(i = 0; i < giocatore->carteInMano; i++) {
        stampaCarta(giocatore->mano[i]);
    }
}


bool presenteExplodingDjanni(Giocatore *giocatore) {
    unsigned short i;

    for(i = 0; i < giocatore->carteInMano; i++) {
        if(giocatore->mano[i].tipo == EXPLODING_DJANNI) {
            return true;
        }
    }

    return false;
}


Carta scartaCartaTipo(Giocatore *giocatore, TipologiaCarta tipoCarta) {
    unsigned short i, j = 0;
    bool scartata = false;
    Carta cartaScartata;
    Carta *nuovaMano;
    nuovaMano = (Carta *) malloc(sizeof(Carta) * (giocatore->carteInMano - 1));
    if(nuovaMano == NULL) {
        exit(-1);
    }

    /* debug */
    /*stampaMano(giocatore);*/
    /* fine debug */

    /* si mettono nel nuovo array tutte le carte della mano tranne quella scartata */
    for(i = 0; i < giocatore->carteInMano; i++) {
        if(giocatore->mano[i].tipo != tipoCarta || scartata) {
            nuovaMano[j] = giocatore->mano[i];
            j++;
        } else {
            cartaScartata = giocatore->mano[i];
            scartata = true;
        }

    }

    /* si libera la memoria occupata dal vecchio array */
    free(giocatore->mano);
    giocatore->mano = nuovaMano;
    (giocatore->carteInMano)--;

    /* debug */
    /*stampaMano(giocatore);*/

    return cartaScartata;


}

/***********************************************************************************
    funzione che permette di scartare una carta dalla mano di un giocatore.
    Si restituisce la carta scartata perché potrebbe essere utile, ma si può anche
    "lasciar cadere nel vuoto".
***********************************************************************************/
Carta scartaCarta(Giocatore *giocatore, unsigned short indice) {
    unsigned short i, j;
    Carta cartaScartata;
    Carta *nuovaMano = (Carta *) malloc(sizeof(Carta) * (giocatore->carteInMano - 1));
    if(nuovaMano == NULL) {
        exit(-1);
    }

    /* si mettono nel nuovo array tutte le carte della mano tranne quella scartata */
    for(i = 0, j = 0; i < giocatore->carteInMano; i++) {
        if(i != indice) {
            nuovaMano[j] = giocatore->mano[i];
            j++;
        } else {
            cartaScartata = giocatore->mano[i];
        }

    }

    free(giocatore->mano);
    giocatore->mano = nuovaMano;
    (giocatore->carteInMano)--;

    /* debug */
    /*stampaMano(giocatore);*/


    return cartaScartata;
}

bool possiedeTipoCarta(Giocatore *giocatore, TipologiaCarta tipoCarta) {
    unsigned short i;

    for(i = 0; i < giocatore->carteInMano; i++) {
        if(giocatore->mano[i].tipo == tipoCarta) {
            return true;
        }
    }

    return false;
}



void aggiungiCarta(Carta carta, Giocatore *giocatore) {
    unsigned short carteManoPrecedente = giocatore->carteInMano;

    giocatore->carteInMano += 1;
    giocatore->mano = (Carta *) realloc(giocatore->mano, giocatore->carteInMano * sizeof(Carta));
    if(giocatore->mano == NULL) {
        exit(-1);
    }


    giocatore->mano[carteManoPrecedente] = carta;
}


bool esisteVincitore(Giocatore *giocatori) {
    if((giocatori[0].inGioco && !giocatori[1].inGioco && !giocatori[2].inGioco && !giocatori[3].inGioco) ||
            (giocatori[1].inGioco && !giocatori[2].inGioco && !giocatori[3].inGioco && !giocatori[0].inGioco) ||
            (giocatori[2].inGioco && !giocatori[3].inGioco && !giocatori[0].inGioco && !giocatori[1].inGioco) ||
            (giocatori[3].inGioco && !giocatori[0].inGioco && !giocatori[1].inGioco && !giocatori[2].inGioco)) {
        return true;
    }

    return false;
}
