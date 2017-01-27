#ifndef MAZZO_H_INCLUDED
#define MAZZO_H_INCLUDED
#define DIM_TITOLO_CARTA 30
// definizione dei tipi di carta possibili
typedef enum {NOPE, ATTACK, SKIP, FAVOR, SEE_THE_FUTURE, SHUFFLE, DJANNI, EXPLODING_DJANNI, MEOOOW} TipologiaCarta;

// definizione struttura della carta singola
typedef struct {
    TipologiaCarta tipo;
    char titoloCarta[DIM_TITOLO_CARTA + 1];
} Carta;

// definizione nodo in vista della creazione di una lista
typedef struct NodoCarta {
    Carta carta;
    struct NodoCarta *prossima;
} NodoCarta;

NodoCarta *nuovoMazzo();
NodoCarta *prependCarta(NodoCarta *testa, Carta nuovaCarta);
Carta pescaCarta();
void stampaMazzo(NodoCarta *testa);

#endif // MAZZO_H_INCLUDED
