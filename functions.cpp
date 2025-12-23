#include <algorithm>
#include <cctype>

#include "header.h"
using namespace std;

static inline string toUpper(string s){
    for (auto& ch : s) ch = (char)toupper((unsigned char)ch);
    return s;
}

unordered_set<string> loadTLDs(const string& filename){
    ifstream file(filename);
    if (!file) {
        cerr << "Nepavyko atidaryti TLD failo: " << filename << '\n';
        return {};
    }

    unordered_set<std::string> tlds;
    string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        while (!line.empty() && std::isspace((unsigned char)line.back()))
            line.pop_back();

        while (!line.empty() && std::isspace((unsigned char)line.front()))
            line.erase(line.begin());

        for (char& c : line)
            c = (char)toupper((unsigned char)c);

        tlds.insert(line);
    }

    return tlds;
}


void url_nuskaitymas(){
    string url;
    cout << "url: "<< endl;
    cin >> url;

    string cmd = "./get_text.sh " + url;
    system(cmd.c_str());

    ifstream in("input.txt");
}

void domain_url_nuskaitymas(){
    string url = "https://data.iana.org/TLD/tlds-alpha-by-domain.txt";

    string cmd = "./get_domain.sh " + url;
    system(cmd.c_str());

    ifstream in("domain.txt");
    cout<< "nuskaityta naujausia domain info" << endl;
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

Result zodziu_isrinkimas(const string& filename) {
    ifstream file(filename);

    map<string, set<int>> zodziai;
    vector<string> special_words;

    string line, word, special, tekstas;
    int line_nr = 0;

    while (getline(file, line)) {
        tekstas += line + '\n';
        line_nr++;

        word.clear();
        special.clear();

        bool has_special = false;
        bool has_letter = false;
        bool has_dot = false;
        bool dot_followed_by_letter = false;

        for (char c : line) {

            // normal words
            if (isalpha(c)) {
                word += tolower(c);
            } else {
                if (!word.empty()) {
                    zodziai[word].insert(line_nr);
                    word.clear();
                }
            }

            // special token logic
            if (isalpha(c) || c == '.' || c == '/' || c == ':') {

                if (isalpha(c)) {
                    has_letter = true;
                    if (has_dot) dot_followed_by_letter = true;
                }

                if (c == '.' || c == '/' || c == ':') has_special = true;
                if (c == '.') has_dot = true;

                special += tolower(c);

            } else {
                if (!special.empty() &&
                    has_special &&
                    has_letter &&
                    (!has_dot || dot_followed_by_letter)) {

                    // remove trailing ':'
                    while (!special.empty() && special.back() == ':')
                        special.pop_back();

                    if (!special.empty())
                        special_words.push_back(special);
                }

                special.clear();
                has_special = false;
                has_letter = false;
                has_dot = false;
                dot_followed_by_letter = false;
            }
        }

        // flush end-of-line
        if (!word.empty())
            zodziai[word].insert(line_nr);

        if (!special.empty() &&
            has_special &&
            has_letter &&
            (!has_dot || dot_followed_by_letter)) {

            while (!special.empty() && special.back() == ':')
                special.pop_back();

            if (!special.empty())
                special_words.push_back(special);
        }
    }

    for (const auto& [zodis, eilutes] : zodziai) {
        if (eilutes.size() > 1) {
            cout << zodis << ": ";
            for (int e : eilutes) cout << e << " ";
            cout << '\n';
        }
    }

    cout << "\nSpecial words:\n";
    for (const auto& s : special_words)
        cout << s << '\n';

    return {tekstas, special_words};;
}



void surasymas_i_txt(){
    ofstream rezultatas("rezultatas.txt");
}