// OK_Generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
/*  kolejnosc argumentow + opis
1 numer_instancji
2 liczba_zadan
3 min_czas_operacji
4 max_czas_operacji
5 wspolczynnik_przerw w zaleznosci od liczby zadan w %

*/

using namespace std;

int main()
{
	srand(time(NULL));
	fstream uchwyt("../instancje.txt", ios::in);
	fstream wyn;
	
	int numer_instancji;
	int liczba_zadan;
	int min_czas_operacji;
	int max_czas_operacji;
	float wspolcznynnik_przerw; // >= 0.1
	int liczba_przerw;
	int min_czas_przerwy;
	int max_czas_przerwy;
	//cout << numer_instancji << endl << liczba_operacji << endl << min_czas_operacji << endl << max_czas_operacji << endl << wspolcznynnik_przerw << endl << liczba_przerw;
	int czas_operacji1;
	int czas_operacji2;
	int czas_przerwy;
	int czas_startu_przerwy;
	string nazwa;
	while (uchwyt >> numer_instancji >> liczba_zadan >> min_czas_operacji >> max_czas_operacji >> wspolcznynnik_przerw >> min_czas_przerwy >> max_czas_przerwy)
	{
		cout << numer_instancji << " " << liczba_zadan << " " << min_czas_operacji << " " << max_czas_operacji << " " << wspolcznynnik_przerw << " " <<  min_czas_przerwy << " " << max_czas_przerwy << endl;
		liczba_przerw = liczba_zadan * wspolcznynnik_przerw;
		nazwa = "../Inst/" + to_string(numer_instancji) + ".txt";
		wyn.open(nazwa, ios::out);
		wyn << numer_instancji << endl;
		wyn << liczba_zadan << endl;
		for (int i = 0; i < liczba_zadan; i++)
		{
			czas_operacji1 = min_czas_operacji + rand() % (1 + max_czas_operacji - min_czas_operacji);
			czas_operacji2 = min_czas_operacji + rand() % (1 + max_czas_operacji - min_czas_operacji);
			wyn << czas_operacji1 << " ";
			wyn << czas_operacji2 << " ";
			if (rand() % 2)
			{
				wyn << "1 2" << endl;
			}
			else
			{
				wyn << "2 1" << endl;
			}
		}
		for (int i = 1; i <= liczba_przerw; i++)
		{
			wyn << i << " " << 1 + rand() % 2 << " ";
			czas_przerwy = min_czas_operacji + rand() % (1 + max_czas_przerwy - min_czas_przerwy);
			wyn << czas_przerwy << " ";
			czas_startu_przerwy = rand() % 100; // zrobic na 2 tablicach, aby wykluczac
			wyn << czas_startu_przerwy << endl;
		}
		wyn.close();
		//cout << "here";
	}
	_getch();
	return 0;
}

