#include "header.h"
using namespace std;

int main(){
    string tekstas;
    url_nuskaitymas();
    domain_url_nuskaitymas();

    //patikrinimas, ar txt file'as susikure ir nuskaito viska
    if(validacija("input.txt")){
        cout << "Validacija sėkminga!" << endl;
        auto result = zodziu_isrinkimas("file.txt");
    }
    
    // result.tekstas arba result.special_words
    auto tlds = loadTLDs("domain.txt");

}