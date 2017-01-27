#include "mazzo.h"
#include <stdlib.h>

NodoCarta *nuovoMazzo() {
    // crea lista vuota
    return NULL;
};

NodoCarta *prependCarta(NodoCarta *testa, Carta nuovaCarta) {
    NodoCarta *nuovaTesta;

    // si alloca memoria per la nuova testa
    nuovaTesta = (NodoCarta *) malloc(sizeof(NodoCarta));
    if(nuovaTesta == NULL) {
        exit(-1);
    }

    // si effettua il collegamento con la vecchia testa
    nuovaTesta->carta = nuovaCarta;
    nuovaTesta->prossima = testa;

    return nuovaTesta;
}

// funzione che restituisce la prima carta in cima al mazzo e la elimina dal mazzo stesso
Carta pescaCarta(NodoCarta **testa) {
    Carta cartaPescata = (*testa)->carta;
    NodoCarta **vecchiaTesta = NULL;

    // si libera lo spazio occupato dalla vecchia testa della lista
    *vecchiaTesta = *testa;
    free(*vecchiaTesta);

    // viene settata una nuova testa perché la carta presente in precedenza è stata pescata
    *testa = (*testa)->prossima;

    return cartaPescata;
}

void stampaCarta(Carta carta) {
    // a seconda del tipo di carta si stampa quello corrispondente
    switch(carta.tipo) {
    case NOPE:
        printf("NOPE");
        break;
    case ATTACK:
        printf("ATTACK");
        break;
    case SKIP:
        printf("SKIP");
        break;
    case FAVOR:
        printf("FAVOR");
        break;
    case SEE_THE_FUTURE:
        printf("SEE_THE_FUTURE");
        break;
    case SHUFFLE:
        printf("SHUFFLE");
        break;
    case DJANNI:
        printf("DJANNI");
        break;
    case EXPLODING_DJANNI:
        printf("EXPLODING DJANNI");
        break;
    case MEOOOW:
        printf("MEOOOW");
        break;
    }
}

void stampaMazzo(NodoCarta *testa) {
    NodoCarta *iteratore = testa;

    // si scorre la lista stampando gli elementi uno per uno
    while(testa->prossima != NULL) {
        stampaCarta(testa->carta);
        testa = testa->prossima;
    }
}
