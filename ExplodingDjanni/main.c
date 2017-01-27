#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mazzo.h"
#include "utilita.h"

int main() {
    /* per ora solo prove */
    Mazzo mazzo;

    mazzo = caricaMazzo("explodingDjanniEasy.txt");

    stampaMazzo(&mazzo);


    return 0;
}
