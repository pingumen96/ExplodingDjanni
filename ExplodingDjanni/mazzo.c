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

    /* debug */
    stampaMazzo(&mazzo);
    return mazzo;
}

/*
    Funzione che viene chiamata per mescolare il mazzo che viene passato come parametro.
    Si può scegliere se rimuovere le carte Exploding Djanni e Meooow o aggiungerle a seconda
    del valore passato nel secondo parametro.
    'a' --> aggiungi
    'r' --> rimuovi
*/
void mescolaMazzo(Mazzo *mazzo, char modalita) {
    Mazzo *mazzoMescolato = NULL;
    mazzoMescolato = (Mazzo *) malloc(sizeof(Mazzo));
    unsigned short i = 0, posizioneRandom;
    NodoCarta *corrente;

    if(modalita == 'a') {
        mazzoMescolato->listaCarte = malloc(sizeof(NodoCarta) * (mazzo->numeroCarte));
    } else if(modalita == 'r') {
        mazzoMescolato->listaCarte = malloc(sizeof(NodoCarta) * (mazzo->numeroCarte - mazzo->numeroExplodingDjanni - mazzo->numeroMeow));
        if(mazzoMescolato->listaCarte == NULL) {
            exit(-1);
        }

        /* si eliminano le carte MEOOOW e EXPLODING DJANNI dal mazzo */
        corrente = mazzo->listaCarte;
        while(corrente != NULL) {
            if(corrente->carta.tipo == MEOOOW || corrente->carta.tipo == EXPLODING_DJANNI) {
                eliminaCarta(&(mazzo->listaCarte), i);
            } else {
                i++;
            }

            corrente = corrente->prossima;
            /*printf("%u\n\n", dimensioneMazzo(mazzo));
            stampaMazzo(mazzo); */
        }
        /*printf("%u\n\n", dimensioneMazzo(mazzo));*/



        /* ora va costruito mazzo partendo dalle carte che si sono prese */
        for(i = 0; i < mazzo->numeroCarte - mazzo->numeroExplodingDjanni - mazzo->numeroMeow; i++) {
            posizioneRandom = rand() % dimensioneMazzo(mazzo);
            /* creare funzione che restituisca carta che si trova in una determinata posizione e la tolga dalla lista */
            mazzoMescolato->listaCarte = prependCarta(mazzoMescolato->listaCarte, prendiCarta(&(mazzo->listaCarte), posizioneRandom));
            stampaMazzo(mazzoMescolato);
            printf("%u\n\n", dimensioneMazzo(mazzoMescolato));
        }
    }

}


Carta prendiCarta(NodoCarta **testa, unsigned short posizione) {
    NodoCarta *temp = *testa, *successivo, *cartaCercata;
    unsigned short i;
    /* se viene presa la testa */
    if(posizione == 0) {
        *testa = temp->prossima;
        return temp->carta;
    }


    /* si cerca l'elemento che precede quello ricercato */
    for(i = 0; i < posizione - 1; i++) {
        temp = temp->prossima;
    }

    successivo = temp->prossima->prossima;

    cartaCercata = temp->prossima;

    temp->prossima = successivo;
    return cartaCercata->carta;

}

void eliminaCarta(NodoCarta **testa, unsigned short posizione) {
    NodoCarta *temp = *testa, *successivo;
    unsigned short i;
    /* se viene rimossa la testa */
    if(posizione == 0) {
        *testa = temp->prossima;
        return;
    }


    /* si cerca l'elemento che precede quello che si vuole eliminare */
    for(i = 0; i < posizione - 1; i++) {
        temp = temp->prossima;
    }

    successivo = temp->prossima->prossima;

    free(temp->prossima);

    temp->prossima = successivo;
}

NodoCarta *svuotaMazzo(NodoCarta *testa) {
    NodoCarta *temp = NULL;

    while(testa != NULL) {
        temp = testa->prossima;
        free(testa);
        testa = temp;
    }

    return testa;
}

unsigned short dimensioneMazzo(Mazzo *mazzo) {
    unsigned short contatoreCarte = 0;
    NodoCarta *corrente = mazzo->listaCarte;

    while(corrente != NULL) {
        contatoreCarte++;
        corrente = corrente->prossima;
    }

    return contatoreCarte;
}
