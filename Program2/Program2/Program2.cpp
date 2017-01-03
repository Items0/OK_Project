// Program2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "obiekt.h"
#include <ctime> // time
#include <cstdlib> // rand, srand
#include <conio.h> // getch
#include <algorithm> // sort
#include <iomanip> // setw w display
using namespace std;

void display(vector <obiekt> &m1, string nazwa1, vector <obiekt> &m2, string nazwa2)
{
	cout << "numer start trwanie koniec" << endl;
	cout << nazwa1 << left << setw(112) << ":" << nazwa2 << ":" << endl;
	for (int i = 0; i < m1.size(); i++)
	{
		if (m1[i].typ == "op")
		{
			cout << " " << left << setw(6) << m1[i].numer << setw(7) << m1[i].czas_startu << left << setw(100) << m1[i].czas_trwania;// << " " << m[i].czas_z_kara << " " << m[i].czas_konca << endl;
		}
	    if (m1[i].typ == "maint")
		{
			cout << " " << left << setw(6) << m1[i].numer << setw(7) << m1[i].czas_startu << setw(7) << m1[i].czas_trwania << left << setw(93) << m1[i].czas_konca;
		}
		if (i >= m2.size())
		{
			cout << endl;
		}
		else
		{
			if (m2[i].typ == "op")
			{
				cout << left << setw(7) << m2[i].numer << setw(7) << m2[i].czas_startu << setw(7) << m2[i].czas_trwania << endl;
			}
			if (m2[i].typ == "maint")
			{
				cout << left << setw(7) << m2[i].numer << setw(7) << m2[i].czas_startu << setw(7) << m2[i].czas_trwania << setw(7) << m2[i].czas_konca << endl;
			}
		}	
	}
	_getch();
}

void update(vector <obiekt> &m)
{
	for (int i = 1; i < m.size(); i++)
	{
		if (m[i].typ == "op") // moze zmieniac tylko czasy operacji
		{		
				if (m[i - 1].maint_kara)
				{
					//m[i - 2].czas_startu = m[i - 3].czas_konca;
					//m[i - 2].czas_konca = m[i - 2].czas_startu + m[i - 2].czas_trwania + m[i - 1].czas_trwania;
					m[i].czas_startu = m[i - 1].maint_kara;	
				}
				else
				{
					m[i].czas_startu = m[i - 1].czas_konca;	
				}
				m[i].czas_konca = m[i].czas_startu + m[i].czas_trwania;
				if (m.size() > i + 1 && m[i].czas_trwania == m[i].czas_z_kara && m[i + 1].typ == "maint")
				{
					m[i].czas_konca += m[i + 1].czas_trwania;
				}	
		}
		else
		{
			if (m[i].typ == "maint" && m[i - 1].czas_konca > m[i].czas_startu)
			{
				m[i].maint_kara = m[i - 1].czas_konca;
			}
		}	
	}
}

void insert_m1(vector <obiekt> &operacje, vector <obiekt> &maszyna) //losuje obiekt, szuka dla niego pierwszego wolnego miejsca
{
	srand(time(NULL));
	int x;
	int i;
	do
	{
		x = rand() % operacje.size(); //losowy z losowego generatora
		if (operacje[x].czas_instancji <= maszyna[0].czas_startu) // trafil w pierwszy przedzial <0, czas startu 1-szego>
		{
			operacje[x].czas_startu = 0;
			operacje[x].czas_konca = operacje[x].czas_trwania = operacje[x].czas_instancji;
			maszyna.insert(maszyna.begin(), operacje[x]);
			//cout << "poczatek" << endl;
		}
		else
		{
			for (i = 1; i < maszyna.size(); i++)
			{
				if ((maszyna[i - 1].maint_kara == 0 && operacje[x].czas_instancji <= maszyna[i].czas_startu - maszyna[i - 1].czas_konca)
					|| (maszyna[i - 1].maint_kara && operacje[x].czas_instancji <= maszyna[i].czas_startu - maszyna[i - 1].maint_kara))
				{
					operacje[x].czas_trwania = operacje[x].czas_instancji;
					maszyna.insert(maszyna.begin() + i, operacje[x]);
					//cout << "--przedzial---" << endl;
					break;
				}
				else
				{
					// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
					if (operacje[x].czas_z_kara - operacje[x].czas_instancji < maszyna[i].czas_startu - maszyna[i - 1].czas_konca
						&& maszyna.size() > i + 1 && maszyna[i].typ == "maint" && maszyna[i].maint_kara == 0
						&& maszyna[i + 1].czas_startu - maszyna[i].czas_konca > operacje[x].czas_z_kara - (maszyna[i].czas_startu - maszyna[i - 1].czas_konca))
					{
						operacje[x].czas_trwania = operacje[x].czas_z_kara;
						maszyna.insert(maszyna.begin() + i, operacje[x]);
						//cout << "--przedzial-WZNAwiialne---" << endl;
						break;
					}
				}	
			}
			if (i == maszyna.size()) //przelecial i nie zmiescil, wiec dodaje na koniec
			{
				operacje[x].czas_trwania = operacje[x].czas_instancji;
				maszyna.push_back(operacje[x]);
				//cout << "koniec" << endl;
			}
		}
		operacje.erase(operacje.begin() + x);
		//display(maszyna, "Przed");
		update(maszyna);
		//display(maszyna, "Pokaz");
	} while (operacje.size() != 0);
}


