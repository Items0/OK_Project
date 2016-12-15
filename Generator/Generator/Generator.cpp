// OK_Generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <vector>
/*  kolejnosc argumentow + opis
1 numer_instancji
2 liczba_zadan
3 min_czas_operacji
4 max_czas_operacji
5 wspolczynnik_przerw w zaleznosci od liczby zadan w %

*/

using namespace std;

vector < vector <int> > tab(3); //tab[0] nieuzywany

bool porownaj(int czas_startu, int czas_trwania, int nr)
{
	bool wartosc = false;
	if (czas_startu + czas_trwania > tab[nr].size())
	{
		wartosc = true;
	}
	else
	{
		for (int i = czas_startu; i < czas_startu + czas_trwania; i++)
		{
			if (tab[nr][i])
			{
				wartosc = true;
				break;
			}
		}
		if (wartosc == false)
		{
			for (int i = czas_startu; i < czas_startu + czas_trwania; i++)
			{
				tab[nr][i] = 1;
			}
		}
	}
	return wartosc;
}

int main()
{
	srand(time(NULL));
	fstream uchwyt("../../InstancjeGenerowanie.txt", ios::in);
	fstream wyn;
	
	int numer_instancji;
	int liczba_zadan;
	int min_czas_operacji;
	int max_czas_operacji;
	float wspolcznynnik_przerw; // >= 0.1
	int liczba_przerw;
	int min_czas_przerwy;
	int max_czas_przerwy;
	//cout << numer_instancji << endl << liczba_zadan << endl << min_czas_operacji << endl << max_czas_operacji << endl << wspolcznynnik_przerw << endl << liczba_przerw;
	int czas_operacji1;
	int czas_operacji2;
	int czas_przerwy;
	int czas_startu_przerwy;
	int suma_czasow_m1 = 0; // do wyznaczania odpowiednich przerw
	int suma_czasow_m2 = 0;
	int nr_maszyny; // jako arg funkcji sprawdzajacej
	string nazwa;
	while (uchwyt >> numer_instancji >> liczba_zadan >> min_czas_operacji >> max_czas_operacji >> wspolcznynnik_przerw >> min_czas_przerwy >> max_czas_przerwy)
	{
		cout << numer_instancji << " " << liczba_zadan << " " << min_czas_operacji << " " << max_czas_operacji << " " << wspolcznynnik_przerw << " " <<  min_czas_przerwy << " " << max_czas_przerwy << endl;
		liczba_przerw = liczba_zadan * wspolcznynnik_przerw;
		nazwa = "../../Instancje/" + to_string(numer_instancji) + ".txt";
		wyn.open(nazwa, ios::out);
		//wyn << numer_instancji << endl;
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
				suma_czasow_m1 += czas_operacji1;
				suma_czasow_m2 += czas_operacji2;
			}
			else
			{
				wyn << "2 1" << endl;
				suma_czasow_m1 += czas_operacji2;
				suma_czasow_m2 += czas_operacji1;
			}
		}
		if (suma_czasow_m1 > RAND_MAX || suma_czasow_m2 > RAND_MAX) { cout << "Uwazaj, Losowanie z przedzialu < RAND_MAX, bledy !" << endl; } // potencjalne bledy, wiem w jakim przedziale jestem
		tab[1].assign(suma_czasow_m1, 0);
		tab[2].assign(suma_czasow_m2, 0);
		for (int i = 1; i <= liczba_przerw; i++) // k przerw dla 1 maszyny
		{	
			wyn << i << " 1 ";
			czas_przerwy = min_czas_przerwy + rand() % (1 + max_czas_przerwy - min_czas_przerwy);
			wyn << czas_przerwy << " ";
			do
			{
					czas_startu_przerwy = rand() % suma_czasow_m1;
			} 
			while (porownaj(czas_startu_przerwy, czas_przerwy, 1));
			wyn << czas_startu_przerwy << endl;
		}
		for (int i = liczba_przerw + 1; i <= 2 * liczba_przerw; i++) // k przerw dla 2 maszyny
		{
			wyn << i << " 2 ";
			czas_przerwy = min_czas_przerwy + rand() % (1 + max_czas_przerwy - min_czas_przerwy);
			wyn << czas_przerwy << " ";
			do
			{
				czas_startu_przerwy = rand() % suma_czasow_m2;
			}
			while (porownaj(czas_startu_przerwy, czas_przerwy, 2));
			wyn << czas_startu_przerwy << endl;
		}
		wyn.close();
		wyn.open("../../LiczbaInstancji.txt", ios::out); // aby otrzymac ostatni numer instancji, wykorzystywany w Program2
		wyn << numer_instancji;
		wyn.close();
	}

	_getch();
	return 0;
}