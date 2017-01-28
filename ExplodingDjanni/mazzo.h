#ifndef MAZZO_H_INCLUDED
#define MAZZO_H_INCLUDED
#define DIM_TITOLO_CARTA 128
#define DIM_TITOLO_MAZZO 30
#define MEOOOW_DA_ESCLUDERE 4

char *MAZZO_EASY, *MAZZO_MEDIUM, *MAZZO_HARD;

/* definizione dei tipi di carta possibili */
typedef enum {EXPLODING_DJANNI, MEOOOW, SHUFFLE, NOPE, SEE_THE_FUTURE, ATTACK, SKIP, FAVOR, DJANNI} TipologiaCarta;

/* definizione struttura della carta singola */
typedef struct {
    TipologiaCarta tipo;
    char titoloCarta[DIM_TITOLO_CARTA];
} Carta;

/* definizione nodo in vista della creazione di una lista */
typedef struct NodoCarta {
    Carta carta;
    struct NodoCarta *prossima;
} NodoCarta;

typedef struct {
    unsigned short numeroCarte;
    unsigned short numeroExplodingDjanni;
    unsigned short numeroMeow;
    NodoCarta *listaCarte;
    char nomeFileMazzo[DIM_TITOLO_MAZZO];
} Mazzo;

NodoCarta *prependCarta(NodoCarta *testa, Carta nuovaCarta);
Carta pescaCarta(NodoCarta **testa);
void stampaMazzo(Mazzo *mazzo);
Mazzo *caricaMazzo(char* nomeFile);
Mazzo* mescolaMazzo(Mazzo *mazzo, char modalita);
unsigned short dimensioneMazzo(Mazzo *mazzo);
void svuotaMazzo(Mazzo *mazzo);
Carta prendiCarta(NodoCarta **testa, unsigned short posizione);
void eliminaCarta(NodoCarta **testa, unsigned short posizione);

#endif /* MAZZO_H_INCLUDED */
