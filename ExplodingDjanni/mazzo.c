#include "mazzo.h"
#include "utilita.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* costanti */
char *MAZZO_EASY = "explodingDjanniEasy.txt";
char *MAZZO_MEDIUM = "explodingDjanniMedium.txt";
char *MAZZO_HARD = "explodingDjanniHard.txt";


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


/* si inserisce un elemento in coda alla lista, ovvero alla fine del mazzo */
NodoCarta *appendCarta(NodoCarta *testa, Carta nuovaCarta) {
    NodoCarta *nuovoNodo = NULL;
    NodoCarta *corrente = testa;
    nuovoNodo = (NodoCarta *) malloc(sizeof(NodoCarta));
    if(nuovoNodo == NULL) {
        exit(-1);
    }

    nuovoNodo->carta = nuovaCarta;
    nuovoNodo->prossima = NULL;

    if(testa != NULL) {
        while(corrente->prossima != NULL) {
            corrente = corrente->prossima;
        }

        corrente->prossima = nuovoNodo;
        return testa;
    } else {
        return nuovoNodo;
    }

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
    } else {
        printf("EXPLODING DJANNI\t\t");
    }

    printf("%s", carta.titoloCarta);
    printf("\n");
}

void stampaMazzo(NodoCarta *testa) {
    NodoCarta *iteratore = testa;

    /* si scorre la lista stampando gli elementi uno per uno */
    while(iteratore != NULL) {
        stampaCarta(iteratore->carta);
        iteratore = iteratore->prossima;
    }
}


