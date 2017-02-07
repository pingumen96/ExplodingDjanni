#include <stdio.h>
#include "partita.h"

#define SOGLIA_PERICOLO_EXPLODING_DJANNI 0.25
#define SOGLIA_UTILIZZO_FAVOR 5

/* procedura che stampa a schermo la situazione attuale della partita */
void stampaSituazionePartita(Giocatore *giocatori, Mazzo *mazzo) {
    unsigned short i;

    /* si stampano varie informazioni sulla situazione attuale */
    printf("Dimensione mazzo: %hu\n", dimensioneMazzo(mazzo->listaCarte));
    printf("Situazione partita:\n");
    for(i = 0; i < N_GIOCATORI; i++) {
        printf("%23s\t", giocatori[i].nome);
        if(giocatori[i].inGioco) {
            printf("in gioco");
        } else {
            printf("ESPLOSO");
        }
        printf("\n");
    }
}



/* stampa del menù di gioco */
unsigned short sceltaMenuGioco(Giocatore *giocatori, int giocatoreCorrente, Mazzo *mazzo) {
    unsigned short sceltaGioco;

    /* variabili utili per IA */
    unsigned short i, contatoreDjanni;
    bool manoUtile = false;
    float possibileExplodingDjanni;

    printf("Cosa vuoi fare?\n");

    if(giocatori[giocatoreCorrente].carteInMano > 0) {
        printf("0. gioca carta (%hu carte nella mano)\n", giocatori[giocatoreCorrente].carteInMano);
    } else {
        printf("Non hai carte in mano.\n");
    }
    printf("1. pesca una carta e concludi turno\n");
    printf("2. salva partita\n");

    if(giocatori[giocatoreCorrente].tipo == UMANO) {
        do {
            scanf("%hu", &sceltaGioco);
            getchar();
        } while(sceltaGioco != GIOCA_CARTA && sceltaGioco != FINISCI_TURNO && sceltaGioco != SALVA_PARTITA);
    } else {
        /* intelligenza artificiale per il menù di gioco */

        /* se non si possiedono carte si conclude turno */
        if(giocatori[giocatoreCorrente].carteInMano == 0) {
            sceltaGioco = FINISCI_TURNO;
        } else {
            /****************************************************************************************************
                in base alla presenza o meno di carte che hanno utilità in questa fase, l'IA decide se giocare
                o meno
            ****************************************************************************************************/

            possibileExplodingDjanni = probabilitaExplodingDjanni(mazzo);

            contatoreDjanni = contatoreCartaTipoMano(giocatori[giocatoreCorrente].mano, DJANNI, giocatori[giocatoreCorrente].carteInMano);

            for(i = 0; i < giocatori[giocatoreCorrente].carteInMano && !manoUtile; i++) {
                /* se esiste almeno una carta utile in questa fase si decide di giocare */
                if(!(giocatori[giocatoreCorrente].mano[i].tipo != NOPE && giocatori[giocatoreCorrente].mano[i].tipo != MEOOOW &&
                     giocatori[giocatoreCorrente].mano[i].tipo == SHUFFLE && contatoreDjanni <= 1)) {

                    manoUtile = true;
                }
            }

            /****************************************************************************************************
                controlli supplementari basati sulla possibilità di apparizione di Exploding Djanni.
                Si decide di giocare una carta utile solo se il rischio di apparizione di Exploding Djanni
                è sufficientemente alto.
                La disposizione delle condizioni potrebbe apparire insensata ma facilita la modifica dei
                comportamenti della CPU (se necessaria) in base alle condizioni stesse.
            ****************************************************************************************************/
            if(manoUtile && possibileExplodingDjanni > SOGLIA_PERICOLO_EXPLODING_DJANNI) {
                sceltaGioco = GIOCA_CARTA;
            } else if(manoUtile) {
                /* se non c'è urgenza di giocare determinate carte si fanno altre valutazioni */
                if(contatoreCartaTipoMano(giocatori[giocatoreCorrente].mano, FAVOR, giocatori[giocatoreCorrente].carteInMano) && giocatori[giocatoreCorrente].carteInMano < SOGLIA_UTILIZZO_FAVOR) {
                    sceltaGioco = GIOCA_CARTA;
                } else if(contatoreCartaTipoMano(giocatori[giocatoreCorrente].mano, SHUFFLE, giocatori[giocatoreCorrente].carteInMano)) {
                    /* se c'è uno shuffle si gioca quello */
                    sceltaGioco = GIOCA_CARTA;
                } else {
                    /* se proprio non c'è nulla di utile da fare */
                    sceltaGioco = FINISCI_TURNO;
                }
            } else {
                sceltaGioco = FINISCI_TURNO;
            }
        }
    }

    return sceltaGioco;
}


unsigned short scegliCarta(Giocatore *giocatori, int giocatoreCorrente, Mazzo *mazzo) {
    unsigned short i, scelta;
    if(giocatori[giocatoreCorrente].tipo == UMANO) {
        printf("Scegli quale carta giocare:\n");
        for(i = 0; i < giocatori[giocatoreCorrente].carteInMano; i++) {
            printf("%hu. ", i);
            stampaCarta(giocatori[giocatoreCorrente].mano[i]);
        }

        scanf("%hu", &scelta);
        getchar();

        /* si dice quale carta sia stata giocata */
        printf("Carta giocata: ");

        stampaCarta(giocatori[giocatoreCorrente].mano[scelta]);

        return scelta;
    } else {
        /* intelligenza artificiale decide quale carta giocare */
        return scelta;
    }
}
