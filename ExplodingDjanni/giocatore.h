#ifndef GIOCATORE_H_INCLUDED
#define GIOCATORE_H_INCLUDED
#include <stdbool.h>
#define DIM_NOME_GIOCATORE 24
#define N_GIOCATORI 4

typedef enum {CPU, REALE} TipoGiocatore;

typedef struct {
    char nome[DIM_NOME_GIOCATORE];
    bool inGioco;
    int carteInMano;
    TipoGiocatore tipo;
} Giocatore;

#endif // GIOCATORE_H_INCLUDED
