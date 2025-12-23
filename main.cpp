#include "header.h"
using namespace std;

int main(){
    Result result;
    string tekstas;
    url_nuskaitymas();
    domain_url_nuskaitymas();

    //patikrinimas, ar txt file'as susikure ir nuskaito viska
    if(validacija("input.txt")){
        cout << "Validacija sėkminga!" << endl;
        result = zodziu_isrinkimas("input.txt");
    }

    auto tlds = loadTLDs("domain.txt");
    write_report(result.tekstas, result.special_words, "report.txt");

}