int zwroc_index(vector <obiekt> &m, int numer)
{
	int index = -1;
	for (int i = 0; i < m.size(); i++)
	{
		if (m[i].numer == numer)
		{
			index = i;
			break;
		}
	}
	return index;
}

void insert_m2(vector <obiekt> &operacje, vector <obiekt> &maszyna, vector <obiekt> &pierwsza) 
{
	srand(time(NULL));
	int x;
	int i;
	int koniec_na_pierwszej;
	bool czy;
	do
	{
		czy = true;
		x = rand() % operacje.size();
		koniec_na_pierwszej = pierwsza[zwroc_index(pierwsza, x + 1)].czas_konca;
		//cout << " -- " << x + 1 << "    " << koniec_na_pierwszej << endl;
		if ( koniec_na_pierwszej <= maszyna[0].czas_startu && operacje[x].czas_instancji <= maszyna[0].czas_startu)
		{
			operacje[x].czas_startu = koniec_na_pierwszej;
		    operacje[x].czas_trwania = operacje[x].czas_instancji;
			operacje[x].czas_konca = operacje[x].czas_startu + operacje[x].czas_trwania;
			maszyna.insert(maszyna.begin(), operacje[x]);
			czy = false;
		}
		else
		{
			for (i = 1; i < maszyna.size(); i++)
			{
				if ((koniec_na_pierwszej <= maszyna[i-1].czas_konca && maszyna[i - 1].maint_kara == 0 && operacje[x].czas_instancji <= maszyna[i].czas_startu - maszyna[i - 1].czas_konca)
					|| (maszyna[i - 1].maint_kara && operacje[x].czas_instancji <= maszyna[i].czas_startu - maszyna[i - 1].maint_kara))
				{
					operacje[x].czas_trwania = operacje[x].czas_instancji;
					maszyna.insert(maszyna.begin() + i, operacje[x]);
					czy = false;
					cout << " -- " << x + 1 << "    " << koniec_na_pierwszej << endl;
					//cout << "--przedzial---" << endl;
					break;
				}
				else
				{
					// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
					if (koniec_na_pierwszej <= maszyna[i - 1].czas_konca && operacje[x].czas_z_kara - operacje[x].czas_instancji < maszyna[i].czas_startu - maszyna[i - 1].czas_konca
						&& maszyna.size() > i + 1 && maszyna[i].typ == "maint" && maszyna[i].maint_kara == 0
						&& maszyna[i + 1].czas_startu - maszyna[i].czas_konca > operacje[x].czas_z_kara - (maszyna[i].czas_startu - maszyna[i - 1].czas_konca))
					{
						operacje[x].czas_trwania = operacje[x].czas_z_kara;
						maszyna.insert(maszyna.begin() + i, operacje[x]);
						czy = false;
						//cout << "--przedzial-WZNAwiialne---" << endl;
						break;
					}
				}
			}
			if (i == maszyna.size() && koniec_na_pierwszej <= maszyna[i - 1].czas_konca) //przelecial i nie zmiescil, wiec dodaje na koniec
			{
				operacje[x].czas_trwania = operacje[x].czas_instancji;
				maszyna.push_back(operacje[x]);
				czy = false;
				//cout << "koniec" << endl;
			}
		}
		if (czy == false)
		{
			operacje.erase(operacje.begin() + x);
			update(maszyna);
		}
		
		//display(maszyna, "Przed");		
		//display(maszyna, "Pokaz");
	} while (operacje.size() != 0);
}



void wypis(vector <obiekt> &maszyna)
{

}


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
				m1_operacje.emplace_back(k, czas_operacji1, 1);
				m2_operacje.emplace_back(k, czas_operacji2, 2);
			}
			else
			{
				m1_operacje.emplace_back(k, czas_operacji2, 2);
				m2_operacje.emplace_back(k, czas_operacji1, 1);
			}
		}
		while (uchwyt >> numer_przerwy >> przerwa_maszyna >> czas_przerwy >> start_przerwy)
		{
			if (przerwa_maszyna == 1)
			{
				m1.emplace_back(numer_przerwy, czas_przerwy, start_przerwy, 1);
			}
			else
			{
				m2.emplace_back(numer_przerwy, czas_przerwy, start_przerwy, 2);
			}
		}
		uchwyt.close();
		sort(m1.begin(), m1.end(), compare);
		sort(m2.begin(), m2.end(), compare);
		display(m1, "M1", m2, "M2");
		insert_m1(m1_operacje, m1);	
		insert_m2(m2_operacje, m2, m1);
		display(m1, "M1", m2, "M2");
		m1.clear();
		m2.clear();
	}
}

