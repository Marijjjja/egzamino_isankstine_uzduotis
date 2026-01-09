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
    map<string, vector<int>> zodziai;
};
bool starts_with(const string& s, const string& prefix);
bool is_link(const string& s);

bool has_any_valid_domain(const string& text);

void url_nuskaitymas();
void domain_url_nuskaitymas();
bool validacija(const string& filename);
Result zodziu_isrinkimas(const string& filename);

unordered_set<string> loadTLDs(const std::string& filename);

void write_report(const Result& res,
                  const string& output_file);

// void write_report(const Result& res,
//                   const string& output_file,
//                   const string& pattern);

bool has_valid_domain(const string& link);
string normalize_lt(const string& s);