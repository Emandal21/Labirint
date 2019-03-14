#include "labirint.h"
#ifndef LABIRINT_CPP
#define LABIRINT_CPP

Labirint::Labirint(string text) {

    ifstream fajl(text);

    int brRedova;
    int brKolona;

    fajl >> brRedova;
    fajl.get();
    fajl >> brKolona;
    fajl.get();

    if(brKolona < 0)
        throw "Neispravan broj kolona";
    if(brRedova%2 == 1)
        throw "Unesite paran broj za broj redova";
    else if(brRedova < 0 || brRedova > 26)
        throw "Za broj redova unesite broj izmedju 0 i 26";

    kolone = brKolona;
    redovi = brRedova ;

    if(brRedova == 0) return;

    vector<int> Zidovi, Prazni;
    for(int i = 0; i < kolone; i++) {
        Zidovi.push_back(i + 1);
        Prazni.push_back(0);
    }

    labirint.push_back(Zidovi);
    labirint.push_back(Prazni);
    labirint.push_back(Prazni);

    for(int i = 0; i < (redovi - 2)/2; i++) {

        vector<int> trenutniRed(kolone);
        for(int i = 1; i <= kolone; i++) trenutniRed[i-1] = i;


        while(fajl.peek() != '\n') {
            int praznoMjesto = 0;
            int pom;
            if(fajl.eof())
                break;

            if( char(fajl.peek()==',') ) fajl.get();

            if(char(fajl.peek()) < '0' || char(fajl.peek()) > '9') throw "Pogresan unos";
            int br=0;
            while(char(fajl.peek())>='0' && char(fajl.peek())<='9') {
                fajl >> pom;
                praznoMjesto = praznoMjesto * pow(10, br) + pom;
                br++;
                if(praznoMjesto < 0 || praznoMjesto > kolone)
                    throw "Pogresan unos";
            }
            trenutniRed[praznoMjesto-1] = 0;
        }

        fajl.get();

        for(int i = 0; i < 2; i++) labirint.push_back(trenutniRed);

        for(int i = 0; i < 2; i++) labirint.push_back(Prazni);


        if(fajl.eof() && i != (redovi-2)/2 - 1)
            throw "Nemate dovoljno redova";
    }

    if(!fajl.eof())
        throw "Imate previse redova";

    fajl.close();

    labirint.push_back(Zidovi);

}

bool Labirint::Zid(char slovo, int kolona) {
    if(Red(slovo) == -1)
        throw "Pogresno unijeto slovo";

    int i = Red(slovo);
    int j = kolona - 1;

    if(labirint[i][j] != 0)
        return true;
    return false;
}

int Labirint::Red(char slovo) {
    if(slovo < 'A' || slovo > 'Z')
        return -1;
    else if(int(slovo) % 2 == 1)
        return (slovo - 'A')*2;
    else
        return (slovo - 'B')*2+3;
}

pair<int, int> Labirint::pocetak(char slovo, int kolona) {
    if(Red(slovo) == -1)
        throw "Pogresno unijeto slovo";

    if(!Zid(slovo, kolona))
        throw "Proslijedjeno mjesto nije zid";

    if(int(slovo) % 2 == 0) return make_pair(Red(slovo) - 1, kolona - 1);
    else return make_pair(Red(slovo) + 1, kolona - 1);

}


vector<int> Labirint::SviProlazi(int red){
    if(red < 0 || red >= redovi * 2)
        throw "Ne postoji proslijedjeni red";

    vector<int> rezultat;

    for(int i = 0; i < kolone; i++)
        if(labirint[red][i] == 0) rezultat.push_back(i);

    return rezultat;

}

bool Labirint::PrazanRed(int red){
    if(red < 0 || red >= redovi * 2)
        throw "Ne postoji proslijedjeni red";

    for(int i = 0; i < kolone; i++)
        if(labirint[red][i] != 0) return false;

    return true;
}

int Labirint::udaljenost(char prviZid, int pozicijaPrvog, char drugiZid, int pozicijaDrugog) {
    pair<int, int> start, cilj;
    start = pocetak(prviZid, pozicijaPrvog);
    cilj = pocetak(drugiZid, pozicijaDrugog);

    vector< vector<int> > udaljenosti(2 * redovi);
    for(int i = 0; i < 2 * redovi; i++) udaljenosti[i].resize(kolone, 0);


    udaljenosti[start.first][start.second] = 1;

    int trenutniRed = start.first + 1;
    int trenutnaKolona = start.second;

    while(true){
        if(trenutniRed == cilj.first && trenutnaKolona == cilj.second) break;

        bool trenutniPrazan = PrazanRed(trenutniRed);
        bool prethodniPrazan = PrazanRed(trenutniRed - 1);

        if(trenutniPrazan && !prethodniPrazan){
                udaljenosti[trenutniRed][trenutnaKolona] = udaljenosti[trenutniRed - 1][trenutnaKolona] + 1;
                trenutniRed++;
        }

        else if(trenutniRed != cilj.first && trenutniPrazan && prethodniPrazan){
            udaljenosti[trenutniRed][trenutnaKolona] = udaljenosti[trenutniRed - 1][trenutnaKolona] + 1;
            vector<int> prolazi = SviProlazi(trenutniRed + 1);

            if(prolazi.size() == 0)
                throw "Greska, red nema prolaza";

            int najbliziProlaz = abs(trenutnaKolona - prolazi[0]);
            int kolona = prolazi[0];

            for(int i = 1; i < prolazi.size(); i++){
                if(najbliziProlaz > abs(trenutnaKolona - prolazi[i])){
                    najbliziProlaz = abs(trenutnaKolona - prolazi[i]);
                    kolona = prolazi[i];
                }
                else if (najbliziProlaz == abs(trenutnaKolona - prolazi[i]))
                    if(abs(cilj.second - kolona) > abs(cilj.second - prolazi[i])){
                        najbliziProlaz = abs(trenutnaKolona - prolazi[i]);
                        kolona = prolazi[i];
                    }
            }

            udaljenosti[trenutniRed][kolona] = udaljenosti[trenutniRed][trenutnaKolona] + najbliziProlaz;
            trenutnaKolona = kolona;
            trenutniRed++;
        }

        else if(!trenutniPrazan && prethodniPrazan){
            udaljenosti[trenutniRed + 1][trenutnaKolona] = udaljenosti[trenutniRed - 1][trenutnaKolona] + 2;
            trenutniRed += 2;
        }

        else if(trenutniRed == cilj.first){
            udaljenosti[trenutniRed][cilj.second] = udaljenosti[trenutniRed - 1][trenutnaKolona] + 1 + abs(trenutnaKolona - cilj.second);
            trenutnaKolona = cilj.second;
        }
    }

    return udaljenosti[cilj.first][cilj.second];
}


void Labirint::Ispis() {

    for(int i = 0; i < 2 * redovi; i++) {
        for(int j = 0; j < kolone; j++) {
            if(labirint[i][j]!=0)
                cout  << setw(3) << labirint[i][j];
            else
                cout  << setw(3) << "  ";
        }
        cout << endl;
    }
}

#endif // LABIRINT_CPP
