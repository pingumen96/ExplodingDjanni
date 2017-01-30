#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "utilita.h"
#include "partita.h"

int main() {
    /* per ora solo prove */
    Mazzo* mazzo = NULL;
    NodoCarta *meoowRimossi = NULL;
    Giocatore primoGiocatore = creaGiocatore();

    primoGiocatore.mano = (Carta *) malloc(sizeof(Carta));
    if(primoGiocatore.mano == NULL) {
        exit(-1);
    }

    srand(time(NULL));

    mazzo = caricaMazzo(MAZZO_MEDIUM);
    mazzo = mescolaMazzo(mazzo, 'r', &meoowRimossi);
    printf("%u\n", dimensioneMazzo(mazzo->listaCarte));
    printf("%u\n", dimensioneMazzo(meoowRimossi));

    primoGiocatore = pescaggioPrimoTurno(&meoowRimossi, &primoGiocatore, mazzo);

    printf("%u\n", dimensioneMazzo(mazzo->listaCarte));
    printf("%u\n", dimensioneMazzo(meoowRimossi));

    mazzo = mescolaMazzo(mazzo, 'a', &meoowRimossi);
    mazzo = mescolaMazzo(mazzo, 's', &meoowRimossi);


    /*pescaggioPrimoTurno(&meoowRimossi, &primoGiocatore, mazzo);*/

    /*printf("Stampa dimensione mazzo: %u\n", dimensioneMazzo(mazzo));
    stampaMano(&primoGiocatore);

    mazzo = mescolaMazzo(mazzo, 'a', &meoowRimossi);

    printf("Stampa dimensione mazzo: %u\n", dimensioneMazzo(mazzo));*/

    return 0;
}
