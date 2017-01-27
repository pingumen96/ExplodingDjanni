#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mazzo.h"
#include "utilita.h"

int main() {
    /* per ora solo prove */
    Mazzo mazzo;

    srand(time(NULL));

    mazzo = caricaMazzo("explodingDjanniEasy.txt");

    stampaMazzo(&mazzo);
    prendiCarta(&(mazzo.listaCarte), 3);
    printf("%u\n", dimensioneMazzo(&mazzo));

    stampaMazzo(&mazzo);
    mescolaMazzo(&mazzo, 'r');

    return 0;
}
