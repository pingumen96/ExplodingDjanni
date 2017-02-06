#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "partita.h"
#include "utilita.h"
#include "giocatore.h"

int main() {
    /* variabili di gioco */
    Giocatore giocatori[N_GIOCATORI];
    Mazzo *mazzo = NULL;
    bool gameOver = false, saltaPesca = false, turnoDoppio = false, doppioDjanni = false, triploDjanni = false;
    char sceltaSiNo = 0;
    char nomePartitaCaricata[DIM_NOME_SALVATAGGIO], nomePartitaSalvata[DIM_NOME_SALVATAGGIO];
    FILE *fileCaricamento, *fileSalvataggio, *loggerPartita;
    int giocatoreCorrente, giocatoreSuccessivo, vittimaAttack, giocatoreCartaRubata;
    unsigned short scelta, sceltaMenuPrincipale, sceltaGioco = 0, i, j, contatoreDjanniUsati,
                contatoreDjanniMano, contatoreTurniPartita = 0;
    StatiGioco stato = MENU_PRINCIPALE;
    NodoCarta *meooowRimossi, *appoggioScorrimentoLista;
    Carta cartaTemp;


    /* si crea il logger della partita */
    loggerPartita = fopen("logger_partita.txt", "w+");
    if(loggerPartita == NULL) {
        printf("Non e' stato possibile creare il file.\n");
        exit(-1);
    }

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
                    /*stampaMano(&giocatori[i]);*/
                }
                fprintf(loggerPartita, "TURNO %hu LE CARTE SONO STATE ASSEGNATE\n", contatoreTurniPartita);

                for(i = 0; i < N_GIOCATORI; i++) {
                    fprintf(loggerPartita, "TURNO %hu %s POSSIEDE ", contatoreTurniPartita, giocatori[i].nome);
                    for(j = 0; j < giocatori[i].carteInMano; j++) {
                        fprintf(loggerPartita, "%s ", giocatori[i].mano[j].titoloCarta);
                        if(j < giocatori[i].carteInMano - 1) {
                            fprintf(loggerPartita, "/ ");
                        }
                    }

                    fprintf(loggerPartita, "\n");
                }

                /* si rimettono le carte nel mazzo e si mescola */
                mazzo = mescolaMazzo(mazzo, 'a', &meooowRimossi);
                mazzo = mescolaMazzo(mazzo, 's', &meooowRimossi);

                /* si determina quale sarà il giocatore a iniziare */
                giocatoreCorrente = rand() % N_GIOCATORI;
                printf("Il giocatore che inizia la partita e' %s.\n", giocatori[giocatoreCorrente].nome);
                fprintf(loggerPartita, "TURNO %hu %s INIZIERA' LA PARTITA\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome);

                stato = IN_GIOCO;

            } else if(sceltaMenuPrincipale == CARICA_PARTITA) {
                /* inizio codice per caricare la partita da file binario */
                printf("Come si chiama il file di salvataggio? (con l'estensione)\n");
                scanf("%s", nomePartitaCaricata);
                getchar();

                /* si apre il file */
                fileCaricamento = fopen(nomePartitaCaricata, "rb");
                if(fileCaricamento == NULL) {
                    exit(-1);
                }

                /* inizia lettura del file */
                /* si leggono i giocatori */
                for(i = 0; i < N_GIOCATORI; i++) {

                    /* si legge il nome del giocatore */
                    for(j = 0; j < DIM_NOME_GIOCATORE; j++) {
                        fread(&(giocatori[i].nome[j]), sizeof(char), 1, fileCaricamento);
                    }

                    /* si legge se il giocatore è in gioco */
                    fread(&giocatori[i].inGioco, sizeof(bool), 1, fileCaricamento);

                    /* se il giocatore è in gioco si legge la mano */
                    if(giocatori[i].inGioco) {
                        /* si legge il numero di carte possedute */
                        fread(&giocatori[i].carteInMano, sizeof(int), 1, fileCaricamento);
                        fread(&giocatori[i].tipo, sizeof(TipoGiocatore), 1, fileCaricamento);

                        /* si alloca lo spazio necessario per la mano del giocatore */
                        giocatori[i].mano = (Carta *) malloc(sizeof(Carta) * giocatori[i].carteInMano);
                        if(!giocatori[i].mano) {
                            exit(-1);
                        }

                        /* si leggono le carte della mano*/
                        for(j = 0; j < giocatori[i].carteInMano; j++) {
                            fread(&giocatori[i].mano[j], sizeof(Carta), 1, fileCaricamento);
                        }
                    }
                }

                /* lettura del numero di carte del mazzo */
                fread(&(mazzo->numeroCarte), sizeof(int), 1, fileCaricamento);

                /* si leggono le varie carte */
                for(i = 0; i < mazzo->numeroCarte; i++) {
                    fread(&cartaTemp, sizeof(Carta), 1, fileCaricamento);
                    mazzo->listaCarte = prependCarta(mazzo->listaCarte, cartaTemp);
                }

                fread(&giocatoreCorrente, sizeof(int), 1, fileCaricamento);
                fread(&turnoDoppio, sizeof(bool), 1, fileCaricamento);
                vittimaAttack = giocatoreCorrente;


                /* si chiude il salvataggio */
                fclose(fileCaricamento);
                fprintf(loggerPartita, "TURNO %hu IL SALVATAGGIO E' STATO CARICATO\n", contatoreTurniPartita);

                /* fine codice per caricare la partita da file binario */
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
                    fprintf(loggerPartita, "TURNO %hu ORA TOCCA A %s\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome);

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
                            } while(sceltaGioco != GIOCA_CARTA && sceltaGioco != FINISCI_TURNO && sceltaGioco != SALVA_PARTITA);

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
                                fprintf(loggerPartita, "TURNO %hu %s HA GIOCATO %s\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome, giocatori[giocatoreCorrente].mano[scelta].titoloCarta);

                                /* attivazione effetto della carta */

                                /* si controlla se i giocatori (ancora in gioco) possiedono un NOPE per bloccare l'effetto se possibile */
                                if(giocatori[giocatoreCorrente].mano[scelta].tipo != EXPLODING_DJANNI &&
                                        giocatori[giocatoreCorrente].mano[scelta].tipo != MEOOOW) {
                                    for(i = 0; i < N_GIOCATORI && sceltaSiNo != 's'; i++) {
                                        /* prima di tutto si controlla se il giocatore è in gioco */
                                        if(giocatori[i].inGioco && possiedeTipoCarta(&giocatori[i], NOPE) && i != giocatoreCorrente && giocatori[i].inGioco) {
                                            printf("%s, vuoi annullare l'effetto della carta giocata da %s? (s/n)\n", giocatori[i].nome, giocatori[giocatoreCorrente].nome);
                                            do {
                                                scanf("%c", &sceltaSiNo);
                                                getchar();

                                            } while(sceltaSiNo != 'n' && sceltaSiNo != 's');
                                        }
                                    }
                                }

                                if(sceltaSiNo == 's') {
                                    /* si scarta il NOPE del giocatore che l'ha usato */
                                    scartaCartaTipo(&giocatori[i - 1], NOPE);
                                    fprintf(loggerPartita, "TURNO %hu L'EFFETTO DELLA CARTA GIOCATA DA %s E' STATO ANNULLATO DAL NOPE DI %s\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome, giocatori[i - 1].nome);
                                } else {
                                    /* si attiva effetto della carta giocata, diverso a seconda del tipo */
                                    if(giocatori[giocatoreCorrente].mano[scelta].tipo == SHUFFLE) {
                                        /* mazzo viene mischiato */
                                        mazzo = mescolaMazzo(mazzo, 's', &meooowRimossi);
                                        printf("Il mazzo è stato mescolato.\n");
                                        fprintf(loggerPartita, "TURNO %hu IL MAZZO E' STATO MESCOLATO\n", contatoreTurniPartita);
                                    } else if(giocatori[giocatoreCorrente].mano[scelta].tipo == SKIP) {
                                        saltaPesca = true;
                                        fprintf(loggerPartita, "TURNO %hu %s NON PESCHERA' NESSUNA CARTA AL TERMINE DEL TURNO\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome);
                                    } else if(giocatori[giocatoreCorrente].mano[scelta].tipo == ATTACK) {
                                        /* a qualcuno tocca fare il turno doppio :D */
                                        turnoDoppio = true;
                                        vittimaAttack = giocatoreSuccessivo;

                                        fprintf(loggerPartita, "TURNO %hu %s FARA' IL SUO TURNO DUE VOLTE\n", contatoreTurniPartita, giocatori[giocatoreSuccessivo].nome);

                                        /* si conclude il turno del giocatore */
                                        sceltaGioco = FINISCI_TURNO;
                                    } else if(giocatori[giocatoreCorrente].mano[scelta].tipo == FAVOR) {
                                        /* comportamento diverso a seconda che il giocatore scelto sia la CPU o meno */
                                        if(giocatori[giocatoreCorrente].tipo == UMANO) {
                                            printf("A quale dei tuoi avversari vuoi rubare una carta? (verra' scelta da lui)\n");
                                            for(i = 0; i < N_GIOCATORI; i++) {
                                                /* stampa dei giocatori in gioco diversi da quello corrente */
                                                if(i != giocatoreCorrente && giocatori[i].inGioco) {
                                                    printf("%hu. %s\n", i, giocatori[i].nome);
                                                }
                                            }
                                            do {
                                                scanf("%d", &giocatoreCartaRubata);
                                            } while(giocatoreCartaRubata > N_GIOCATORI &&
                                                    !giocatori[giocatoreCartaRubata].inGioco &&
                                                    giocatoreCartaRubata == giocatoreCorrente);

                                            printf("%s, scegli quale carta dare a %s:\n", giocatori[giocatoreCartaRubata].nome, giocatori[giocatoreCorrente].nome);

                                            for(i = 0; i < giocatori[giocatoreCartaRubata].carteInMano; i++) {
                                                printf("%hu. ", i);
                                                stampaCarta(giocatori[giocatoreCartaRubata].mano[i]);
                                            }

                                            do {
                                                scanf("%hu", &scelta);
                                                getchar();
                                            } while(scelta >= giocatori[giocatoreCartaRubata].carteInMano);

                                            fprintf(loggerPartita, "TURNO %hu %s HA RUBATO LA CARTA %s DA %s\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome, giocatori[giocatoreCartaRubata].mano[scelta].titoloCarta, giocatori[giocatoreCartaRubata].nome);
                                            aggiungiCarta(scartaCarta(&giocatori[giocatoreCartaRubata], scelta), &giocatori[giocatoreCorrente]);
                                        } else {
                                            /* gestione AI */
                                        }
                                    } else if(giocatori[giocatoreCorrente].mano[scelta].tipo == SEE_THE_FUTURE) {
                                        printf("Ecco le prime tre carte in cima al mazzo:\n");
                                        seeTheFuture(mazzo->listaCarte, CARTE_SEE_THE_FUTURE);
                                        fprintf(loggerPartita, "TURNO %hu %s HA VISTO LE PRIME TRE CARTE DEL MAZZO\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome);
                                    } else if(giocatori[giocatoreCorrente].mano[scelta].tipo == DJANNI) {
                                        /*
                                            se giocata da sola non ha effetto, se giocata in tripla, il giocatore
                                            deve scegliere un avversario e prenderà a caso una sua carta; se giocata
                                            in tripla, il giocatore deve scegliere un avversario e prenderà una sua
                                            carta a scelta
                                        */
                                        contatoreDjanniUsati = 1;
                                        contatoreDjanniMano = contatoreCartaTipoMano(giocatori[giocatoreCorrente].mano, DJANNI, giocatori[giocatoreCorrente].carteInMano);
                                        if(contatoreDjanniMano >= 2) {

                                            do {
                                                printf("Vuoi usare anche un'altra Djanni card? (s/n)\n");

                                                do {
                                                    scanf("%c", &sceltaSiNo);
                                                } while(sceltaSiNo != 'n' && sceltaSiNo != 's');

                                                if(sceltaSiNo == 's') {
                                                    contatoreDjanniUsati++;
                                                }
                                            } while(sceltaSiNo != 'n' && contatoreDjanniUsati < contatoreDjanniMano);

                                            if(contatoreDjanniUsati > 2) {
                                                /* se si usano 3 o più DJANNI */
                                                /* si sceglie avversario e una carta delle sue */
                                                triploDjanni = true;
                                                if(giocatori[giocatoreCorrente].tipo == UMANO) {
                                                    printf("A quale dei tuoi avversari vuoi rubare una carta? (verra' scelta da lui)\n");
                                                    for(i = 0; i < N_GIOCATORI; i++) {
                                                        /* stampa dei giocatori in gioco diversi da quello corrente */
                                                        if(i != giocatoreCorrente && giocatori[i].inGioco) {
                                                            printf("%hu. %s\n", i, giocatori[i].nome);
                                                        }
                                                    }
                                                    do {
                                                        scanf("%d", &giocatoreCartaRubata);
                                                    } while(giocatoreCartaRubata > N_GIOCATORI &&
                                                            !giocatori[giocatoreCartaRubata].inGioco &&
                                                            giocatoreCartaRubata == giocatoreCorrente);

                                                    printf("%s, scegli quale carta rubare da %s:\n", giocatori[giocatoreCorrente].nome, giocatori[giocatoreCartaRubata].nome);

                                                    for(i = 0; i < giocatori[giocatoreCartaRubata].carteInMano; i++) {
                                                        printf("%hu. ", i);
                                                        stampaCarta(giocatori[giocatoreCartaRubata].mano[i]);
                                                    }

                                                    do {
                                                        scanf("%hu", &scelta);
                                                        getchar();
                                                    } while(scelta >= giocatori[giocatoreCartaRubata].carteInMano);
                                                    fprintf(loggerPartita, "TURNO %hu %s HA RUBATO LA CARTA %s DA %s\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome, giocatori[giocatoreCartaRubata].mano[scelta].titoloCarta, giocatori[giocatoreCartaRubata].nome);
                                                    aggiungiCarta(scartaCarta(&giocatori[giocatoreCartaRubata], scelta), &giocatori[giocatoreCorrente]);


                                                } else {
                                                    /* gestione AI */
                                                }

                                            } else if(contatoreDjanniUsati > 1) {
                                                /* se si usano 2 DJANNI */
                                                /* si sceglie avversario e una carta random delle sue */
                                                doppioDjanni = true;
                                                if(giocatori[giocatoreCorrente].tipo == UMANO) {
                                                    printf("A quale dei tuoi avversari vuoi rubare una carta? (non potrai vederla)\n");
                                                    for(i = 0; i < N_GIOCATORI; i++) {
                                                        /* stampa dei giocatori in gioco diversi da quello corrente */
                                                        if(i != giocatoreCorrente && giocatori[i].inGioco) {
                                                            printf("%hu. %s\n", i, giocatori[i].nome);
                                                        }
                                                    }
                                                    do {
                                                        scanf("%d", &giocatoreCartaRubata);
                                                    } while(giocatoreCartaRubata > N_GIOCATORI &&
                                                            !giocatori[giocatoreCartaRubata].inGioco &&
                                                            giocatoreCartaRubata == giocatoreCorrente);

                                                    printf("%s, scegli quale carta rubare da %s:\n", giocatori[giocatoreCorrente].nome, giocatori[giocatoreCartaRubata].nome);

                                                    for(i = 0; i < giocatori[giocatoreCartaRubata].carteInMano; i++) {
                                                        printf("%hu. %hua carta\n", i, i + 1);
                                                    }

                                                    do {
                                                        scanf("%hu", &scelta);
                                                        getchar();
                                                    } while(scelta >= giocatori[giocatoreCartaRubata].carteInMano);
                                                    printf("Hai rubato: ");
                                                    stampaCarta(giocatori[giocatoreCartaRubata].mano[scelta]);

                                                    fprintf(loggerPartita, "TURNO %hu %s HA RUBATO LA CARTA %s DA %s\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome, giocatori[giocatoreCartaRubata].mano[scelta].titoloCarta, giocatori[giocatoreCartaRubata].nome);
                                                    aggiungiCarta(scartaCarta(&giocatori[giocatoreCartaRubata], scelta), &giocatori[giocatoreCorrente]);


                                                } else {
                                                    /* gestione AI */
                                                }
                                            }
                                        }
                                    }
                                }

                                /* infine le carte giocate vengono scartate dal gioco */
                                if(doppioDjanni) {
                                    scartaCartaTipo(&giocatori[giocatoreCorrente], DJANNI);
                                    scartaCartaTipo(&giocatori[giocatoreCorrente], DJANNI);
                                    doppioDjanni = false;
                                } else if(triploDjanni) {
                                    scartaCartaTipo(&giocatori[giocatoreCorrente], DJANNI);
                                    scartaCartaTipo(&giocatori[giocatoreCorrente], DJANNI);
                                    scartaCartaTipo(&giocatori[giocatoreCorrente], DJANNI);
                                    triploDjanni = false;
                                } else {
                                    scartaCarta(&giocatori[giocatoreCorrente], scelta);
                                }


                            } else if(sceltaGioco == FINISCI_TURNO) {
                                /* se il giocatore non deve saltare la pesca (come nel caso in cui abbia giocato una carta SKIP) */
                                if(!saltaPesca) {
                                    riceviCarte(&giocatori[giocatoreCorrente], 1, mazzo);
                                    printf("Hai pescato ");
                                    stampaCarta(giocatori[giocatoreCorrente].mano[giocatori[giocatoreCorrente].carteInMano - 1]);
                                    printf("\n");

                                    fprintf(loggerPartita, "TURNO %hu %s HA PESCATO %s\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome, giocatori[giocatoreCorrente].mano[giocatori[giocatoreCorrente].carteInMano -1].titoloCarta);


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
                                            inserimentoCasuale(scartaCartaTipo(&giocatori[giocatoreCorrente], EXPLODING_DJANNI), mazzo);
                                            fprintf(loggerPartita, "TURNO %hu %s SI E' SALVATO USANDO MEOOOW\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome);
                                            /* debug */
                                            /*printf("EXPLODING DJANNI presenti nel mazzo: %hu\n", contatoreCartaTipoMazzo(mazzo->listaCarte, EXPLODING_DJANNI));*/

                                        } else {
                                            /* se il giocatore, abbastanza stupidamente, non utilizza il MEOOOW di cui dispone, esplode */
                                            giocatori[giocatoreCorrente].inGioco = false;
                                            fprintf(loggerPartita, "TURNO %hu %s E' FUORI DAL GIOCO\n", contatoreTurniPartita, giocatori[giocatoreCorrente].nome);
                                        }
                                    }

                                    contatoreTurniPartita++;


                                }
                            } else if(sceltaGioco == SALVA_PARTITA) {
                                /* salvataggio della partita in un file con estensione .sav */
                                printf("Scegli il nome da dare al file di salvataggio (l'estensione viene aggiunta in automatico):\n");
                                scanf("%s", nomePartitaSalvata);
                                getchar();

                                /* si crea il file */

                                strcat(nomePartitaSalvata, ".sav");

                                fileSalvataggio = fopen(nomePartitaSalvata, "wb");

                                /* ora si inizia a scrivere sul file */

                                /* i vari giocatori */
                                for(i = 0; i < N_GIOCATORI; i++) {
                                    for(j = 0; j < DIM_NOME_GIOCATORE; j++) {
                                        fwrite(&giocatori[i].nome[j], sizeof(char), 1, fileSalvataggio);
                                    }


                                    fwrite(&giocatori[i].inGioco, sizeof(bool), 1, fileSalvataggio);

                                    if(giocatori[i].inGioco) {
                                        fwrite(&giocatori[i].carteInMano, sizeof(int), 1, fileSalvataggio);
                                        fwrite(&giocatori[i].tipo, sizeof(TipoGiocatore), 1, fileSalvataggio);

                                        for(j = 0; j < giocatori[i].carteInMano; j++) {
                                            fwrite(&giocatori[i].mano[j], sizeof(Carta), 1, fileSalvataggio);
                                        }

                                    }
                                }

                                /* informazioni sulle carte */
                                mazzo->numeroCarte = dimensioneMazzo(mazzo->listaCarte);
                                fwrite(&mazzo->numeroCarte, sizeof(int), 1, fileSalvataggio);

                                appoggioScorrimentoLista = mazzo->listaCarte;

                                for(i = 0; i < mazzo->numeroCarte; i++) {
                                    /* si deve scorrere sulla lista */
                                    fwrite(&appoggioScorrimentoLista->carta, sizeof(Carta), 1, fileSalvataggio);
                                    appoggioScorrimentoLista = appoggioScorrimentoLista->prossima;
                                }

                                /* info aggiuntive */
                                fwrite(&giocatoreCorrente, sizeof(int), 1, fileSalvataggio);
                                fwrite(&turnoDoppio, sizeof(bool), 1, fileSalvataggio);

                                fclose(fileSalvataggio);
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
                if(esisteVincitore(giocatori)) {
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
    fprintf(loggerPartita, "%s HA VINTO!", giocatori[giocatoreCorrente].nome);
    fclose(loggerPartita);

    return 0;
}
