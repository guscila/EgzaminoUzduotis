#include "funkcijos.h"
#include "meniu.h"

//Globalūs konteineriai:
map<string, int> zodziai;   // tekste esančių žodžių ir kiekio konteineris
map<string, set<int>> cross;    // tekste esančių žodžių lokacijos konteineris
set<string> URLs;   // tekste esančių URL nuorodų konteineris

MeniuAts meniu() {  // meniu funkcija
    string name;
    cout << string(50, '-') << endl;
    cout << "Iveskite norimos nagrineti svetaines/failo pavadinima: \n"; cin >> name;
    string failas = "failai\\" + name + ".txt";  // nuskaitomo failo pavadinimas
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

void TekstoNuskaitymas(string& failas, int pasirinkimas) {    // funkcija nuskaitanti tekstą iš failo
    ifstream df(failas);
    if (!df) {
        cerr << "Failas " << failas << " neegzistuoja.\n";
        exit(0);
    }
    zodziai.clear();    // išvalomas konteineris
    cross.clear();      // išvalomas konteineris
    URLs.clear();       // išvalomas konteineris
    unordered_set<string> formatai; // galimų URL formatų konteineris
    if (pasirinkimas == 1) {    // teskte randami visi žodžiai ir kiek kartų jie pasirodė
        string eil, zodis;
        int vieta = 0;
        bool LtRaides = false;
        while (getline(df, eil)) {  // tekstas skaitomas po eilutę
            vieta++;    // eilutės numerio skaičiavimas
            LtRaides = false;
            for (unsigned char charas : eil) {  // eilutė skaitoma po simbolį
                if (charas < 128 && isalnum(charas)) {
                    char simb = (char)charas;
                    simb = SimbolioTransformacija(simb); // simbolis sumažinamas/tranformuojamas
                    zodis.push_back(simb);  // simbolis pridedamas į žodį
                    LtRaides = false;
                }
                else if (charas == 0xC4 || charas == 0xC5) {    // veiksmai su lietuviškos raidės pirmu baitu
                    zodis.push_back((char)charas);
                    LtRaides = true;
                }
                else if (LtRaides && (charas >= 0x80 && charas <= 0xBF)) {  // veiksmai su lietuviškos raidės antru baitu
                    zodis.push_back((char)charas);
                    LtRaides = false;
                }
                else {  // veiksmai jei simbolis yra kas kito
                    LtRaides = false;
                    if (!zodis.empty()) {
                        zodziai[zodis]++;   // žodis pridedamas į konteinerį
                        cross[zodis].insert(vieta); // žodžiui priskiriama jo lokacija
                        zodis.clear();
                    }
                }
            }
            if (!zodis.empty()) {
                zodziai[zodis]++;   // žodžio kiekio papildymas
                cross[zodis].insert(vieta); // žodžio lokacijos papildymas
                zodis.clear();
            }
        }
        df.close();
    }
    else if (pasirinkimas == 2) {   // tekste randamos visos URL nuorodos
        string sarasas = "failai\\URLList_12.18.txt";
        if (!URLList(sarasas, formatai)) {
            cerr << "Formatu failas nerastas.\n";
            exit(0);
        }
        string eil;
        bool LtRaides = false;
        while (getline(df, eil)) {  // tekstas skaitomas po eilutę
            URLFind(eil, formatai, URLs);   // funkcijos ieškančios URL nuorodų iškvietimas
        }
        df.close();
    }
    else {
        string sarasas = "failai\\URLList_12.18.txt";
        if (!URLList(sarasas, formatai)) {
            cerr << "Formatu failas nerastas.\n";
            exit(0);
        }
        string eil, zodis;
        int vieta = 0;
        bool LtRaides = false;
        while (getline(df, eil)) {  // tekstas skaitomas po eilutę
            vieta++;    // eilutės numerio skaičiavimas
            URLFind(eil, formatai, URLs);   // funkcijos ieškančios URL nuorodų iškvietimas
            LtRaides = false;
            for (unsigned char charas : eil) {  // eilutė skaitoma po simbolį
                if (charas < 128 && isalnum(charas)) {
                    char simb = (char)charas;
                    simb = SimbolioTransformacija(simb); // simbolis sumažinamas/tranformuojamas
                    zodis.push_back(simb);  // simbolis pridedamas į žodį
                    LtRaides = false;
                }
                else if (charas == 0xC4 || charas == 0xC5) {    // veiksmai su lietuviškos raidės pirmu baitu
                    zodis.push_back((char)charas);
                    LtRaides = true;
                }
                else if (LtRaides && (charas >= 0x80 && charas <= 0xBF)) {  // veiksmai su lietuviškos raidės antru baitu
                    zodis.push_back((char)charas);
                    LtRaides = false;
                }
                else {  // veiksmai jei simbolis yra kas kito
                    LtRaides = false;
                    if (!zodis.empty()) {
                        zodziai[zodis]++;   // žodis pridedamas į konteinerį
                        cross[zodis].insert(vieta); // žodžiui priskiriama jo lokacija
                        zodis.clear();
                    }
                }
            }
            if (!zodis.empty()) {
                zodziai[zodis]++;   // žodžio kiekio papildymas
                cross[zodis].insert(vieta); // žodžio lokacijos papildymas
                zodis.clear();
            }
        }
        df.close();
    }
}

void RezultatuIsvedimas(int& pasirinkimas, int& isvedimas) {
    if (pasirinkimas == 1) {    // žodžių tekste rezultatų išvedimas
        ofstream rf("failai\\rezultatai.txt");
        //rf << "Zodis  |  " << "Kartai  |  " << "Lokacija (eil. nr.)" << endl;
        rf << left << setw(20) << "Zodis" << setw(15) << "Kartai" << "Lokacija (eil. nr.)" << endl;
        cout << string(50, '-') << endl;
        for (const auto& obj : zodziai) {
            const string& zodis = obj.first;
            int kiekis = obj.second;
            if (kiekis > 1) {   // išvedami tik žodžiai pasirodę >=2 kartus
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
    else if (pasirinkimas == 2) {   // URL nuorodų tekste rezultatų išvedimas
        if (isvedimas == 1) {   // išvedimas į failą
            ofstream rf("failai\\rezultatai.txt");
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
        else {  // išvedimas į terminalą
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
    else {  // teksto žodžių ir URL nuorodų rezultatų išvedimas
        ofstream rf("failai\\rezultatai.txt");
        //rf << "Zodis  |  " << "Kartai  |  " << "Lokacija (eil. nr.)" << endl;
        rf << left << setw(20) << "Zodis" << setw(15) << "Kartai" << "Lokacija (eil. nr.)" << endl;
        cout << string(50, '-') << endl;
        for (const auto& obj : zodziai) {
            const string& zodis = obj.first;
            int kiekis = obj.second;
            if (kiekis > 1) {   // išvedami tik žodžiai pasirodę >=2 kartus
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
    formatai.clear();   // išvalomas konteineris
    string blokas;
    while (read >> blokas) {    // formatas nuskaitomas
        if (!blokas.empty() && blokas[0] == '#') {  // praleidžiami komentarai ar tarpai
            string eilute;
            getline(read, eilute);
            continue;
        }
        blokas = TekstoTransformacija(blokas);  // blokas sumažinamas/transformuojamas
        formatai.insert(blokas);    // formatas pridedamas į konteinerį
    }
    read.close();
    return !formatai.empty();
}

string LinkTvarkymas(const string& link) {
    auto tvarkymas = [](unsigned char charas) { // pašalinami simboliai
        if (charas >= 128) return true;
        return charas == '.' || charas == ',' || charas == ';' || charas == ':' ||
            charas == '!' || charas == '?' || charas == ')' || charas == '(' ||
            charas == ']' || charas == '[' || charas == '"' || charas == '\'' ||
            charas == '<' || charas == '>';
        };
    int raide = 0;
    int galas = (int)link.size() - 1;
    while (raide <= galas && tvarkymas((unsigned char)link[raide])) raide++;    // tvarkymas iš priekio
    while (galas >= raide && tvarkymas((unsigned char)link[galas])) galas--;    // tvarkymas iš galo
    if (raide > galas) return "";
    return link.substr(raide, galas - raide + 1); // grąžinama apkirpta dalis
}

bool URLTest(const string& link, const unordered_set<string>& links, string& galutinis) {
    galutinis = LinkTvarkymas(link);    // aptvarkomas/apkarpomas URL
    if (galutinis.empty()) return false;    // ne URL
    string url = galutinis;
    size_t start = 0;
    if (url.rfind("http://", 0) == 0) start = 7;
    else if (url.rfind("https://", 0) == 0) start = 8;
    else if (url.rfind("www.", 0) == 0) start = 0;
    else start = 0;
    size_t end = url.find_first_of("/?#:", start);
    string host = (end == string::npos) ? url.substr(start) : url.substr(start, end - start);   // host nustatymas
    if (url.find("://") == string::npos && host.find('@') != string::npos) return false;    // el. pašto adreso atmetimas
    size_t taskas = host.rfind('.');    // taško skirties suradimas
    if (taskas == string::npos || taskas == host.size() - 1) return false;
    string tld = host.substr(taskas + 1);
    tld = TekstoTransformacija(tld);
    if (links.find(tld) == links.end()) return false;   // tikrina ar URL domenas yra formatų sąraše
    return true;
}

void URLFind(const string& eil, const unordered_set<string>& links, set<string>& URLs) {
    string link;
    stringstream ss(eil);
    while (ss >> link) {    // suskaldo tekstą į tokenus
        string galutinis;
        if (URLTest(link, links, galutinis)) {  // tikrina ar tokenas yra URL nuoroda
            URLs.insert(galutinis); // URL nuoroda pridedama į konteinerį
        }
    }
}