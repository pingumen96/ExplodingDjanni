#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "partita.h"
#include "utilita.h"
#include "giocatore.h"

int main() {
    /* variabili di gioco */
    Giocatore giocatori[N_GIOCATORI];
    Mazzo *mazzo = NULL;
    bool gameOver = false;
    unsigned short scelta, i;
    StatiGioco stato = MENU_PRINCIPALE;
    NodoCarta *meooowRimossi;

    /* si alloca memoria per il mazzo */
    mazzo = (Mazzo *) malloc(sizeof(Mazzo));
    if(!mazzo) {
        exit(-1);
    }

    meooowRimossi = (NodoCarta *) malloc(sizeof(NodoCarta));
    if(!meooowRimossi) {
        exit(-1);
    }


    /* ciclo di gioco */
    while(!gameOver) {
        if(stato == MENU_PRINCIPALE) {
            /**/
            printf("Benvenuto in Exploding Djanni!\n");
            printf("0. Nuova partita\n1. Carica partita\n");

            do {
                scanf("%hu", &scelta);
            } while(scelta != NUOVA_PARTITA && scelta != CARICA_PARTITA);


            /* setup della partita in base alla volontà di crearne una nuova o caricarla */
            if(scelta == NUOVA_PARTITA) {
                printf("Scegli la difficolta':\n");
                printf("0. Easy\n1. Medium\n2. Hard\n");
                do {
                    scanf("%hu", &scelta);
                } while(scelta != EASY && scelta != MEDIUM && scelta != HARD);

                /* in base alla difficoltà scelta si usa un mazzo diverso */
                if(scelta == EASY) {
                    mazzo = caricaMazzo(MAZZO_EASY);
                } else if(scelta == MEDIUM) {
                    mazzo = caricaMazzo(MAZZO_MEDIUM);
                } else {
                    mazzo = caricaMazzo(MAZZO_HARD);
                }

                printf("Mazzo caricato.\n");
                printf("Ora verranno creati i giocatori.\n");

                for(i = 0; i < N_GIOCATORI; i++) {
                    giocatori[i] = creaGiocatore();
                }

                /* si mescolano le carte e si rimuovono gli EXPLODING DJANNI e i MEOOOW momentaneamente */
                mazzo = mescolaMazzo(mazzo, 'r', &meooowRimossi);

                /* si distribuiscono le carte e si "attivano" i giocatori */
                for(i = 0; i < N_GIOCATORI; i++) {
                    giocatori[i] = pescaggioPrimoTurno(&meooowRimossi, &giocatori[i], mazzo);
                    giocatori[i].inGioco = true;

                    /* stampa mano, debug*/
                    stampaMano(&giocatori[0]);
                }

                /* si rimettono le carte nel mazzo e si mescola */
                mazzo = mescolaMazzo(mazzo, 'a', &meooowRimossi);
                mazzo = mescolaMazzo(mazzo, 's', &meooowRimossi);

                stato = IN_GIOCO;

            } else if(scelta == CARICA_PARTITA) {
                /* codice per caricare la partita da file binario */
                stato = IN_GIOCO;
            }

        } else if(stato == IN_GIOCO) {
            /* debug */
            /*printf("Ciao");*/
        }


        /* si controlla se si è game over */

    }

    return 0;
}
