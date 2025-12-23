#include "funkcijos.h"

int main() {
    MeniuAts m = meniu();   // meniu funkcijos iškvietimas
    TekstoNuskaitymas(m.failas);    // failo nuskaitymo funkcijos iškvietimas
    RezultatuIsvedimas(m.ivestis, m.isvedimas); // rezultatų išvedimo funkcijos iškvietimas
    return 0;
}
