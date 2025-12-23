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
    set<string> special_words;
};
bool starts_with(const string& s, const string& prefix);
bool is_link(const string& s);

void url_nuskaitymas();
void domain_url_nuskaitymas();
bool validacija(const string& filename);
Result zodziu_isrinkimas(const string& filename);

unordered_set<string> loadTLDs(const std::string& filename);

void write_report(const string& tekstas,
                  const set<string>& special_words,
                  const string& output_file);
bool has_valid_domain(const string& link);

void surasymas_i_txt();