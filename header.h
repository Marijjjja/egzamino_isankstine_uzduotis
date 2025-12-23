#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <cctype>
#include <regex>
#include <unordered_set>

using namespace std;

struct Result {
    string tekstas;
    vector<string> special_words;
};

void url_nuskaitymas();
void domain_url_nuskaitymas();
bool validacija(const string& filename);
Result zodziu_isrinkimas(const string& filename);

unordered_set<string> loadTLDs(const std::string& filename);


void surasymas_i_txt();