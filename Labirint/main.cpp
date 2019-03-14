#include "labirint.h"

using namespace std;

int main() {

    try {
        Labirint l("primjer.txt");
        l.Ispis();
        cout<<endl;

        cout <<"Udaljenost: "<<l.udaljenost('A',9,'J',8) << endl;
    }
    catch(const char greska[]) {
        cout << greska;
    }
    return 0;
}
