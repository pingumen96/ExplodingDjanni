#include <stdbool.h>
#include "mazzo.h"
#define DIM_NOME_GIOCATORE 24
#define N_GIOCATORI 4


typedef enum {CPU, UMANO} TipoGiocatore;

typedef struct {
    char nome[DIM_NOME_GIOCATORE];
    bool inGioco;
    int carteInMano;
    TipoGiocatore tipo;
    Carta *mano;
} Giocatore;

Giocatore creaGiocatore();
Giocatore pescaggioPrimoTurno(NodoCarta **meooowRimossi, Giocatore *giocatore, Mazzo *mazzo);
void riceviCarte(Giocatore *giocatore, unsigned short numeroCarte, Mazzo *mazzo);
void stampaMano(Giocatore *giocatore);
