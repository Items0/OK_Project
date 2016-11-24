// OK_Generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
/*  kolejnosc argumentow + opis
1 numer_instancji
2 liczba_operacji
3 min_czas_operacji
4 max_czas_operacji
5 wspolczynnik_przerw w zaleznosci od liczby zadan w %

*/

using namespace std;

int StrToInt(char tab[])
{
	istringstream napis(tab);
	int result;
	napis >> result;
	return result;
}


int main(int argc, char *argv[])
{
	if (argc > 6) { cout << "Zla liczba arg" << endl; exit(-1); }
	int numer_instancji = StrToInt(argv[1]);
	int liczba_operacji = StrToInt(argv[2]);
	int min_czas_operacji = StrToInt(argv[3]);
	int max_czas_operacji = StrToInt(argv[4]);
	float wspolcznynnik_przerw = 1.0 * StrToInt(argv[5]) / 100;
	int liczba_przerw = liczba_operacji * wspolcznynnik_przerw;
	//cout << numer_instancji << endl << liczba_operacji << endl << min_czas_operacji << endl << max_czas_operacji << endl << wspolcznynnik_przerw << endl << liczba_przerw;
	return 0;
}

