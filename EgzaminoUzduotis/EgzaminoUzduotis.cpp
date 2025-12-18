#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <set>

//#include "funkcijos.cpp"

using namespace std;

bool SimboliuFiltras(unsigned char simb) {
    return isalnum(simb);
}

char ZodziuTransformacija(char raide) {
    if (raide >= 'A' && raide <= 'Z') return (char)(raide - 'A' + 'a');
    return raide;
}

int main() {
    string name;
    cout << "Iveskite norimos nagrineti svetaines pavadinima: \n"; cin >> name;
    string failas = name + ".txt";
    ifstream df(failas);
    if (!df) {
        cerr << "Failas " << failas << " neegzistuoja.\n";
        return 0;
    }
    map<string, int> zodziai;
    map<string, set<int>> cross;
    string eil, zodis;
    int vieta = 0;
    bool LtRaides = false;
    while (getline(df, eil)) {
        vieta++;
        for (unsigned char charas : eil) {
            if (SimboliuFiltras(charas)) {
                char simb = (char)charas;
                if (charas < 128) simb = ZodziuTransformacija(simb);
                zodis.push_back(simb);
                LtRaides = false;
            }
            else if (charas == 0xC4 || charas == 0xC5) {
                zodis.push_back((char)charas);
                LtRaides = true;
            }
            else if (LtRaides && (charas >= 0x80 && charas <= 0xBF)) {
                zodis.push_back((char)charas);
                LtRaides = false;
            }
            else {
                LtRaides = false;
                if (!zodis.empty()) {
                    zodziai[zodis]++;
                    cross[zodis].insert(vieta);
                    zodis.clear();
                }
            }
        }
        if (!zodis.empty()) {
            zodziai[zodis]++;
            cross[zodis].insert(vieta);
            zodis.clear();
        }
    }
    ofstream rf("rezultatai.txt");
    rf << left << setw(20) << "Zodis" << setw(15) << "Kartai" << "Lokacija (eil. nr.)" << endl;
    for (const auto& obj : zodziai) {
        const string& zodis = obj.first;
        int kiekis = obj.second;
        if (kiekis > 1) {
            rf << left << setw(20) << zodis << setw(15) << kiekis;
            const set<int>& eilute = cross[zodis];
            for (int i : eilute) {
                rf << i << ", ";
            }
            rf << endl;
        }
    }
    cout << "Rezultatai isvesti faile 'rezultatai.txt'.\n";
    return 0;
}
