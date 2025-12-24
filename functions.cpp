#include <algorithm>
#include <cctype>
#include "header.h"
using namespace std;

bool has_any_valid_domain(const string& text) {
    regex domain_regex(R"((https?:\/\/)?(www\.)?([a-zA-Z0-9-]+\.)+[a-zA-Z]{2,})");

    auto begin = sregex_iterator(text.begin(), text.end(), domain_regex);
    auto end   = sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        string match = it->str();
        if (has_valid_domain(match))
            return true;
    }
    return false;
}

bool has_valid_domain(const string& link) {
    size_t dot = link.find_last_of('.');
    if (dot == string::npos || dot + 1 >= link.size())
        return false;

    string tld = link.substr(dot + 1);

    while (!tld.empty() && !isalpha(static_cast<unsigned char>(tld.back())))
        tld.pop_back();

    if (tld.empty())
        return false;

    for (char& c : tld)
        c = tolower(static_cast<unsigned char>(c));

    ifstream file("domain.txt");
    if (!file.is_open())
        return false;

    string allowed;
    while (file >> allowed) {
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

    unordered_set<string> tlds;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        while (!line.empty() && isspace((unsigned char)line.back()))
            line.pop_back();

        while (!line.empty() && isspace((unsigned char)line.front()))
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
    cout<< "Nuskaityta naujausia domain info" << endl;
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

            if (isalpha(static_cast<unsigned char>(c))) {
                word += tolower(static_cast<unsigned char>(c));
            } else {
                if (!word.empty()) {
                    zodziai[word].insert(line_nr);
                    word.clear();
                }
            }

            if (isalpha(static_cast<unsigned char>(c)) || c == '.' || c == '/' || c == ':' || c == '_') {

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

                    if (is_link(special) && has_any_valid_domain(special))
                        special_words.insert(special);
                }

                special.clear();
                has_special = false;
                has_letter = false;
                has_dot = false;
                dot_followed_by_letter = false;
            }
        }

        if (!word.empty())
            zodziai[word].insert(line_nr);

        if (!special.empty()){

            while (!special.empty() && special.back() == ':')
                special.pop_back();

            if (is_link(special) && has_any_valid_domain(special))
                special_words.insert(special);
        }
    }

    // cout << "\nSpecial words:\n";
    // for (const auto& s : special_words)
    //     cout << s << '\n';

    return {tekstas, special_words, zodziai};
}


void write_report(const Result& res, const string& output_file){

    ofstream out(output_file);
    if (!out.is_open()) {
        cerr << "Failed to open file: " << output_file << '\n';
        return;
    }

    // out << "Žodžių dažniai (tik > 1):\n\n";

    // for (const auto& [word, lines] : res.zodziai) {
    //     if (lines.size() > 1)
    //         out << word << " : " << lines.size() << '\n';
    // }

    out << "\nCross-reference (eilutės):\n\n";

    for (const auto& [word, lines] : res.zodziai) {
        if (lines.size() > 1) {
            out << word << " (" << lines.size() << ") : ";
            for (int ln : lines) out << ln << " ";
            out << '\n';
        }
    }

    out << "\n --- --- --- --- --- --- Linkai --- --- --- --- --- --- --- --- \n\n";
    for (const auto& s : res.special_words)
        out << s << '\n';
}


void surasymas_i_txt(){
    ofstream rezultatas("rezultatas.txt");
}