/******************************************************************************************
    Funzione che si occupa di caricare il mazzo salvato in un file e salvarlo in una lista
    concatenata.
******************************************************************************************/
Mazzo *caricaMazzo(char* nomeFile) {
    FILE *fileMazzo;
    NodoCarta *listaCaricata = NULL;
    Carta cartaTemporanea = {"", 0};
    int temp;
    int contatoreCaratteri = 0, i;
    Mazzo *mazzo;

    /* si alloca lo spazio necessario per il mazzo */
    mazzo = (Mazzo *) malloc(sizeof(Mazzo));
    if(mazzo == NULL) {
        exit(-1);
    }

    /* viene aperto il file */
    fileMazzo = fopen(nomeFile, "r");

    /* si leggono i dati sul numero di carte spostandosi lungo il file */
    mazzo->numeroExplodingDjanni = conversioneCifraCarattere(getc(fileMazzo));
    fseek(fileMazzo, 1, SEEK_CUR);
    mazzo->numeroMeow = conversioneCifraCarattere(getc(fileMazzo));
    fseek(fileMazzo, 1, SEEK_CUR);
    mazzo->numeroCarte = mazzo->numeroExplodingDjanni + mazzo->numeroMeow + conversioneCifraCarattere(getc(fileMazzo)) * 10 + conversioneCifraCarattere(getc(fileMazzo));
    fseek(fileMazzo, 1, SEEK_CUR);

    /* lettura da file e salvataggio nella lista */
    for(i = 0; i < mazzo->numeroCarte; i++) {
        cartaTemporanea.tipo = conversioneCifraCarattere(getc(fileMazzo));

        fseek(fileMazzo, 1, SEEK_CUR);

        /* si legge sino alla fine della riga */
        while((temp = getc(fileMazzo)) != '\n' && temp != '\r' && temp != EOF) {
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

    /*fclose(fileMazzo);*/

    mazzo->listaCarte = listaCaricata;
    strcpy(mazzo->nomeFileMazzo, nomeFile);


    /* debug */
    stampaMazzo(mazzo->listaCarte);
    return mazzo;
}

/*
    Funzione che viene chiamata per mescolare il mazzo che viene passato come parametro.
    Si può scegliere se rimuovere le carte Exploding Djanni e Meooow o aggiungerle a seconda
    del valore passato nel secondo parametro.
    'a' --> aggiungi
    'r' --> rimuovi
    's' --> standard (DA IMPLEMENTARE)
*/
Mazzo *mescolaMazzo(Mazzo *mazzo, char modalita, NodoCarta **meoowRimossi) {
    NodoCarta *mazzoMescolato = NULL;
    unsigned short i = 0, posizioneRandom, contatoreMeooowTolti = 0, dimensioneMazzoPartita;
    NodoCarta *corrente;
    Carta explodingDjanni = {"EXPLODING DJANNI", EXPLODING_DJANNI};



    if(modalita == 'a') {
        /* si aggiungono gli exploding djanni rimossi e i meooow che non sono stati pescati */
        printf("%u\n", dimensioneMazzo(mazzo->listaCarte));

        /* si reinseriscono i MEOOOW nel mazzo */
        contatoreMeooowTolti = contatoreCartaTipoMazzo(mazzo->listaCarte, MEOOOW);
        while(dimensioneMazzo(*meoowRimossi) > 0) {
            mazzo->listaCarte = prependCarta(mazzo->listaCarte, prendiCarta(meoowRimossi, 0));

            /* debug */
            /*printf("%u\n", dimensioneMazzo(mazzo->listaCarte));
            stampaMazzo(mazzo->listaCarte);
            printf("%u\n", dimensioneMazzo(*meoowRimossi));*/
        }

        /* si reinseriscono gli EXPLODING DJANNI */
        for(i = 0; i < mazzo->numeroExplodingDjanni; i++) {
            mazzo->listaCarte = prependCarta(mazzo->listaCarte, explodingDjanni);

            /* debug */
            /*printf("%u\n", dimensioneMazzo(mazzo->listaCarte));
            stampaMazzo(mazzo->listaCarte);*/
        }


    } else if(modalita == 'r') {
        /* si eliminano 4 carte MEOOOW e tutti gli EXPLODING DJANNI dal mazzo */

        corrente = mazzo->listaCarte;
        while(corrente != NULL) {
            if(corrente->carta.tipo == MEOOOW && contatoreMeooowTolti < MEOOOW_DA_PESCARE) {
                /*mazzo->numeroMeow -= 1;*/
                contatoreMeooowTolti++;
                *meoowRimossi = prependCarta(*meoowRimossi, prendiCarta(&(mazzo->listaCarte), i));
            } else if(corrente->carta.tipo == EXPLODING_DJANNI) {
                eliminaCarta(&(mazzo->listaCarte), i);
            } else {
                i++;
            }

            corrente = corrente->prossima;

            /* debug */
            /*printf("%u\n\n", dimensioneMazzo(mazzo->listaCarte));
            stampaMazzo(mazzo->listaCarte);*/
        }

        /* debug */
        /*printf("%u\n\n", dimensioneMazzo(mazzo));*/


        /* per un qualche motivo si trovano già carte all'interno del mazzo quando questo viene creato, si svuota e si toglie ogni dubbio */
        mazzoMescolato = svuotaMazzo(mazzoMescolato);

        /* ora si costruisce il mazzo mescolato partendo dalle carte che si sono prese */
        for(i = 0; i < mazzo->numeroCarte - mazzo->numeroExplodingDjanni - dimensioneMazzo(*meoowRimossi); i++) {
            posizioneRandom = rand() % dimensioneMazzo(mazzo->listaCarte);
            /* creare funzione che restituisca carta che si trova in una determinata posizione e la tolga dalla lista */
            mazzoMescolato = prependCarta(mazzoMescolato, prendiCarta(&(mazzo->listaCarte), posizioneRandom));
            /*stampaMazzo(mazzoMescolato);
            printf("%u\n\n", dimensioneMazzo(mazzoMescolato));*/
        }

        mazzo->listaCarte = mazzoMescolato;
    } else if(modalita == 's') {
        /* parte che si occupa semplicemente di mescolare il mazzo senza aggiungere o rimuovere niente */
        mazzoMescolato = NULL;

        dimensioneMazzoPartita = dimensioneMazzo(mazzo->listaCarte);
        /* ora si costruisce il mazzo mescolato partendo dalle carte che si sono prese */
        for(i = 0; i < dimensioneMazzoPartita; i++) {
            posizioneRandom = rand() % dimensioneMazzo(mazzo->listaCarte);
            /* creare funzione che restituisca carta che si trova in una determinata posizione e la tolga dalla lista */
            mazzoMescolato = prependCarta(mazzoMescolato, prendiCarta(&(mazzo->listaCarte), posizioneRandom));
            /*stampaMazzo(mazzoMescolato);
            printf("%u\n\n", dimensioneMazzo(mazzoMescolato));*/
        }


        mazzo->listaCarte = svuotaMazzo(mazzo->listaCarte);
        mazzo->listaCarte = mazzoMescolato;
    }



    return mazzo;
}


/* si rimuove una carta da una lista. La carta rimossa viene restituita. */
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

    /* si elimina la carta nella posizione selezionata */
    free(temp->prossima);

    temp->prossima = successivo;
}

NodoCarta *svuotaMazzo(NodoCarta *testa) {
    NodoCarta *temp;

    while(testa != NULL) {
        temp = testa->prossima;
        free(testa);
        testa = temp;
    }

    return NULL;
}

unsigned short dimensioneMazzo(NodoCarta *testa) {
    unsigned short contatoreCarte = 0;
    NodoCarta *corrente = testa;

    while(corrente != NULL) {
        contatoreCarte++;
        corrente = corrente->prossima;
    }

    return contatoreCarte;
}


/* funzione che conta la quantità delle carte di un tipo dato in input in un mazzo anch'esso passata in input */
unsigned short contatoreCartaTipoMazzo(NodoCarta *testa, TipologiaCarta tipoCarta) {
    unsigned short contatore = 0;
    NodoCarta *iteratore = testa;

    /* si scorre la lista stampando gli elementi uno per uno */
    while(iteratore != NULL) {
        if(iteratore->carta.tipo == tipoCarta) {
            contatore++;
        }
        iteratore = iteratore->prossima;
    }

    return contatore;
}

/* esegue l'effetto della carta SEE THE FUTURE */
bool seeTheFuture(NodoCarta *testa, unsigned short numeroCarte) {
    NodoCarta *iteratore = testa;
    unsigned short contatore = 0;
    bool rischioConcretoExpDjanni = false;

    /* si scorre la lista stampando i primi numeroCarte elementi */
    while(iteratore != NULL && contatore < numeroCarte) {
        /* si vede se esce exploding djanni */
        if(!rischioConcretoExpDjanni && iteratore->carta.tipo == EXPLODING_DJANNI) {
            rischioConcretoExpDjanni = true;
        }

        stampaCarta(iteratore->carta);
        iteratore = iteratore->prossima;

        contatore++;
    }

    return rischioConcretoExpDjanni;

}

/* procedura che inserisce una carta nel mazzo in posizione casuale */
void inserimentoCasuale(Carta carta, Mazzo *mazzo) {
    /* inserisce una carta in una posizione casuale del mazzo */
    unsigned short i, posizioneCasuale;
    NodoCarta *corrente = mazzo->listaCarte;

    /* può capitare che il mazzo sia vuoto */
    if(dimensioneMazzo(mazzo->listaCarte) == 0) {
        mazzo->listaCarte = prependCarta(mazzo->listaCarte, carta);
    } else {
        posizioneCasuale = rand() % dimensioneMazzo(mazzo->listaCarte);

        /* scorre la lista sino alla posizione nella quale verrà inserita la carta */
        for(i = 0; i < posizioneCasuale; i++) {
            corrente = corrente->prossima;
        }

        /* viene inserita la carta */
        corrente->prossima = prependCarta(corrente->prossima, carta);
    }
}

/* funzione che conta la quantità delle carte di un tipo dato in input in una mano anch'essa passata in input */
unsigned short contatoreCartaTipoMano(Carta *mano, TipologiaCarta tipoCarta, unsigned short dimensione) {
    unsigned short contatore = 0, i;

    for(i = 0; i < dimensione; i++) {
        if(mano[i].tipo == tipoCarta) {
            contatore++;
        }
    }

    return contatore;
}


/* calcolo della probabilità di pescare EXPLODING DJANNI */
float probabilitaExplodingDjanni(Mazzo *mazzo) {
    return (float) contatoreCartaTipoMazzo(mazzo->listaCarte, EXPLODING_DJANNI) / (float) dimensioneMazzo(mazzo->listaCarte);
}


/* si determina l'importanza assoluta di una carta all'interno di una partita in una scala da 1 a 8 (EXPLODING DJANNI non viene considerato) */
unsigned short valoreCarta(TipologiaCarta tipoCarta) {
    if(tipoCarta == MEOOOW) {
        return 8;
    } else if(tipoCarta == ATTACK) {
        return 7;
    } else if(tipoCarta == SKIP) {
        return 6;
    } else if(tipoCarta == NOPE) {
        return 5;
    } else if(tipoCarta == FAVOR) {
        return 4;
    } else if(tipoCarta == DJANNI) {
        return 3;
    } else if(tipoCarta == SEE_THE_FUTURE) {
        return 2;
    } else if(tipoCarta == SHUFFLE) {
        return 1;
    } else {
        return 0;
    }
}
