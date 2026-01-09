#include "header.h"
using namespace std;

int main(){
    Result result;

    // string pattern;
    // cout << "iveskite patterna: ";
    // cin >> pattern;

    //url_nuskaitymas();
    domain_url_nuskaitymas();

    if(validacija("input.txt")){
        cout << "Validacija sėkminga!" << endl;
        result = zodziu_isrinkimas("input.txt");
    }

    auto tlds = loadTLDs("domain.txt");
    // void write_report(const Result& res,
    //               const string& output_file,
    //               const string& pattern);

    write_report(result, "report.txt");
    // write_report(result.tekstas, result.special_words, "report.txt");

}