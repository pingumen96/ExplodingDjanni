#include <stdlib.h>
#include "partita.h"

Giocatore pescaggioPrimoTurno(NodoCarta **meooowRimossi, Giocatore *giocatore, Mazzo *mazzo) {
    riceviCarte(giocatore, 4, mazzo);

    giocatore->mano = (Carta *) realloc(giocatore->mano, (giocatore->carteInMano + 1) * sizeof(Carta));
    if(giocatore->mano == NULL) {
        exit(-1);
    }
    giocatore->mano[giocatore->carteInMano] = prendiCarta(meooowRimossi, 0);

    giocatore->carteInMano += 1;
    return *giocatore;
}

void cicloDiGioco() {
}
