#include <stdio.h>
#include <stdlib.h>
#include "partita.h"

#define SOGLIA_PERICOLO_EXPLODING_DJANNI 0.15
#define SOGLIA_UTILIZZO_FAVOR 5
#define SOGLIA_MAZZO_MOLTO_RISCHIOSO 12
#define SOGLIA_MAZZO_RISCHIOSO 18
#define NUMERO_SPARATO 255

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
    unsigned short i, contatoreDjanni, carteMazzo = dimensioneMazzo(mazzo->listaCarte);
    bool manoUtile = false, serveGiocare;
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

            /* si determina se è necessario giocare o meno */
            if(possibileExplodingDjanni > SOGLIA_PERICOLO_EXPLODING_DJANNI && carteMazzo < SOGLIA_MAZZO_RISCHIOSO) {
                serveGiocare = true;
            } else {
                /***************************************************************************************************************
                    la CPU decide, più o meno come un essere umano, se giocare o meno nonostante non sia strettamente necessario.
                    C'è 1/4 di probabilità che la CPU decida di giocare.
                ***************************************************************************************************************/
                if(rand() % 4 <= 2) {
                    serveGiocare = false;
                } else {
                    serveGiocare = true;
                }

            }

            for(i = 0; i < giocatori[giocatoreCorrente].carteInMano && !manoUtile; i++) {
                /* se esiste almeno una carta utile in questa fase si decide di giocare */
                if(giocatori[giocatoreCorrente].mano[i].tipo == FAVOR || giocatori[giocatoreCorrente].mano[i].tipo == SEE_THE_FUTURE
                        || giocatori[giocatoreCorrente].mano[i].tipo == SHUFFLE || giocatori[giocatoreCorrente].mano[i].tipo == ATTACK
                        || giocatori[giocatoreCorrente].mano[i].tipo == SKIP || contatoreDjanni >= 2) {

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
            if(manoUtile && serveGiocare) {
                sceltaGioco = GIOCA_CARTA;

            } else {
                sceltaGioco = FINISCI_TURNO;
            }
        }
        /* si stampa la scelta effettuata dalla CPU come se fosse effettuata da un giocatore */
        if(sceltaGioco == GIOCA_CARTA) {
            printf("0");
        } else {
            printf("1");
        }

        printf("\n");
    }


    return sceltaGioco;
}


unsigned short scegliCarta(Giocatore *giocatori, int giocatoreCorrente, Mazzo *mazzo, bool rischioConcretoExpDjanni) {
    unsigned short i, scelta = NUMERO_SPARATO, carteMazzo = dimensioneMazzo(mazzo->listaCarte);
    PericoloExplodingDjanni semaforo;
    float possibileExplodingDjanni = probabilitaExplodingDjanni(mazzo);
    TipologiaCarta tipoCartaScelta;

    /* si assegna numero che non corrisponde a nessuna tipologia di carta in modo da facilitare il while successivo (per la CPU) */
    tipoCartaScelta = NUMERO_SPARATO;

    printf("Scegli quale carta giocare:\n");
    for(i = 0; i < giocatori[giocatoreCorrente].carteInMano; i++) {
        printf("%hu. ", i);
        stampaCarta(giocatori[giocatoreCorrente].mano[i]);
    }

    if(giocatori[giocatoreCorrente].tipo == UMANO) {

        scanf("%hu", &scelta);
        /*getchar();*/

        /* si dice quale carta sia stata giocata */
        printf("Carta giocata: ");

        stampaCarta(giocatori[giocatoreCorrente].mano[scelta]);

        return scelta;
    } else {
        /********************************************************************************************************
            intelligenza artificiale decide quale carta giocare.
            Esistono tre livelli di rischio, indicati idealmente con un semaforo che può essere:
            - ROSSO -> PERICOLO ALTO
            - GIALLO -> PERICOLO MEDIO
            - VERDE -> PERICOLO BASSO
            In base al pericolo e in base alle carte che si possiedono ci si comporta in maniera differente.
            La prima cosa che si fa è quindi quantificare il pericolo in cui ci si trova.
        *********************************************************************************************************/

        if((possibileExplodingDjanni > SOGLIA_PERICOLO_EXPLODING_DJANNI && carteMazzo < SOGLIA_MAZZO_MOLTO_RISCHIOSO) || rischioConcretoExpDjanni) {
            semaforo = ROSSO;
        } else if(possibileExplodingDjanni > SOGLIA_PERICOLO_EXPLODING_DJANNI && carteMazzo < SOGLIA_MAZZO_RISCHIOSO) {
            semaforo = GIALLO;
        } else {
            /* nel momento in cui non ci sono pericoli si decide casualmente se usare qualche carta che si userebbe in altri contesti */
            if(rand() % 2 == false) {
                semaforo = VERDE;
            } else {
                semaforo = ROSSO;
            }
        }



        while(tipoCartaScelta == NUMERO_SPARATO) {
            /* se il semaforo è GIALLO o ROSSO si gioca, se lo si possiede, un SEE THE FUTURE per capire il rischio concreto */
            if(possiedeTipoCarta(&giocatori[giocatoreCorrente], SEE_THE_FUTURE) && !rischioConcretoExpDjanni && (semaforo == GIALLO || semaforo == ROSSO)) {
                tipoCartaScelta = SEE_THE_FUTURE;
            } else if(semaforo == ROSSO) {
                if(possiedeTipoCarta(&giocatori[giocatoreCorrente], ATTACK)) {
                    tipoCartaScelta = ATTACK;
                } else if(possiedeTipoCarta(&giocatori[giocatoreCorrente], SKIP)) {
                    tipoCartaScelta = SKIP;
                } else {
                    semaforo = GIALLO;
                }
            } else if(semaforo == GIALLO) {
                if(contatoreCartaTipoMano(giocatori[giocatoreCorrente].mano, DJANNI, giocatori[giocatoreCorrente].carteInMano) >= 2) {
                    tipoCartaScelta = DJANNI;
                } else {
                    semaforo = VERDE;
                }
            } else if(semaforo == VERDE) {
                if(possiedeTipoCarta(&giocatori[giocatoreCorrente], FAVOR)) {
                    tipoCartaScelta = FAVOR;
                } else if(possiedeTipoCarta(&giocatori[giocatoreCorrente], SHUFFLE)) {
                    tipoCartaScelta = SHUFFLE;
                } else if(possiedeTipoCarta(&giocatori[giocatoreCorrente], SEE_THE_FUTURE)) {
                    tipoCartaScelta = SEE_THE_FUTURE;
                } else {
                    /* se non ci sono situazioni particolarmente rischiose si gioca in maniera più avventata */
                    semaforo = ROSSO;
                }
            }
        }

        /* ora si cerca la carta scelta nella mano e la si gioca */
        for(i = 0; i < giocatori[giocatoreCorrente].carteInMano && scelta == NUMERO_SPARATO; i++) {
            if(giocatori[giocatoreCorrente].mano[i].tipo == tipoCartaScelta) {
                scelta = i;
            }
        }

        /* si stampa la carta scelta come se fosse stato un umano a scriverlo */
        printf("%hu\n", scelta);



        return scelta;
    }
}
