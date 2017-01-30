#include <stdbool.h>
#include "mazzo.h"
#define DIM_NOME_GIOCATORE 24


typedef enum {CPU, UMANO} TipoGiocatore;

typedef struct {
    char nome[DIM_NOME_GIOCATORE];
    bool inGioco;
    int carteInMano;
    TipoGiocatore tipo;
    Carta *mano;
} Giocatore;

Giocatore creaGiocatore();
void riceviCarte(Giocatore *giocatore, unsigned short numeroCarte, Mazzo *mazzo);
void stampaMano(Giocatore *giocatore);
