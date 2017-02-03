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
    bool gameOver = false, saltaPesca = false, turnoDoppio = false;
    char sceltaSiNo = 0;
    unsigned short scelta, sceltaMenuPrincipale, sceltaGioco = 0, i, giocatoreCorrente, giocatoreSuccessivo, vittimaAttack;
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

    /* generazione casuale */
    srand(time(NULL));


    /* ciclo di gioco */
    while(!gameOver) {
        if(stato == MENU_PRINCIPALE) {
            /* menù principale */
            printf("Benvenuto in Exploding Djanni!\n");
            printf("0. Nuova partita\n1. Carica partita\n");

            do {
                scanf("%hu", &sceltaMenuPrincipale);
            } while(sceltaMenuPrincipale != NUOVA_PARTITA && sceltaMenuPrincipale != CARICA_PARTITA);


            /* setup della partita in base alla volontà di crearne una nuova o caricarla */
            if(sceltaMenuPrincipale == NUOVA_PARTITA) {
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
                    stampaMano(&giocatori[i]);
                }

                /* si rimettono le carte nel mazzo e si mescola */
                mazzo = mescolaMazzo(mazzo, 'a', &meooowRimossi);
                mazzo = mescolaMazzo(mazzo, 's', &meooowRimossi);

                /* si determina quale sarà il giocatore a iniziare */
                giocatoreCorrente = rand() % N_GIOCATORI;
                printf("Il giocatore che inizia la partita e' %s.\n", giocatori[giocatoreCorrente].nome);

                stato = IN_GIOCO;

            } else if(sceltaMenuPrincipale == CARICA_PARTITA) {
                /* codice per caricare la partita da file binario */
                stato = IN_GIOCO;
            }

        } else if(stato == IN_GIOCO) {


            /* si svolgono i turni di tutti i giocatori */
            while(giocatoreCorrente < N_GIOCATORI && !gameOver) {
                giocatoreSuccessivo = giocatoreCorrente;
                do {
                    giocatoreSuccessivo = (giocatoreSuccessivo + 1) % N_GIOCATORI;
                } while(!giocatori[giocatoreSuccessivo].inGioco);


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
                        sceltaGioco = 0;

                        /* giocatore effettua la scelta finché non decide di finire il turno */
                        while(sceltaGioco != FINISCI_TURNO) {
                            /* menù delle possibili scelte */
                            printf("Cosa vuoi fare?\n");

                            if(giocatori[giocatoreCorrente].carteInMano > 0) {
                                printf("0. gioca carta (%hu carte nella mano)\n", giocatori[giocatoreCorrente].carteInMano);
                            } else {
                                printf("Non hai carte in mano.\n");
                            }
                            printf("1. pesca una carta e concludi turno\n");
                            printf("2. salva partita\n");
                            do {
                                scanf("%hu", &sceltaGioco);
                                getchar();
                            } while(sceltaGioco != GIOCA_CARTA && sceltaGioco != FINISCI_TURNO);

                            /* se il giocatore ha scelto di giocare una carta */
                            if(sceltaGioco == GIOCA_CARTA) {
                                /* gestione di tutto ciò che riguarda gli effetti delle carte */
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

                                /* attivazione effetto della carta */

                                /* si controlla se il giocatore successivo (ancora in gioco) possiede un NOPE per bloccare l'effetto se possibile */


                                if(possiedeTipoCarta(&giocatori[giocatoreSuccessivo], NOPE) &&
                                        giocatori[giocatoreCorrente].mano[scelta].tipo != EXPLODING_DJANNI &&
                                        giocatori[giocatoreCorrente].mano[scelta].tipo != MEOOOW) {
                                    printf("%s, vuoi annullare l'effetto della carta giocata da %s? (s/n)\n", giocatori[giocatoreSuccessivo].nome, giocatori[giocatoreCorrente].nome);
                                    do {
                                        scanf("%c", &sceltaSiNo);
                                        getchar();

                                    } while(sceltaSiNo != 'n' && sceltaSiNo != 's');
                                }

                                if(sceltaSiNo == 's') {
                                    /* si scarta il NOPE del giocatore successivo */
                                    scartaCartaTipo(&giocatori[giocatoreSuccessivo], NOPE);
                                } else {
                                    /* si attiva effetto della carta giocata, diverso a seconda del tipo */
                                    if(giocatori[giocatoreCorrente].mano[scelta].tipo == SHUFFLE) {
                                        /* mazzo viene mischiato */
                                        mazzo = mescolaMazzo(mazzo, 's', &meooowRimossi);
                                        printf("Il mazzo è stato mescolato.\n");
                                    } else if(giocatori[giocatoreCorrente].mano[scelta].tipo == SKIP) {
                                        saltaPesca = true;
                                    } else if(giocatori[giocatoreCorrente].mano[scelta].tipo == ATTACK) {
                                        turnoDoppio = true;
                                        vittimaAttack = giocatoreSuccessivo;
                                    }
                                }

                                /* infine la carta giocata viene scartata dal gioco */
                                scartaCarta(&giocatori[giocatoreCorrente], scelta);

                            } else if(sceltaGioco == FINISCI_TURNO) {
                                /* se il giocatore non deve saltare la pesca (come nel caso in cui abbia giocato una carta SKIP) */
                                if(!saltaPesca) {
                                    riceviCarte(&giocatori[giocatoreCorrente], 1, mazzo);
                                    printf("Hai pescato ");
                                    stampaCarta(giocatori[giocatoreCorrente].mano[giocatori[giocatoreCorrente].carteInMano - 1]);
                                    printf("\n");

                                    /*
                                        se il giocatore ha pescato EXPLODING DJANNI esplode ed esce dal gioco a meno che non utilizzi
                                        un MEOOOW
                                    */
                                    if(possiedeTipoCarta(&giocatori[giocatoreCorrente], EXPLODING_DJANNI) &&
                                            !possiedeTipoCarta(&giocatori[giocatoreCorrente], MEOOOW)) {
                                        giocatori[giocatoreCorrente].inGioco = false;
                                    } else if(possiedeTipoCarta(&giocatori[giocatoreCorrente], EXPLODING_DJANNI) &&
                                              possiedeTipoCarta(&giocatori[giocatoreCorrente], MEOOOW)) {
                                        printf("%s, vuoi giocare la tua carta MEOOOW per non uscire dal gioco? (s/n)\n", giocatori[giocatoreCorrente].nome);

                                        do {
                                            scanf("%c", &sceltaSiNo);
                                            getchar();
                                        } while(sceltaSiNo != 's' && sceltaSiNo != 'n');

                                        if(sceltaSiNo == 's') {
                                            /* scarta il MEOOOW */
                                            scartaCartaTipo(&giocatori[giocatoreCorrente], MEOOOW);

                                            /* si rimette EXPLODING DJANNI nel mazzo in un posto casuale */
                                            mazzo->listaCarte = prependCarta(mazzo->listaCarte, scartaCartaTipo(&giocatori[giocatoreCorrente], EXPLODING_DJANNI));

                                            /* si mescola mazzo */
                                            mazzo = mescolaMazzo(mazzo, 's', &meooowRimossi);


                                        } else {
                                            /* se il giocatore, abbastanza stupidamente, non utilizza il MEOOOW di cui dispone, esplode */
                                            giocatori[giocatoreCorrente].inGioco = false;
                                        }
                                    }


                                }
                            } else if(sceltaGioco == SALVA_PARTITA) {
                                /* salvataggio della partita in un file con estensione .sav */
                            }


                            sceltaSiNo = 0;
                        }

                        sceltaSiNo = 0;


                    } else {
                        /* intelligenza artificiale al lavoro, ultima parte alla quale pensare */
                    }

                }

                /* si controlla se si è game over, se sì si decreta il vincitore, se no si prosegue col prossimo turno */
                /* al posto di questa condizione enorme si deve creare una funzione */
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
                    /****************************************************************************************************
                        si avanza al prossimo turno, se il giocatore è vittima della carta ATTACK resterà lo stesso anche
                        nel prossimo turno
                    ****************************************************************************************************/
                    if(turnoDoppio && vittimaAttack == giocatoreCorrente) {
                        /* giocatore corrente non cambia */
                        turnoDoppio = false;
                    } else {
                        giocatoreCorrente = (giocatoreCorrente + 1) % N_GIOCATORI;
                    }
                }


                /* reset delle variabili */
                sceltaSiNo = 0;
                saltaPesca = false;
                sceltaGioco = 0;



            }
        }



    }

    /* si annuncia il vincitore, ovvero l'ultimo ad essere rimasto in gioco */
    printf("Il vincitore della partita e' %s!\n", giocatori[giocatoreCorrente].nome);

    return 0;
}
