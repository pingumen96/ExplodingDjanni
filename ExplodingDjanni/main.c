#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mazzo.h"
#include "utilita.h"

int main() {
    /* per ora solo prove */
    Mazzo* mazzo = NULL;


    srand(time(NULL));

    mazzo = caricaMazzo(MAZZO_HARD);

    mazzo = mescolaMazzo(mazzo, 'r');
    /*mazzo = mescolaMazzo(mazzo, 'a');*/
    printf("%u\n", dimensioneMazzo(mazzo));
    return 0;
}
