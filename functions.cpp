#include "header.h"
using namespace std;

void url_nuskaitymas(){
    string url;
    cout << "url: "<< endl;
    cin >> url;

    string cmd = "./get_text.sh " + url;
    system(cmd.c_str());

    ifstream in("input.txt");
}

bool validacija(const string& filename) {
    ifstream file(filename);

    if (!file) {
        cerr << "Failo nepavyko atidaryti\n";
        return false;
    }

    if (file.peek() == ifstream::traits_type::eof()) {
        cerr << "Failas tuščias\n";
        return false;
    }
    return true;
}

void zodziu_isrinkimas(const string& filename){
    ifstream file(filename);
    map<string, set<int> > zodziai;
    string line, word;
    int line_nr = 0;

    while (getline(file, line)) {
        line_nr++;
        word.clear();

        for (char c : line) {
            if (isalpha(c)) {
                word += tolower(c);
            } else {
                if (!word.empty()) {
                    zodziai[word].insert(line_nr);
                    word.clear();
                }
            }
        }

        if (!word.empty()) {
            zodziai[word].insert(line_nr);
        }
    }

    // output testui
    for (const auto& [zodis, eilutes] : zodziai) {
        if (eilutes.size() > 1) {
            cout << zodis << ": ";
            for (int e : eilutes) cout << e << " ";
            cout << '\n';
        }
    }

    while (getline(file, line)) {
        cout << line << '\n';
    }
}





void surasymas_i_txt(){
    ofstream rezultatas("rezultatas.txt");
}