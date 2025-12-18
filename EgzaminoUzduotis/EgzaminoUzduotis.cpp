#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <set>
#include <unordered_set>
#include <sstream>


using namespace std;

bool SimboliuFiltras(unsigned char simb) {
    return isalnum(simb);
}

char SimbolioTransformacija(char raide) {
    if (raide >= 'A' && raide <= 'Z') return (char)(raide - 'A' + 'a');
    return raide;
}

string ZodzioTransformacija(string link) {
    for (char& i : link) {
        if (i >= 'A' && i <= 'Z') i = (char)(i - 'A' + 'a');
    }
    return link;
}

bool URLList(const string& failas, unordered_set<string>& urls) {
    ifstream read(failas);
    if (!read) {
        cerr << "Failas " << failas << " neegzistuoja.\n";
        return 0;
    }
    urls.clear();
    string blokas;
    while (read >> blokas) {
        if (!blokas.empty() && blokas[0] == '#') {
            string eilute;
            getline(read, eilute);
            continue;
        }
        blokas = ZodzioTransformacija(blokas);
        urls.insert(blokas);
    }
    read.close();
    return !urls.empty();
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

bool URLTest(const string& link, const unordered_set<string>& urls, string& galutinis) {
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
    tld = ZodzioTransformacija(tld);
    if (urls.find(tld) == urls.end()) return false;
    return true;
}

void URLFind(const string& eil, const unordered_set<string>& urls, set<string>& links) {
    string link;
    stringstream ss(eil);
    while (ss >> link) {
        string galutinis;
        if (URLTest(link, urls, galutinis)) {
            links.insert(galutinis);
        }
    }
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
    unordered_set<string> urls;
    if (!URLList("URLList_12.18.txt", urls)) {
        cerr << "Failas " << failas << " neegzistuoja.\n";
        return 0;
    }
    map<string, int> zodziai;
    map<string, set<int>> cross;
    set<string> links;
    string eil, zodis;
    int vieta = 0;
    bool LtRaides = false;
    while (getline(df, eil)) {
        vieta++;
        URLFind(eil, urls, links);
        LtRaides = false;
        for (unsigned char charas : eil) {
            if (SimboliuFiltras(charas)) {
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
    ofstream rf("rezultatai.txt");
    rf << left << setw(20) << "Zodis" << setw(15) << "Kartai" << "Lokacija (eil. nr.)" << endl;
    for (const auto& obj : zodziai) {
        const string& zodis = obj.first;
        int kiekis = obj.second;
        if (kiekis > 1) {
            rf << left << setw(20) << zodis << setw(15) << kiekis;
            const set<int>& eilute = cross[zodis];
            for (int i : eilute) {
                rf << i << " ";
            }
            rf << endl;
        }
    }
    rf << "URLs:\n";
    if (links.empty()) {
        rf << "Tekste nera URL nuorodu.\n";
    }
    else {
        for (const auto& i : links) {
            rf << i << "\n";
        }
    }
    cout << "Rezultatai isvesti faile 'rezultatai.txt'.\n";
    df.close();
    rf.close();
    return 0;
}
