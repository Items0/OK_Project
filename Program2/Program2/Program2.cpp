// Program2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "obiekt.h"
#include <conio.h> //getch
#include <algorithm> // sort
using namespace std;

void display(vector <obiekt> &m, string nazwa)
{
	cout << nazwa << ":" << endl;
	for (int i = 0; i < m.size(); i++)
	{
		if (m[i].typ == "op")
		{
			cout << m[i].numer << " " << m[i].czas_instancji << endl;
		}
		else if (m[i].typ == "maint")
		{
			cout << m[i].numer << " " << m[i].czas_startu << " " << m[i].czas_trwania << " " << m[i].czas_konca << endl;
		}
	}
	_getch();
}

//int zwroc_index(vector <obiekt> &m, int start)
//{
//	if (m.size() == 0)
//	{
//		return 0;
//	}
//	else
//	{
//		int i;
//		for (i = 0; i < m.size() && m[i].czas_startu < start; i++);
//		return i;
//	}
//}


//void update(vector <obiekt> &m)
//{
//	m[0].czas_startu = 0;
//	m[0].czas
//	for (int i = 1; i < m.size(); i++)
//	{
//		m[i].czas
//	}
//}

bool compare(obiekt a, obiekt b)
{
	return (a.czas_startu < b.czas_startu);	
}
int main()
{
	string nazwa = "../../Instancje/";
	string nazwa_rozw = "../../Rozwiazania/";
	fstream uchwyt("../../LiczbaInstancji.txt", ios::in);
	fstream wynik;
	vector <obiekt> m1; //maszyna 1
	vector <obiekt> m2; //maszyna 2
	vector <obiekt> m1_operacje; //operacje dla m1
	vector <obiekt> m2_operacje; //operacje dla m2
	int liczba_instancji;
	int liczba_operacji;
	int czas_operacji1;
	int czas_operacji2;
	int nmdop1; // numer maszyny dla operacji 1
	int nmdop2;
	int numer_przerwy;
	int przerwa_maszyna;
	int czas_przerwy;
	int start_przerwy;
	uchwyt >> liczba_instancji;
	uchwyt.close();
	for (int i = 1; i <= liczba_instancji; i++)
	{
		uchwyt.open(nazwa + to_string(i) + ".txt", ios::in);
		uchwyt >> liczba_operacji;
		for (int k = 1; k <= liczba_operacji; k++)
		{
			uchwyt >> czas_operacji1 >> czas_operacji2 >> nmdop1 >> nmdop2;
			if (nmdop1 == 1) // operacja pierwsza na pierwszej maszynie
			{
				m1_operacje.emplace_back(k, czas_operacji1);
				m2_operacje.emplace_back(k, czas_operacji2);
			}
			else
			{ 
				m1_operacje.emplace_back(k, czas_operacji2);
				m2_operacje.emplace_back(k, czas_operacji1);
			}
		}
		while (uchwyt >> numer_przerwy >> przerwa_maszyna >> czas_przerwy >> start_przerwy)
		{
			if (przerwa_maszyna == 1)
			{
				m1.emplace_back(numer_przerwy, czas_przerwy, start_przerwy);
			}
			else
			{
				m2.emplace_back(numer_przerwy, czas_przerwy, start_przerwy);
			}
		}
		uchwyt.close();
		sort(m1.begin(), m1.end(), compare);
		sort(m2.begin(), m2.end(), compare);
		display(m1, "M1");
		display(m2, "M2");
		m1.clear();
		m2.clear();
	}


}

