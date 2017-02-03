#include "utilita.h"
#include <math.h>

int conversioneCifraCarattere(char carattere) {
    switch(carattere) {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    default:
        return -1;
    }
}

bool cifra(char carattere) {
    switch(carattere) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        default:
            return false;
    }
}

/* funzione che si occupa di leggere un numero intero presente in una stringa */
int convertiStringaIntero(char *stringa) {
    unsigned short i = 0;
    unsigned int numeroCifre = 0, numeroConvertito = 0;

    while(cifra(stringa[i])) {
        numeroCifre++;
        i++;
    }

    for(i = 0; i < numeroCifre; i++) {
        numeroConvertito += conversioneCifraCarattere(stringa[numeroCifre - i - 1]) * pow(10, i);
    }

    return numeroConvertito;
}
