#define DIM_NOME_SALVATAGGIO 30
#include "giocatore.h"

/* enumeratori */
typedef enum {MENU_PRINCIPALE, IN_GIOCO} StatiGioco;
typedef enum {NUOVA_PARTITA, CARICA_PARTITA} ScelteMenuPrincipale;
typedef enum {EASY, MEDIUM, HARD} Difficolta;
typedef enum {GIOCA_CARTA, FINISCI_TURNO, SALVA_PARTITA} ScelteMenuGioco;


void stampaSituazionePartita(Giocatore *giocatori, Mazzo *mazzo);
unsigned short sceltaMenuGioco(Giocatore *giocatori, int giocatoreCorrente, Mazzo *mazzo);
unsigned short scegliCarta(Giocatore *giocatori, int giocatoreCorrente, Mazzo *mazzo);
