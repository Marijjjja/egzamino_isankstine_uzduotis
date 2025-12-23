#include "header.h"
using namespace std;

int main(){
    url_nuskaitymas();

    //patikrinimas, ar txt file'as susikure ir nuskaito viska
    if(validacija("input.txt")){
        cout << "Validacija sėkminga!" << endl;
        zodziu_isrinkimas("input.txt");
    }
}