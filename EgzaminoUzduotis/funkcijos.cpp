#include "funkcijos.h"
#include "meniu.h"

//Globalūs konteineriai:
map<string, int> zodziai;
map<string, set<int>> cross;
set<string> URLs;

MeniuAts meniu() {  // meniu funkcija
    string name;
    cout << string(50, '-') << endl;
    cout << "Iveskite norimos nagrineti svetaines/failo pavadinima: \n"; cin >> name;
    string failas = name + ".txt";  // nuskaitomo failo pavadinimas
    int ivestis, isvedimas;    // naudotojo meniu ir išvedimo pasirinkimai
    cout << string(50, '-') << endl;
    cout << "1 - rasti pasikartojancius zodzius ir ju lokacijas;\n";
    cout << "2 - rasti tekste esancias URL nuorodas;\n";
    cout << "3 - abu auksciau esantys variantai;\n";
    cout << "Iveskite savo pasirinkima: ";
    while (true) {
        ivestis = tikNr();  // teigiamo skaičiaus funkcijos iškvietimas
        if (ivestis > 3) cout << "Neteisinga ivestis. Bandykite vel: ";
        else break;
    }
    cout << string(50, '-') << endl;
    if (ivestis == 1) return MeniuAts{ failas, ivestis };   // grąžinami naudotojo pasirinkimai/įvestys
    else if (ivestis == 2) {
        cout << "Pasirinkite norima rezultatu isvedima:\n";
        cout << "1 - i faila\n";
        cout << "2 - i terminala\n";
        cout << "Iveskite savo pasirinkima: ";
        while (true) {
            isvedimas = tikNr();  // teigiamo skaičiaus funkcijos iškvietimas
            if (isvedimas > 2) cout << "Neteisinga ivestis. Bandykite vel: ";
            else break;
        }
        cout << string(50, '-') << endl;
        return MeniuAts{ failas, ivestis, isvedimas };  // grąžinami naudotojo pasirinkimai/įvestys
    }
    else return MeniuAts{ failas, ivestis };    // grąžinami naudotojo pasirinkimai/įvestys
}

int tikNr() {   // funkcija patikrai, kad įvestis yra skaičius didesnis už 0
    int ivestis;
    cin >> ivestis;
    while (true) {
        if (cin.fail() || ivestis <= 0) { // veiksmai jei įvestis žodinio tipo arba <= 0
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Neteisinga ivestis. Bandykite vel: "; cin >> ivestis;
        }
        else return ivestis;
    }
}

char SimbolioTransformacija(char simb) {    // funkcija transformuojanti simbolius į mažasias raides
    if (simb >= 'A' && simb <= 'Z') return (char)(simb - 'A' + 'a');
    return simb;
}

string TekstoTransformacija(string tekstas) {   // funkcija transformuojanti teksto bloką į mažasias raides
    for (char& i : tekstas) {
        if (i >= 'A' && i <= 'Z') i = (char)(i - 'A' + 'a');
    }
    return tekstas;
}

