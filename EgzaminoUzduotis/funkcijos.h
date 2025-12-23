#pragma once

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <set>
#include <unordered_set>
#include <sstream>
#include <utility>
#include <limits>

#include "meniu.h"

using namespace std;

MeniuAts meniu();	// meniu funkcija
int tikNr();	// funkcija patikrai, kad įvestis yra skaičius didesnis už 0
char SimbolioTransformacija(char raide);	// funkcija transformuojanti simbolius į mažasias raides
string TekstoTransformacija(string link);	// funkcija transformuojanti teksto bloką į mažasias raides
void TekstoNuskaitymas(string& name, int pasirinkimas);	// funkcija nuskaitanti tekstą iš failo
void RezultatuIsvedimas(int& pasirinkimas, int& isvedimas);	// funkcija išvedanti rezultatus į failą/terminalą
bool URLList(const string& failas, unordered_set<string>& urls);	// funkcija sudaranti galimų URL formatų sąrašą
string LinkTvarkymas(const string& link);	// funkcija "sutvarkanti" link'us
bool URLTest(const string& link, const unordered_set<string>& urls, string& galutinis);	// fukcija patikrai, ar teksto blokas yra URL nuoroda
void URLFind(const string& eil, const unordered_set<string>& urls, set<string>& links);	// funkcija randanti URL nuorodas tekste

// Extern globalūs konteineriai:
extern map<string, int> zodziai;
extern map<string, set<int>> cross;
extern set<string> URLs;