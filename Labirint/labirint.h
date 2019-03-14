#ifndef LABIRINT_H
#define LABIRINT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <algorithm>
#include <iomanip>


using namespace std;

class Labirint {
private:

    vector< vector<int> > labirint;
    int kolone, redovi;

    bool Zid(char slovo, int kolona);
    int Red(char slovo);
    pair<int, int> pocetak(char slovo, int kolona);
    vector<int> SviProlazi(int red);
    bool PrazanRed(int red);

public:

    Labirint(string text);
    int udaljenost (char prviZid, int pozicijaPrvog, char drugiZid, int pozicijaDrugog);
    void Ispis();

};

#endif // LABIRINT_H

