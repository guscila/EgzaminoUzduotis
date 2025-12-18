#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>

//#include "funkcijos.cpp"

using namespace std;

bool SimbolioTestas(unsigned char simb) {
    return isalnum(simb) || simb >= 128;
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
    string eil, zodis;
    while (getline(df, eil)) {
        for (unsigned char charas : eil) {
            if (SimbolioTestas(charas)) {
                char simb = (char)charas;
                if (charas < 128) simb = ZodziuTransformacija(simb);
                zodis.push_back(simb);
            }
            else {
                if (!zodis.empty()) {
                    zodziai[zodis]++;
                    zodis.clear();
                }
            }
        }
        if (!zodis.empty()) {
            zodziai[zodis]++;
            zodis.clear();
        }
    }
    ofstream rf("rezultatai.txt");
    rf << left << setw(15) << "Zodis" << setw(15) << "Kartai" << endl;
    for (const auto& obj : zodziai) {
        const string& zodis = obj.first;
        int kiekis = obj.second;
        if (kiekis > 1) {
            rf << zodis << " : " << kiekis << "\n";
        }
    }
    cout << "Rezultatai isvesti faile 'rezultatai.txt'.\n";
    return 0;
}