void TekstoNuskaitymas(string& failas) {    // funkcija nuskaitanti tekstą iš failo
    ifstream df(failas);
    if (!df) {
        cerr << "Failas " << failas << " neegzistuoja.\n";
        exit(0);
    }
    unordered_set<string> formatai;
    if (!URLList("URLList_12.18.txt", formatai)) {
        cerr << "Failas " << failas << " neegzistuoja.\n";
        exit(0);
    }
    zodziai.clear();
    cross.clear();
    URLs.clear();
    string eil, zodis;
    int vieta = 0;
    bool LtRaides = false;
    while (getline(df, eil)) {
        vieta++;
        URLFind(eil, formatai, URLs);
        LtRaides = false;
        for (unsigned char charas : eil) {
            if (isalnum(charas)) {
                char simb = (char)charas;
                simb = SimbolioTransformacija(simb);
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
    df.close();
}

void RezultatuIsvedimas(int& pasirinkimas, int& isvedimas) {
    if (pasirinkimas == 1) {
        ofstream rf("rezultatai.txt");
        //rf << "Zodis  |  " << "Kartai  |  " << "Lokacija (eil. nr.)" << endl;
        rf << left << setw(20) << "Zodis" << setw(15) << "Kartai" << "Lokacija (eil. nr.)" << endl;
        cout << string(50, '-') << endl;
        for (const auto& obj : zodziai) {
            const string& zodis = obj.first;
            int kiekis = obj.second;
            if (kiekis > 1) {
                //rf << zodis << "  |  " << kiekis << "  |  ";
                rf << left << setw(20) << zodis << setw(15) << kiekis;
                const set<int>& eilute = cross[zodis];
                for (int i : eilute) {
                    rf << i << " ";
                }
                rf << endl;
            }
        }
        cout << "Rezultatai isvesti faile 'rezultatai.txt'.\n";
        rf.close();
    }
    else if (pasirinkimas == 2) {
        if (isvedimas == 1) {
            ofstream rf("rezultatai.txt");
            rf << "URLs:\n";
            rf << string(50, '-') << endl;
            if (URLs.empty()) {
                cout << "Tekste nera URL nuorodu.\n";
                exit(0);
            }
            else {
                for (const auto& i : URLs) {
                    rf << i << "\n";
                }
            }
            cout << "Rezultatai isvesti faile 'rezultatai.txt'.\n";
            rf.close();
        }
        else {
            cout << "URLs:\n";
            cout << string(50, '-') << endl;
            if (URLs.empty()) {
                cout << "Tekste nera URL nuorodu.\n";
            }
            else {
                for (const auto& i : URLs) {
                    cout << i << "\n";
                }
            }
        }
    }
    else {
        ofstream rf("rezultatai.txt");
        //rf << "Zodis  |  " << "Kartai  |  " << "Lokacija (eil. nr.)" << endl;
        rf << left << setw(20) << "Zodis" << setw(15) << "Kartai" << "Lokacija (eil. nr.)" << endl;
        cout << string(50, '-') << endl;
        for (const auto& obj : zodziai) {
            const string& zodis = obj.first;
            int kiekis = obj.second;
            if (kiekis > 1) {
                //rf << zodis << "  |  " << kiekis << "  |  ";
                rf << left << setw(20) << zodis << setw(15) << kiekis;
                const set<int>& eilute = cross[zodis];
                for (int i : eilute) {
                    rf << i << " ";
                }
                rf << endl;
            }
        }
        rf << endl;
        rf << "URLs:\n";
        rf << string(50, '-') << endl;
        if (URLs.empty()) {
            rf << "Tekste nera URL nuorodu.\n";
            cout << "Tekste nera URL nuorodu. Kiti rezultatai isvesti faile 'rezultatai.txt'.\n";
            exit(0);
        }
        else {
            for (const auto& i : URLs) {
                rf << i << "\n";
            }
        }
        cout << "Rezultatai isvesti faile 'rezultatai.txt'.\n";
        rf.close();
    }
}

bool URLList(const string& failas, unordered_set<string>& formatai) {
    ifstream read(failas);
    if (!read) {
        cerr << "Failas " << failas << " neegzistuoja.\n";
        return 0;
    }
    formatai.clear();
    string blokas;
    while (read >> blokas) {
        if (!blokas.empty() && blokas[0] == '#') {
            string eilute;
            getline(read, eilute);
            continue;
        }
        blokas = TekstoTransformacija(blokas);
        formatai.insert(blokas);
    }
    read.close();
    return !formatai.empty();
}

string LinkTvarkymas(const string& link) {
    auto tvarkymas = [](unsigned char charas) {
        return charas == '.' || charas == ',' || charas == ';' || charas == ':' ||
            charas == '!' || charas == '?' || charas == ')' || charas == '(' ||
            charas == ']' || charas == '[' || charas == '"' || charas == '\'' ||
            charas == '<' || charas == '>';
        };
    int raide = 0;
    int galas = (int)link.size() - 1;
    while (raide <= galas && tvarkymas((unsigned char)link[raide])) raide++;
    while (galas >= raide && tvarkymas((unsigned char)link[galas])) galas--;
    if (raide > galas) return "";
    return link.substr(raide, galas - raide + 1);
}

bool URLTest(const string& link, const unordered_set<string>& links, string& galutinis) {
    galutinis = LinkTvarkymas(link);
    if (galutinis.empty()) return false;
    string url = galutinis;
    size_t start = 0;
    if (url.rfind("http://", 0) == 0) start = 7;
    else if (url.rfind("https://", 0) == 0) start = 8;
    else if (url.rfind("www.", 0) == 0) start = 0;
    else start = 0;
    size_t end = url.find_first_of("/?#:", start);
    string host = (end == string::npos) ? url.substr(start) : url.substr(start, end - start);
    if (url.find("://") == string::npos && host.find('@') != string::npos) return false;
    size_t taskas = host.rfind('.');
    if (taskas == string::npos || taskas == host.size() - 1) return false;
    string tld = host.substr(taskas + 1);
    tld = TekstoTransformacija(tld);
    if (links.find(tld) == links.end()) return false;
    return true;
}

void URLFind(const string& eil, const unordered_set<string>& links, set<string>& URLs) {
    string link;
    stringstream ss(eil);
    while (ss >> link) {
        string galutinis;
        if (URLTest(link, links, galutinis)) {
            URLs.insert(galutinis);
        }
    }
}

