#include <algorithm>
#include <cctype>

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

static string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return toupper(c); });
    return s;
}

static string extractTLD(const string& url) {
    size_t dot = url.find_last_of('.');
    if (dot == string::npos) return "";
    return url.substr(dot + 1);
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

unordered_set<string> loadTLDs(const string& filename) {
    unordered_set<string> tlds;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        tlds.insert(line);
    }
    return tlds;
}

void zodziu_isrinkimas(const string& filename){
    ifstream file(filename);
    map<string, set<int> > zodziai;
    string line, word, tekstas;
    int line_nr = 0;

    while (getline(file, line)) {
        tekstas += line + '\n';
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
    }

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

    unordered_set<string> tlds = loadTLDs("tlds.txt");
    vector<string> links = extractValidLinks(tekstas, tlds);

    cout << "\nRasti URL'ai:\n";
    for (const auto& link : links) {
        cout << link << '\n';
    }
}

vector<string> extractValidLinks(
    const string& text,
    const unordered_set<string>& validTLDs
) {
    vector<string> results;

    regex urlRegex(R"((https?:\/\/)?(www\.)?[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)+)");

    auto begin = sregex_iterator(text.begin(), text.end(), urlRegex);
    auto end   = sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        string url = it->str();

        while (!url.empty() && ispunct(url.back()) && url.back() != '/') {
            url.pop_back();
        }

        string tld = toUpper(extractTLD(url));
        if (validTLDs.count(tld)) {
            results.push_back(url);
        }
    }

    return results;
}

void surasymas_i_txt(){
    ofstream rezultatas("rezultatas.txt");
}