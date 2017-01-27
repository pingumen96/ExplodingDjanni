#include <stdio.h>
#include <stdlib.h>
#include "mazzo.h"
#include "utilita.h"

int main() {
    // per ora solo prove
    NodoCarta *lista = nuovoMazzo();

    lista = caricaMazzo("explodingDjanniMedium.txt");

    stampaMazzo(lista);


    return 0;
}
