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
    unsigned short scelta, i, giocatoreCorrente;
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

                /* si determina quale sarà il giocatore a iniziare */
                giocatoreCorrente = rand() % N_GIOCATORI;
                printf("Il giocatore che inizia la partita e' %s.\n", giocatori[giocatoreCorrente].nome);

                stato = IN_GIOCO;

            } else if(scelta == CARICA_PARTITA) {
                /* codice per caricare la partita da file binario */
                stato = IN_GIOCO;
            }

        } else if(stato == IN_GIOCO) {
            /* si svolgono i turni di tutti i giocatori */
            while(giocatoreCorrente < N_GIOCATORI && !gameOver) {
                /* giocatore fa il suo turno solo se è in gioco */
                if(giocatori[giocatoreCorrente].inGioco) {
                    printf("Tocca a %s!\n", giocatori[giocatoreCorrente].nome);

                    /* se il giocatore è umano gli si chiede cosa vuole fare, altrimenti ci penserà l'intelligenza artificiale */
                    if(giocatori[giocatoreCorrente].tipo == UMANO ) {

                        /* stampa della situazione della situazione della partita */
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

                        /* si stampa la mano del giocatore */
                        stampaMano(&giocatori[giocatoreCorrente]);

                        /* si resetta la scelta */
                        scelta = 0;

                        /* giocatore effettua la scelta finché non decide di finire il turno */
                        while(scelta != FINISCI_TURNO) {
                            /* menù delle possibili scelte */
                            printf("Cosa vuoi fare?\n");

                            if(giocatori[giocatoreCorrente].carteInMano > 0) {
                                printf("0. gioca carta (%hu carte nella mano)\n", giocatori[giocatoreCorrente].carteInMano);
                            }
                            printf("1. pesca una carta e concludi turno\n");
                            printf("2. salva partita\n");
                            do {
                                scanf("%hu", &scelta);
                                getchar();
                            } while(scelta != GIOCA_CARTA && scelta != FINISCI_TURNO);

                            /* se il giocatore ha scelto di giocare una carta */
                            if(scelta == GIOCA_CARTA) {
                                /* gestione di tutto ciò che riguarda gli effetti delle carte */
                            } else if(scelta == FINISCI_TURNO) {
                                riceviCarte(&giocatori[giocatoreCorrente], 1, mazzo);
                                printf("Hai pescato %s\n", giocatori[giocatoreCorrente].mano[giocatori[giocatoreCorrente].carteInMano - 1].titoloCarta);

                                /* se il giocatore ha pescato EXPLODING DJANNI esplode ed esce dal gioco */
                                if(presenteExplodingDjanni(&giocatori[giocatoreCorrente])) {
                                    giocatori[giocatoreCorrente].inGioco = false;
                                }
                            } else if(scelta == SALVA_PARTITA) {
                                /* salvataggio della partita in un file con estensione .sav */
                            }
                        }





                    } else {
                        /* intelligenza artificiale al lavoro, ultima parte alla quale pensare */
                    }

                }

                /* si controlla se si è game over, se sì si decreta il vincitore, se no si prosegue col prossimo turno */
                if((giocatori[0].inGioco && !giocatori[1].inGioco && !giocatori[2].inGioco && !giocatori[3].inGioco) ||
                        (giocatori[1].inGioco && !giocatori[2].inGioco && !giocatori[3].inGioco && !giocatori[0].inGioco) ||
                        (giocatori[2].inGioco && !giocatori[3].inGioco && !giocatori[0].inGioco && !giocatori[1].inGioco) ||
                        (giocatori[3].inGioco && !giocatori[0].inGioco && !giocatori[1].inGioco && !giocatori[2].inGioco)) {
                    gameOver = true;

                    /* si cerca l'unico giocatore ancora in gioco */
                    while(!giocatori[giocatoreCorrente].inGioco) {
                        giocatoreCorrente = (giocatoreCorrente + 1) % N_GIOCATORI;
                    }
                } else {
                    /* si avanza al prossimo turno */
                    giocatoreCorrente = (giocatoreCorrente + 1) % N_GIOCATORI;
                }



            }
        }



    }

    /* si decreta il vincitore */
    printf("Il vincitore della partita e' %s!\n", giocatori[giocatoreCorrente].nome);

    return 0;
}
