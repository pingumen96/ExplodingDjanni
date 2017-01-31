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
    unsigned short scelta;
    StatiGioco stato = MENU_PRINCIPALE;

    /* si alloca memoria per il mazzo */
    mazzo = (Mazzo *) malloc(sizeof(Mazzo));
    if(!mazzo) {
        exit(-1);
    }


    /* ciclo di gioco */
    while(!gameOver) {
        if(stato == MENU_PRINCIPALE) {
            /**/
            printf("Benvenuto in Exploding Djanni!");
            printf("1. Nuova partita\n2. Carica partita\n");

            do {
                scanf("%hu", &scelta);
            } while(scelta != NUOVA_PARTITA && scelta != CARICA_PARTITA);


            /* setup della partita in base alla volontà di crearne una nuova o caricarla */
            if(scelta == NUOVA_PARTITA) {
                printf("Scegli la difficolta':");
                printf("1. Easy\n2. Medium\n3. Hard");
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

            } else if(scelta == CARICA_PARTITA) {
                /* codice per caricare la partita da file binario */
                stato = IN_GIOCO;
            }

        } else if(stato == IN_GIOCO) {
            /**/
        }


        /* si controlla se si è game over */

    }

    return 0;
}
