#include "mazzo.h"
#include "utilita.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
    Funzione che inserisce una nuova carta in testa alla lista.
*/
NodoCarta *prependCarta(NodoCarta *testa, Carta nuovaCarta) {
    NodoCarta *nuovaTesta = NULL;

    /* si alloca memoria per la nuova testa */
    nuovaTesta = (NodoCarta *) malloc(sizeof(NodoCarta));
    if(nuovaTesta == NULL) {
        exit(-1);
    }

    /* si effettua il collegamento con la vecchia testa */
    nuovaTesta->carta = nuovaCarta;
    nuovaTesta->prossima = testa;

    return nuovaTesta;
}

/* funzione che restituisce la prima carta in cima al mazzo e la elimina dal mazzo stesso */
Carta pescaCarta(NodoCarta **testa) {
    Carta cartaPescata = (*testa)->carta;
    NodoCarta **vecchiaTesta = NULL;

    /* si libera lo spazio occupato dalla vecchia testa della lista */
    *vecchiaTesta = *testa;
    free(*vecchiaTesta);

    /* viene settata una nuova testa perché la carta presente in precedenza è stata pescata */
    *testa = (*testa)->prossima;

    return cartaPescata;
}

void stampaCarta(Carta carta) {
    /* a seconda del tipo di carta si stampa quello corrispondente */
    if(carta.tipo == NOPE) {
        printf("NOPE\t\t");
    } else if(carta.tipo == MEOOOW) {
        printf("MEOOOW\t\t");
    } else if(carta.tipo == SHUFFLE) {
        printf("SHUFFLE\t\t");
    } else if(carta.tipo == SEE_THE_FUTURE) {
        printf("SEE THE FUTURE\t");
    } else if(carta.tipo == ATTACK) {
        printf("ATTACK\t\t");
    } else if(carta.tipo == SKIP) {
        printf("SKIP\t\t");
    } else if(carta.tipo == FAVOR) {
        printf("FAVOR\t\t");
    } else if(carta.tipo == DJANNI) {
        printf("DJANNI\t\t");
    }

    printf("%s", carta.titoloCarta);
    printf("\n");
}

void stampaMazzo(Mazzo *mazzo) {
    NodoCarta *iteratore = mazzo->listaCarte;

    /* si scorre la lista stampando gli elementi uno per uno */
    while(iteratore != NULL) {
        stampaCarta(iteratore->carta);
        iteratore = iteratore->prossima;
    }
}


/*
    Funzione che si occupa di caricare il mazzo salvato in un file e salvarlo in una lista
    concatenata.
*/
Mazzo caricaMazzo(char* nomeFile) {
    FILE *fileMazzo;
    NodoCarta *listaCaricata = NULL;
    Carta cartaTemporanea = {0, ""};
    char temp;
    int contatoreCaratteri = 0, i;
    Mazzo mazzo;

    /* viene aperto il file */
    fileMazzo = fopen(nomeFile, "r");

    /* si leggono i dati sul numero di carte */
    mazzo.numeroExplodingDjanni = conversioneCifraCarattere(getc(fileMazzo));
    fseek(fileMazzo, 1, SEEK_CUR);
    mazzo.numeroMeow = conversioneCifraCarattere(getc(fileMazzo));
    fseek(fileMazzo, 1, SEEK_CUR);
    mazzo.numeroCarte = mazzo.numeroExplodingDjanni + mazzo.numeroMeow + conversioneCifraCarattere(getc(fileMazzo)) * 10 + conversioneCifraCarattere(getc(fileMazzo));
    fseek(fileMazzo, 1, SEEK_CUR);

    /* lettura da file e salvataggio nella lista */
    for(i = 0; i < mazzo.numeroCarte; i++) {
        cartaTemporanea.tipo = conversioneCifraCarattere(getc(fileMazzo));

        fseek(fileMazzo, 1, SEEK_CUR);

        /* si legge sino alla fine della riga */
        while((temp = getc(fileMazzo)) != '\n' && temp != '\r') {
            cartaTemporanea.titoloCarta[contatoreCaratteri] = temp;
            contatoreCaratteri++;
        }


        /* si aggiunge la carta alla lista */
        listaCaricata = prependCarta(listaCaricata, cartaTemporanea);

        /* si resetta tutto per la prossima carta da leggere */
        while(contatoreCaratteri != 0) {
            cartaTemporanea.titoloCarta[contatoreCaratteri - 1] = '\0';
            contatoreCaratteri--;
        }
        cartaTemporanea.tipo = 0;

    }

    mazzo.listaCarte = listaCaricata;
    return mazzo;
}


NodoCarta *prendiCarta(unsigned short posizione, Mazzo *mazzo) {
    unsigned short posizioneFutura;
}


void mescolaMazzo(NodoCarta **testa) {
    /* prima necessaria funzione che restituisca n-esimo elemento della lista e lo rimuova dalla lista */
}
