#include <stdbool.h>
#include "utilita.h"
#define DIM_TITOLO_CARTA 128
#define DIM_TITOLO_MAZZO 30
#define MEOOOW_DA_PESCARE 4
#define EXPLODING_DJANNI_RIMOSSI 3
#define CARTE_SEE_THE_FUTURE 3
#define MAX_DJANNI_UTILIZZABILI 3

char *MAZZO_EASY, *MAZZO_MEDIUM, *MAZZO_HARD;

/* definizione dei tipi di carta possibili */
typedef enum {EXPLODING_DJANNI, MEOOOW, SHUFFLE, NOPE, SEE_THE_FUTURE, ATTACK, SKIP, FAVOR, DJANNI} TipologiaCarta;

/* definizione struttura della carta singola */
typedef struct {
    char titoloCarta[DIM_TITOLO_CARTA];
    TipologiaCarta tipo;
} Carta;

/* definizione nodo in vista della creazione di una lista */
typedef struct NodoCarta {
    Carta carta;
    struct NodoCarta *prossima;
} NodoCarta;

typedef struct {
    int numeroCarte;
    unsigned short numeroExplodingDjanni;
    unsigned short numeroMeow;
    NodoCarta *listaCarte;
    char nomeFileMazzo[DIM_TITOLO_MAZZO];
} Mazzo;

NodoCarta *prependCarta(NodoCarta *testa, Carta nuovaCarta);
NodoCarta *appendCarta(NodoCarta *testa, Carta nuovaCarta);
Carta pescaCarta(NodoCarta **testa);
void stampaMazzo(NodoCarta *testa);
Mazzo *caricaMazzo(char* nomeFile);
Mazzo* mescolaMazzo(Mazzo *mazzo, char modalita, NodoCarta **meooowRimossi);
unsigned short dimensioneMazzo(NodoCarta *testa);
NodoCarta *svuotaMazzo(NodoCarta *testa);
Carta prendiCarta(NodoCarta **testa, unsigned short posizione);
void eliminaCarta(NodoCarta **testa, unsigned short posizione);
unsigned short contatoreCartaTipoMazzo(NodoCarta *testa, TipologiaCarta tipoCarta);
unsigned short contatoreCartaTipoMano(Carta *mano, TipologiaCarta tipoCarta, unsigned short dimensione);
void stampaCarta(Carta carta);
unsigned short valoreCarta(TipologiaCarta tipoCarta);
bool seeTheFuture(NodoCarta *testa, unsigned short numeroCarte);
void inserimentoCasuale(Carta carta, Mazzo *mazzo);
float probabilitaExplodingDjanni(Mazzo *mazzo);
