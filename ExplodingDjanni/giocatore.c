#include <stdlib.h>
#include <stdio.h>
#include "giocatore.h"

Giocatore creaGiocatore() {
    Giocatore nuovoGiocatore;

    printf("Inserisci il nome del giocatore:\n");
    getchar();
    scanf("%24[^\n]s", nuovoGiocatore.nome);
    printf("Che tipo di giocatore vuoi che sia?\n");
    printf("0. CPU\n1. Umano\n");
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
