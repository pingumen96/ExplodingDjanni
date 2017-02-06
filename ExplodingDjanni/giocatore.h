#include <stdbool.h>
#include "mazzo.h"
#define DIM_NOME_GIOCATORE 24
#define N_GIOCATORI 4


typedef enum {UMANO, CPU} TipoGiocatore;

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
bool presenteExplodingDjanni(Giocatore *giocatore);
Carta scartaCarta(Giocatore *giocatore, unsigned short indice);
Carta scartaCartaTipo(Giocatore *giocatore, TipologiaCarta tipoCarta);
bool possiedeTipoCarta(Giocatore *giocatore, TipologiaCarta tipoCarta);
void aggiungiCarta(Carta carta, Giocatore *giocatore);
bool esisteVincitore(Giocatore *giocatori);
