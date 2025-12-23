# EgzaminoUzduotis
## Programos aprašymas
Programa leidžia naudotojui sužinoti kiek pasirinktame tekstiniame faile yra du ir daugiau kartų pasikartojančių žodžių bei sužinoti jų lokacijas (eilučių numerius). Programa taip pat turi funkcija rasti visas URL nuorodas minėtame tekstiniame faile. Rezultatai išvedami į `rezultatai.txt` failą, URL radimo atveju, naudotojui suteikiamas pasirinkimas.

## Programos naudojimo instrukcija
1. Paleidžiame programą.
2. Naudotojas įveda norimo išanalizuoti ir `failai` aplankale išsaugoto `.txt` failo pavadinimą. 
### Meniu
* 1 - rasti pasikartojančius žodžius ir jų lokacijas;<br>
*Rezultatai išvedami į failą `rezultatai.txt`.*
* 2 - rasti tekste esančias URL nuorodas;
  * 2.1 - naudotojas pasirenka kur išvedami rezultatai:
      * 1 - rezultatai išvedami į failą `rezultatai.txt`;<br>
    *Rezultatai išvedami į failą `rezultatai.txt`.*
      * 2 - rezultatai išvedami į terminalą;<br>
    *Rezultatai išvedami į terminalą.*
* 3 - rasti tekste pasikartojančius žodžius ir jų lokacijas bei visame tekste esančias URL nuorodas;<br>
*Rezultatai išvedami į failą `rezultatai.txt`.*
3. Programa nustoja veikti, kai yra išvedami rezultatai terminale arba išvedama teksinė/informacinė eilutė, po vieno iš šių įvykių paspaudus, bet kurį klaviatūros klavišą programa užsidaro.

## Programos failai
### Programos failai
* `EgzaminoUzduotis.cpp` - pagrindinis programos failas.
* `funkcijos.h` - programos funkcijų header'ių failas.
* `meniu.h` - meniu struktūros failas.
* `funkcijos.cpp` - programos funkcijų failas.
### Programa testuoti failai
* `Siauliai.txt` - ~2300ž.
* `Trakai.txt` - ~900ž.
* `testas.txt` - 39ž. (jokių URL nuorodų).
* `spidermenas.txt` - ~9400ž. (angl.k.).
