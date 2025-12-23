#include <algorithm>
#include <cctype>

#include "header.h"
using namespace std;

static inline string toUpper(string s){
    for (auto& ch : s) ch = (char)toupper((unsigned char)ch);
    return s;
}

bool has_valid_domain(const string& link) {
    // 1. find last dot
    size_t dot = link.find_last_of('.');
    if (dot == string::npos || dot + 1 >= link.size())
        return false;

    // 2. extract candidate TLD
    string tld = link.substr(dot + 1);

    // strip trailing non-letters
    while (!tld.empty() && !isalpha(static_cast<unsigned char>(tld.back())))
        tld.pop_back();

    if (tld.empty())
        return false;

    // normalize to lowercase
    for (char& c : tld)
        c = tolower(static_cast<unsigned char>(c));

    // 3. open domain.txt and check
    ifstream file("domain.txt");
    if (!file.is_open())
        return false;

    string allowed;
    while (file >> allowed) {
        // normalize domain.txt entry
        for (char& c : allowed)
            c = tolower(static_cast<unsigned char>(c));

        if (allowed == tld)
            return true;
    }

    return false;
}


bool starts_with(const string& s, const string& prefix) {
    return s.size() >= prefix.size() &&
           s.compare(0, prefix.size(), prefix) == 0;
}

bool is_link(const string& s) {
    if (s.empty()) return false;

    if (starts_with(s, "http://") || starts_with(s, "https://"))
        return true;

    if (starts_with(s, "www."))
        return true;

    if (s.find('.') != string::npos &&
        s.find('/') == string::npos &&
        s.find(':') == string::npos &&
        s.front() != '.' &&
        s.back() != '.')
        return true;

    return false;
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
    set<string> special_words;

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
            if (isalpha(static_cast<unsigned char>(c))) {
                word += tolower(static_cast<unsigned char>(c));
            } else {
                if (!word.empty()) {
                    zodziai[word].insert(line_nr);
                    word.clear();
                }
            }

            // special token logic
            if (isalpha(static_cast<unsigned char>(c)) || c == '.' || c == '/' || c == ':') {

                if (isalpha(static_cast<unsigned char>(c))) {
                    has_letter = true;
                    if (has_dot) dot_followed_by_letter = true;
                }

                if (c == '.' || c == '/' || c == ':') has_special = true;
                if (c == '.') has_dot = true;

                special += tolower(static_cast<unsigned char>(c));

            } else {
                if (!special.empty()) {
                    while (!special.empty() && special.back() == ':')
                        special.pop_back();

                    if (is_link(special) && has_valid_domain(special))
                        special_words.insert(special);
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
                special_words.insert(special);
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

    return {tekstas, special_words};
}

void write_report(const string& tekstas,
                  const set<string>& special_words,
                  const string& output_file){

    ofstream out(output_file);
    if (!out.is_open()) {
        cerr << "Failed to open file: " << output_file << '\n';
        return;
    }

    map<string, int> word_count;

    istringstream iss(tekstas);
    string line, word;

    while (getline(iss, line)) {
        word.clear();

        for (char c : line) {
            if (isalpha(static_cast<unsigned char>(c))) {
                word += tolower(static_cast<unsigned char>(c));
            } else {
                if (!word.empty()) {
                    ++word_count[word];
                    word.clear();
                }
            }
        }
        if (!word.empty())
            ++word_count[word];
    }

    out << "Žodžių dažniai:\n\n";
    for (const auto& [w, count] : word_count) {
        out << w << " : " << count << '\n';
    }

    out << "\n--- --- Linkai --- ---:\n\n";
    for (const auto& s : special_words) {
        out << s << '\n';
    }
}


void surasymas_i_txt(){
    ofstream rezultatas("rezultatas.txt");
}

