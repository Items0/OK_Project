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
#include <tuple> // tuple
using namespace std;

//global - sterowanie
int global_procent_mutacji;// = 20;
int global_procent_krzyzowania;// = 30;
int liczba_iteracji;// = 10;
int wielkosc_populacji;// = 50; // dla selekcji
int poczatkowa_populacja;// = 10;
/////////////////////
void display(vector <obiekt> &m1, string nazwa1, vector <obiekt> &m2, string nazwa2)
{
	//cout << "numer start trwanie koniec" << endl;
	cout << nazwa1 << left << setw(4) << ":" << setw(108) << m1.size() << nazwa2 << ":" << setw(4) << right << m2.size() << endl;
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
	//_getch();
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
				if (m[i].ruszaj)
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

int zwroc_index(vector <obiekt> &m, int numer)
{
	int index = -1;
	for (int i = 0; i < m.size(); i++)
	{
		if (m[i].typ == "op" && m[i].numer == numer)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		//cout << "error";
	}
	else
	{
		return index;
	}
}

int zwroc_indexx(vector <int> m, int numer)
{
	int index = -1;
	for (int i = 0; i < m.size(); i++)
	{
		if (m[i] == numer)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		return index;
		//cout << "error";
	}
	else
	{
		return index;
	}
}

void insert_M(vector <obiekt> m1_operacje, vector <obiekt> m1_operacje_drugie, vector <obiekt> &maszyna1, vector <obiekt> m2_operacje, vector <obiekt> m2_operacje_drugie, vector <obiekt> &maszyna2, vector <int> &m1_kolejnosc, vector <int> &m2_kolejnosc)
{
	//trzeba jednoczesnie 2 maszyny obsluzyc, -> przeplatanie sie w do-while
	//srand(time(NULL));
	int x;
	int i;
	bool czy;
	int koniec_pierwszej_op;
	do
	{
		// dla 1-szej maszyny
		if (m1_operacje.size() != 0)
		{
			x = rand() % m1_operacje.size(); //losowy z losowego generatora
			m1_kolejnosc.push_back(x);
			if (m1_operacje[x].nr_operacji == 1)
			{
				if (m1_operacje[x].czas_instancji <= maszyna1[0].czas_startu) // trafil w pierwszy przedzial <0, czas startu 1-szego>
				{
					m1_operacje[x].czas_startu = 0;
					m1_operacje[x].czas_konca = m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
					maszyna1.insert(maszyna1.begin(), m1_operacje[x]);
				}
				else
				{
					for (i = 1; i < maszyna1.size(); i++)
					{
						if ((maszyna1[i - 1].maint_kara == 0 && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca)
							|| (maszyna1[i - 1].maint_kara && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].maint_kara))
						{
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
							break;
						}
						else
						{
							// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
							if ((m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca
								&& maszyna1.size() > i + 1 && maszyna1[i].typ == "maint" && maszyna1[i].maint_kara == 0
								&& maszyna1[i + 1].czas_startu - maszyna1[i].czas_konca > m1_operacje[x].czas_z_kara - (maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca))
								|| (m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca && i + 1 == maszyna1.size()))
							{
								m1_operacje[x].czas_trwania = m1_operacje[x].czas_z_kara;
								maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
								break;
							}
						}
					}
					if (i == maszyna1.size()) //przelecial i nie zmiescil, wiec dodaje na koniec
					{
						m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
						maszyna1.push_back(m1_operacje[x]);
					}
				}
				m2_operacje.push_back(m2_operacje_drugie[zwroc_index(m2_operacje_drugie, m1_operacje[x].numer)]);
				m2_operacje_drugie.erase(m2_operacje_drugie.begin() + zwroc_index(m2_operacje_drugie, m1_operacje[x].numer));
				m1_operacje.erase(m1_operacje.begin() + x);
				update(maszyna1);
			}
			else
			{
				czy = true;
				koniec_pierwszej_op = maszyna2[zwroc_index(maszyna2, m1_operacje[x].numer)].czas_konca;
				if (koniec_pierwszej_op <= maszyna1[0].czas_startu && m1_operacje[x].czas_instancji <= maszyna1[0].czas_startu)
				{
					m1_operacje[x].czas_startu = koniec_pierwszej_op;
					m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
					m1_operacje[x].czas_konca = m1_operacje[x].czas_startu + m1_operacje[x].czas_trwania;
					m1_operacje[x].ruszaj = false; // 09/01/17
					maszyna1.insert(maszyna1.begin(), m1_operacje[x]);
					czy = false;
				}
				else
				{
					for (i = 1; i < maszyna1.size(); i++)
					{
						if ((koniec_pierwszej_op <= maszyna1[i - 1].czas_konca && maszyna1[i - 1].maint_kara == 0 && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca)
							|| (maszyna1[i - 1].maint_kara && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].maint_kara))
						{
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
							czy = false;
							break;
						}
						else
						{
							if ((koniec_pierwszej_op <= maszyna1[i - 1].czas_konca && m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca
								&& maszyna1.size() > i + 1 && maszyna1[i].typ == "maint" && maszyna1[i].maint_kara == 0
								&& maszyna1[i + 1].czas_startu - maszyna1[i].czas_konca > m1_operacje[x].czas_z_kara - (maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca))
								|| (koniec_pierwszej_op <= maszyna1[i - 1].czas_konca && m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca && i + 1 == maszyna1.size()))
							{
								m1_operacje[x].czas_trwania = m1_operacje[x].czas_z_kara;
								maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
								czy = false;
								break;
							}
						}
					}
					if (i == maszyna1.size())
					{
						if (koniec_pierwszej_op <= maszyna1[i - 1].czas_konca)
						{
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							maszyna1.push_back(m1_operacje[x]);
							czy = false;
						}
						else
						{
							m1_operacje[x].czas_startu = koniec_pierwszej_op;
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							m1_operacje[x].ruszaj = false;
							maszyna1.push_back(m1_operacje[x]);
							czy = false;
						}
					}
				}
				if (czy == false)
				{
					m1_operacje.erase(m1_operacje.begin() + x);
					update(maszyna1);
				}
			}
		}
		if (m2_operacje.size() != 0)
		{
			x = rand() % m2_operacje.size();
			m2_kolejnosc.push_back(x);
			if (m2_operacje[x].nr_operacji == 1)
			{
				if (m2_operacje[x].czas_instancji <= maszyna2[0].czas_startu)
				{
					m2_operacje[x].czas_startu = 0;
					m2_operacje[x].czas_konca = m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
					maszyna2.insert(maszyna2.begin(), m2_operacje[x]);
				}
				else
				{
					for (i = 1; i < maszyna2.size(); i++)
					{
						if ((maszyna2[i - 1].maint_kara == 0 && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca)
							|| (maszyna2[i - 1].maint_kara && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].maint_kara))
						{
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
							break;
						}
						else
						{
							if ((m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca
								&& maszyna2.size() > i + 1 && maszyna2[i].typ == "maint" && maszyna2[i].maint_kara == 0
								&& maszyna2[i + 1].czas_startu - maszyna2[i].czas_konca > m2_operacje[x].czas_z_kara - (maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca))
								|| (m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca && i + 1 == maszyna2.size()))
							{
								m2_operacje[x].czas_trwania = m2_operacje[x].czas_z_kara;
								maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
								break;
							}
						}
					}
					if (i == maszyna2.size())
					{
						m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
						maszyna2.push_back(m2_operacje[x]);
					}
				}
				m1_operacje.push_back(m1_operacje_drugie[zwroc_index(m1_operacje_drugie, m2_operacje[x].numer)]);
				m1_operacje_drugie.erase(m1_operacje_drugie.begin() + zwroc_index(m1_operacje_drugie, m2_operacje[x].numer));
				m2_operacje.erase(m2_operacje.begin() + x);
				update(maszyna2);
			}
			else
			{
				czy = true;
				koniec_pierwszej_op = maszyna1[zwroc_index(maszyna1, m2_operacje[x].numer)].czas_konca;
				if (koniec_pierwszej_op <= maszyna2[0].czas_startu && m2_operacje[x].czas_instancji <= maszyna2[0].czas_startu)
				{
					m2_operacje[x].czas_startu = koniec_pierwszej_op;
					m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
					m2_operacje[x].czas_konca = m2_operacje[x].czas_startu + m2_operacje[x].czas_trwania;
					m2_operacje[x].ruszaj = false; // 09/01/17
					maszyna2.insert(maszyna2.begin(), m2_operacje[x]);
					czy = false;
				}
				else
				{
					for (i = 1; i < maszyna2.size(); i++)
					{
						if ((koniec_pierwszej_op <= maszyna2[i - 1].czas_konca && maszyna2[i - 1].maint_kara == 0 && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca)
							|| (maszyna2[i - 1].maint_kara && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].maint_kara))
						{
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
							czy = false;
							break;
						}
						else
						{
							if ((koniec_pierwszej_op <= maszyna2[i - 1].czas_konca && m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca
								&& maszyna2.size() > i + 1 && maszyna2[i].typ == "maint" && maszyna2[i].maint_kara == 0
								&& maszyna2[i + 1].czas_startu - maszyna2[i].czas_konca > m2_operacje[x].czas_z_kara - (maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca))
								|| (koniec_pierwszej_op <= maszyna2[i - 1].czas_konca && m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca && i + 1 == maszyna2.size()))
							{
								m2_operacje[x].czas_trwania = m2_operacje[x].czas_z_kara;
								maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
								czy = false;
								break;
							}
						}
					}
					if (i == maszyna2.size())
					{
						if (koniec_pierwszej_op <= maszyna2[i - 1].czas_konca)
						{
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							maszyna2.push_back(m2_operacje[x]);
							czy = false;
						}
						else
						{
							m2_operacje[x].czas_startu = koniec_pierwszej_op;
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							m2_operacje[x].ruszaj = false;
							maszyna2.push_back(m2_operacje[x]);
							czy = false;
						}
					}

				}
				if (czy == false)
				{
					m2_operacje.erase(m2_operacje.begin() + x);
					update(maszyna2);
				}
			}
		}

		//display(maszyna1, "M1", maszyna2, "M2")
	} while (m2_operacje.size() != 0 || m1_operacje.size() != 0 || m2_operacje_drugie.size() != 0 || m1_operacje_drugie.size() != 0);
}

void insert_kolejnosc(vector <obiekt> m1_operacje, vector <obiekt> m1_operacje_drugie, vector <obiekt> &maszyna1, vector <obiekt> m2_operacje, vector <obiekt> m2_operacje_drugie, vector <obiekt> &maszyna2, vector <int> m1_kolejnosc, vector <int> m2_kolejnosc) // dwa parametry przez wartosc
{
	//trzeba jednoczesnie 2 maszyny obsluzyc, -> przeplatanie sie w do-while
	//srand(time(NULL));
	int x;
	int i;
	bool czy;
	int koniec_pierwszej_op;
	do
	{
		// dla 1-szej maszyny
		if (m1_operacje.size() != 0)
		{
			x = m1_kolejnosc[0]; //pierwszy z odtworzonej kolejnosci
			m1_kolejnosc.erase(m1_kolejnosc.begin());
			if (m1_operacje[x].nr_operacji == 1)
			{
				if (m1_operacje[x].czas_instancji <= maszyna1[0].czas_startu) // trafil w pierwszy przedzial <0, czas startu 1-szego>
				{
					m1_operacje[x].czas_startu = 0;
					m1_operacje[x].czas_konca = m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
					maszyna1.insert(maszyna1.begin(), m1_operacje[x]);
				}
				else
				{
					for (i = 1; i < maszyna1.size(); i++)
					{
						if ((maszyna1[i - 1].maint_kara == 0 && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca)
							|| (maszyna1[i - 1].maint_kara && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].maint_kara))
						{
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
							break;
						}
						else
						{
							// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
							if ((m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca
								&& maszyna1.size() > i + 1 && maszyna1[i].typ == "maint" && maszyna1[i].maint_kara == 0
								&& maszyna1[i + 1].czas_startu - maszyna1[i].czas_konca > m1_operacje[x].czas_z_kara - (maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca))
								|| (m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca && i + 1 == maszyna1.size()))
							{
								m1_operacje[x].czas_trwania = m1_operacje[x].czas_z_kara;
								maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
								break;
							}
						}
					}
					if (i == maszyna1.size()) //przelecial i nie zmiescil, wiec dodaje na koniec
					{
						m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
						maszyna1.push_back(m1_operacje[x]);
					}
				}
				m2_operacje.push_back(m2_operacje_drugie[zwroc_index(m2_operacje_drugie, m1_operacje[x].numer)]);
				m2_operacje_drugie.erase(m2_operacje_drugie.begin() + zwroc_index(m2_operacje_drugie, m1_operacje[x].numer));
				m1_operacje.erase(m1_operacje.begin() + x);
				update(maszyna1);
			}
			else
			{
				czy = true;
				koniec_pierwszej_op = maszyna2[zwroc_index(maszyna2, m1_operacje[x].numer)].czas_konca;
				if (koniec_pierwszej_op <= maszyna1[0].czas_startu && m1_operacje[x].czas_instancji <= maszyna1[0].czas_startu)
				{
					m1_operacje[x].czas_startu = koniec_pierwszej_op;
					m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
					m1_operacje[x].czas_konca = m1_operacje[x].czas_startu + m1_operacje[x].czas_trwania;
					m1_operacje[x].ruszaj = false; // 09/01/17
					maszyna1.insert(maszyna1.begin(), m1_operacje[x]);
					czy = false;
				}
				else
				{
					for (i = 1; i < maszyna1.size(); i++)
					{
						if ((koniec_pierwszej_op <= maszyna1[i - 1].czas_konca && maszyna1[i - 1].maint_kara == 0 && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca)
							|| (maszyna1[i - 1].maint_kara && m1_operacje[x].czas_instancji <= maszyna1[i].czas_startu - maszyna1[i - 1].maint_kara))
						{
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
							czy = false;
							break;
						}
						else
						{
							if ((koniec_pierwszej_op <= maszyna1[i - 1].czas_konca && m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca
								&& maszyna1.size() > i + 1 && maszyna1[i].typ == "maint" && maszyna1[i].maint_kara == 0
								&& maszyna1[i + 1].czas_startu - maszyna1[i].czas_konca > m1_operacje[x].czas_z_kara - (maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca))
								|| (koniec_pierwszej_op <= maszyna1[i - 1].czas_konca && m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca && i + 1 == maszyna1.size()))
							{
								m1_operacje[x].czas_trwania = m1_operacje[x].czas_z_kara;
								maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
								czy = false;
								break;
							}
						}
					}
					if (i == maszyna1.size())
					{
						if (koniec_pierwszej_op <= maszyna1[i - 1].czas_konca)
						{
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							maszyna1.push_back(m1_operacje[x]);
							czy = false;
						}
						else
						{
							m1_operacje[x].czas_startu = koniec_pierwszej_op;
							m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
							m1_operacje[x].ruszaj = false;
							maszyna1.push_back(m1_operacje[x]);
							czy = false;
						}
					}
				}
				if (czy == false)
				{
					m1_operacje.erase(m1_operacje.begin() + x);
					update(maszyna1);
				}
			}
		}
		if (m2_operacje.size() != 0)
		{
			x = m2_kolejnosc[0]; //pierwszy z odtworzonej kolejnosci
			m2_kolejnosc.erase(m2_kolejnosc.begin());
			if (m2_operacje[x].nr_operacji == 1)
			{
				if (m2_operacje[x].czas_instancji <= maszyna2[0].czas_startu)
				{
					m2_operacje[x].czas_startu = 0;
					m2_operacje[x].czas_konca = m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
					maszyna2.insert(maszyna2.begin(), m2_operacje[x]);
				}
				else
				{
					for (i = 1; i < maszyna2.size(); i++)
					{
						if ((maszyna2[i - 1].maint_kara == 0 && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca)
							|| (maszyna2[i - 1].maint_kara && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].maint_kara))
						{
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
							break;
						}
						else
						{
							if ((m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca
								&& maszyna2.size() > i + 1 && maszyna2[i].typ == "maint" && maszyna2[i].maint_kara == 0
								&& maszyna2[i + 1].czas_startu - maszyna2[i].czas_konca > m2_operacje[x].czas_z_kara - (maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca))
								|| (m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca && i + 1 == maszyna2.size()))
							{
								m2_operacje[x].czas_trwania = m2_operacje[x].czas_z_kara;
								maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
								break;
							}
						}
					}
					if (i == maszyna2.size())
					{
						m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
						maszyna2.push_back(m2_operacje[x]);
					}
				}
				m1_operacje.push_back(m1_operacje_drugie[zwroc_index(m1_operacje_drugie, m2_operacje[x].numer)]);
				m1_operacje_drugie.erase(m1_operacje_drugie.begin() + zwroc_index(m1_operacje_drugie, m2_operacje[x].numer));
				m2_operacje.erase(m2_operacje.begin() + x);
				update(maszyna2);
			}
			else
			{
				czy = true;
				koniec_pierwszej_op = maszyna1[zwroc_index(maszyna1, m2_operacje[x].numer)].czas_konca;
				if (koniec_pierwszej_op <= maszyna2[0].czas_startu && m2_operacje[x].czas_instancji <= maszyna2[0].czas_startu)
				{
					m2_operacje[x].czas_startu = koniec_pierwszej_op;
					m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
					m2_operacje[x].czas_konca = m2_operacje[x].czas_startu + m2_operacje[x].czas_trwania;
					m2_operacje[x].ruszaj = false; // 09/01/17
					maszyna2.insert(maszyna2.begin(), m2_operacje[x]);
					czy = false;
				}
				else
				{
					for (i = 1; i < maszyna2.size(); i++)
					{
						if ((koniec_pierwszej_op <= maszyna2[i - 1].czas_konca && maszyna2[i - 1].maint_kara == 0 && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca)
							|| (maszyna2[i - 1].maint_kara && m2_operacje[x].czas_instancji <= maszyna2[i].czas_startu - maszyna2[i - 1].maint_kara))
						{
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
							czy = false;
							break;
						}
						else
						{
							if ((koniec_pierwszej_op <= maszyna2[i - 1].czas_konca && m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca
								&& maszyna2.size() > i + 1 && maszyna2[i].typ == "maint" && maszyna2[i].maint_kara == 0
								&& maszyna2[i + 1].czas_startu - maszyna2[i].czas_konca > m2_operacje[x].czas_z_kara - (maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca))
								|| (koniec_pierwszej_op <= maszyna2[i - 1].czas_konca && m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca && i + 1 == maszyna2.size()))
							{
								m2_operacje[x].czas_trwania = m2_operacje[x].czas_z_kara;
								maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
								czy = false;
								break;
							}
						}
					}
					if (i == maszyna2.size())
					{
						if (koniec_pierwszej_op <= maszyna2[i - 1].czas_konca)
						{
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							maszyna2.push_back(m2_operacje[x]);
							czy = false;
						}
						else
						{
							m2_operacje[x].czas_startu = koniec_pierwszej_op;
							m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
							m2_operacje[x].ruszaj = false;
							maszyna2.push_back(m2_operacje[x]);
							czy = false;
						}
					}

				}
				if (czy == false)
				{
					m2_operacje.erase(m2_operacje.begin() + x);
					update(maszyna2);
				}
			}
		}

		//display(maszyna1, "M1", maszyna2, "M2")
	} while (m2_operacje.size() != 0 || m1_operacje.size() != 0 || m2_operacje_drugie.size() != 0 || m1_operacje_drugie.size() != 0);
}

void insert_wycinajacy(vector <obiekt> &m1_operacje, vector <obiekt> &m1_operacje_drugie, vector <obiekt> &m2_operacje, vector <obiekt> &m2_operacje_drugie, vector <int> &m1_kolejnosc, vector <int> &m2_kolejnosc, int liczba_przebiegow)
{
	int x;
	do
	{
		if (m1_operacje.size() != 0)
		{
			x = m1_kolejnosc[0]; //pierwszy z odtworzonej kolejnosci
			m1_kolejnosc.erase(m1_kolejnosc.begin());
			if (m1_operacje[x].nr_operacji == 1)
			{
				m2_operacje.push_back(m2_operacje_drugie[zwroc_index(m2_operacje_drugie, m1_operacje[x].numer)]);
				m2_operacje_drugie.erase(m2_operacje_drugie.begin() + zwroc_index(m2_operacje_drugie, m1_operacje[x].numer));
			}
			m1_operacje.erase(m1_operacje.begin() + x);
		}
		if (m2_operacje.size() != 0)
		{
			x = m2_kolejnosc[0]; //pierwszy z odtworzonej kolejnosci
			m2_kolejnosc.erase(m2_kolejnosc.begin());
			if (m2_operacje[x].nr_operacji == 1)
			{
				m1_operacje.push_back(m1_operacje_drugie[zwroc_index(m1_operacje_drugie, m2_operacje[x].numer)]);
				m1_operacje_drugie.erase(m1_operacje_drugie.begin() + zwroc_index(m1_operacje_drugie, m2_operacje[x].numer));
			}
			m2_operacje.erase(m2_operacje.begin() + x);
		}			
		liczba_przebiegow--;
	} while (liczba_przebiegow);
}

void insert_wycinajacy_z_ktorym(vector <obiekt> m1_operacje, vector <obiekt> m1_operacje_drugie, vector <obiekt> m2_operacje, vector <obiekt> m2_operacje_drugie, vector <int> m1_kolejnosc, vector <int> m2_kolejnosc, vector <int> &m1_kol_wyb, vector <int> &m2_kol_wyb)
{
	// kartka z ktorym
	int x;
	do
	{
		if (m1_operacje.size() != 0)
		{
			x = m1_kolejnosc[0]; //pierwszy z odtworzonej kolejnosci
			m1_kolejnosc.erase(m1_kolejnosc.begin());
			m1_kol_wyb.push_back(m1_operacje[x].numer);
			if (m1_operacje[x].nr_operacji == 1)
			{
				m2_operacje.push_back(m2_operacje_drugie[zwroc_index(m2_operacje_drugie, m1_operacje[x].numer)]);
				m2_operacje_drugie.erase(m2_operacje_drugie.begin() + zwroc_index(m2_operacje_drugie, m1_operacje[x].numer));
			}
			m1_operacje.erase(m1_operacje.begin() + x);
		}
		
		if (m2_operacje.size() != 0)
		{
			x = m2_kolejnosc[0]; //pierwszy z odtworzonej kolejnosci
			m2_kolejnosc.erase(m2_kolejnosc.begin());
			m2_kol_wyb.push_back(m2_operacje[x].numer);
			if (m2_operacje[x].nr_operacji == 1)
			{
				m1_operacje.push_back(m1_operacje_drugie[zwroc_index(m1_operacje_drugie, m2_operacje[x].numer)]);
				m1_operacje_drugie.erase(m1_operacje_drugie.begin() + zwroc_index(m1_operacje_drugie, m2_operacje[x].numer));
			}
			m2_operacje.erase(m2_operacje.begin() + x);
		}		
	} while (m1_operacje.size() != 0 || m2_operacje.size() != 0);
}

bool compare(obiekt a, obiekt b)
{
	return (a.czas_startu < b.czas_startu);
}

bool klucz_sort(pair <int, int> a, pair <int, int> b)
{
	return (a.second < b.second);
}
/*bool tutaj(tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt> > a, tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt> > b)
{
if (max(get<0>(a)[get<0>(a).size() - 1].czas_konca, get<1>(a)[get<1>(a).size() - 1].czas_konca) < max(get<0>(b)[get<0>(b).size() - 1].czas_konca, get<1>(b)[get<1>(b).size() - 1].czas_konca))
{
return false;
}
else
return true;
}*/

void mutacja(vector <obiekt> &tab)
{
	//srand(time(NULL));
	int pozycja;
	int pozycja_cel;
	pozycja = rand() % tab.size();
	do
	{
		pozycja_cel = rand() % tab.size();
	} while (pozycja == pozycja_cel);
	//cout << "poz = " << pozycja << " nr = " <<tab[pozycja].numer << " cel = " << pozycja_cel << " nr = " << tab[pozycja_cel].numer << endl;
	swap(tab[pozycja], tab[pozycja_cel]);
}

void roznica(vector <int> &odjemna, vector <int> odjemnik)
{
	for (int i = 0; i < odjemnik.size(); i++)
	{
		for (int k = 0; k < odjemna.size(); k++)
		{
			if (odjemna[k] == odjemnik[i])
			{
				odjemna.erase(odjemna.begin() + k);
				break;
			}
		}	
	}
}

tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > krzyzowanie(tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > wyjsciowy, tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > z_ktorym)
{
	tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > nowy = wyjsciowy;
	//W - wyjsciowy, Z - z_ktorym, N - nowy
	vector <obiekt> m1N; 
	vector <obiekt> m2N;
	vector <obiekt> m1W_operacje, m1Z_operacje, m1N_operacje;
	vector <obiekt> m2W_operacje, m2Z_operacje, m2N_operacje;
	vector <obiekt> m1W_operacje_drugie, m1Z_operacje_drugie, m1N_operacje_drugie;
	vector <obiekt> m2W_operacje_drugie, m2Z_operacje_drugie, m2N_operacje_drugie;
	vector <int> m1W_kolejnosc, m1N_kolejnosc, m1Z_kolejnosc;
	vector <int> m2W_kolejnosc, m2N_kolejnosc, m2Z_kolejnosc;
	tie(ignore, ignore, m1W_operacje, m2W_operacje, m1W_operacje_drugie, m2W_operacje_drugie, m1W_kolejnosc, m2W_kolejnosc) = wyjsciowy;
	tie(ignore, ignore, m1Z_operacje, m2Z_operacje, m1Z_operacje_drugie, m2Z_operacje_drugie, m1Z_kolejnosc, m2Z_kolejnosc) = z_ktorym;
	tie(m1N, m2N, m1N_operacje, m2N_operacje, m1N_operacje_drugie, m2N_operacje_drugie, m1N_kolejnosc, m2N_kolejnosc) = nowy;
	vector <int> m1_kol_wyb, m2_kol_wyb;
	vector <int> m1_nowa_kolejnosc;
	vector <int> m2_nowa_kolejnosc;
	vector <pair <int, int> > do_sorta1, do_sorta2; // numer, index
	insert_wycinajacy_z_ktorym(m1Z_operacje, m1Z_operacje_drugie, m2Z_operacje, m2Z_operacje_drugie, m1Z_kolejnosc, m2Z_kolejnosc, m1_kol_wyb, m2_kol_wyb);
	insert_wycinajacy(m1W_operacje, m1W_operacje_drugie, m2W_operacje, m2W_operacje_drugie, m1W_kolejnosc, m2W_kolejnosc, m1W_kolejnosc.size() / 2);
	int x;
	do
	{
		if (m1W_operacje.size() != 0)
		{
			do_sorta1.clear();
			for (int i = 0; i < m1W_operacje.size(); i++)
			{
				do_sorta1.push_back(make_pair(m1W_operacje[i].numer, zwroc_indexx(m1_kol_wyb, m1W_operacje[i].numer)));
			}
			sort(do_sorta1.begin(), do_sorta1.end(), klucz_sort);
			x = zwroc_index(m1W_operacje, do_sorta1[0].first);
			m1_nowa_kolejnosc.push_back(x);
			m1W_kolejnosc.erase(m1W_kolejnosc.begin());
			if (m1W_operacje[x].nr_operacji == 1)
			{
				m2W_operacje.push_back(m2W_operacje_drugie[zwroc_index(m2W_operacje_drugie, m1W_operacje[x].numer)]);
				m2W_operacje_drugie.erase(m2W_operacje_drugie.begin() + zwroc_index(m2W_operacje_drugie, m1W_operacje[x].numer));
			}
			m1W_operacje.erase(m1W_operacje.begin() + x);
		}
		
		if (m2W_operacje.size() != 0)
		{
			do_sorta2.clear();
			for (int i = 0; i < m2W_operacje.size(); i++)
			{
				do_sorta2.push_back(make_pair(m2W_operacje[i].numer, zwroc_indexx(m2_kol_wyb, m2W_operacje[i].numer)));
			}
			sort(do_sorta2.begin(), do_sorta2.end(), klucz_sort);
			{
				x = zwroc_index(m2W_operacje, do_sorta2[0].first);
			}
			m2_nowa_kolejnosc.push_back(x);
			m2W_kolejnosc.erase(m2W_kolejnosc.begin());
			if (m2W_operacje[x].nr_operacji == 1)
			{
				m1W_operacje.push_back(m1W_operacje_drugie[zwroc_index(m1W_operacje_drugie, m2W_operacje[x].numer)]);
				m1W_operacje_drugie.erase(m1W_operacje_drugie.begin() + zwroc_index(m1W_operacje_drugie, m2W_operacje[x].numer));
			}
			m2W_operacje.erase(m2W_operacje.begin() + x);
		}
		
	} while (m1W_operacje.size() || m2W_operacje.size());
	m1N_kolejnosc.resize(m1N_kolejnosc.size() / 2);
	m1N_kolejnosc.shrink_to_fit();
	m1N_kolejnosc.insert(m1N_kolejnosc.end(), m1_nowa_kolejnosc.begin(), m1_nowa_kolejnosc.end());
	m2N_kolejnosc.resize(m2N_kolejnosc.size() / 2);
	m2N_kolejnosc.shrink_to_fit();
	m2N_kolejnosc.insert(m2N_kolejnosc.end(), m2_nowa_kolejnosc.begin(), m2_nowa_kolejnosc.end());
	//cout << "OK" << endl;
	nowy = make_tuple(m1N, m2N, m1N_operacje, m2N_operacje, m1N_operacje_drugie, m2N_operacje_drugie, m1N_kolejnosc, m2N_kolejnosc);
	return nowy;
}

vector <tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > > selekcja(vector <tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > >  &inst, int wielkosc_populacji)
// liczba instancji = polowa (+1)
// popraw dla 1-ynki
{
	int player1;
	int player2;
	vector <tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > > nowy;
	if (inst.size() > wielkosc_populacji)
	{
		do
		{
			if (inst.size() <= 1)
			{
				break;
			}
			do
			{
				player1 = rand() % inst.size();
				player2 = rand() % inst.size();
			} while (player1 == player2); // losuje dwa rozne
										  // max szuka dluzszego uporzadkowania wsrod 2 maszyn jednej instancji
										  // instancja majaca mniejszego max'a przechodzi dalej
			if (max(get<0>(inst[player1])[get<0>(inst[player1]).size() - 1].czas_konca, get<1>(inst[player1])[get<1>(inst[player1]).size() - 1].czas_konca) < max(get<0>(inst[player2])[get<0>(inst[player2]).size() - 1].czas_konca, get<1>(inst[player2])[get<1>(inst[player2]).size() - 1].czas_konca))
			{
				nowy.push_back(inst[player1]);	 // player1 przechodzi
				//inst.erase(inst.begin() + player2);
			}
			else
			{
				nowy.push_back(inst[player2]);	 // player2 przechodzi
				//inst.erase(inst.begin() + player1);
			}
			if (player2 > player1)
			{
				inst.erase(inst.begin() + player2);
				inst.erase(inst.begin() + player1);
			}
			else
			{
				inst.erase(inst.begin() + player1);
				inst.erase(inst.begin() + player2);
			}

		} while (inst.size() + nowy.size() > wielkosc_populacji); // jesli zostalo nieparzyscie, to przechodzi
		nowy.insert(nowy.end(), inst.begin(), inst.end());
		if (nowy.size() != wielkosc_populacji)
		{
			return selekcja(nowy, wielkosc_populacji);
		}
		return nowy;
	}
	else
		return inst;
}

void wypelnij_idle(vector <obiekt> &maszyna)
{
	obiekt element;
	element.numer = 1;
	element.czas_instancji = element.czas_z_kara = element.maint_kara = element.nr_operacji = element.czas_startu = 0;
	element.typ = "idle";
	if (maszyna[0].czas_startu != 0) // mogl tam wpasc maint np. w 2 sec
	{
		element.czas_konca = element.czas_trwania = maszyna[0].czas_startu;
		maszyna.insert(maszyna.begin(), element);
		element.numer++;
	}
	for (int i = 1; i < maszyna.size(); i++)
	{
		if (maszyna[i - 1].maint_kara == 0 && maszyna[i].czas_startu > maszyna[i - 1].czas_konca)
		{
			element.czas_startu = maszyna[i - 1].czas_konca;
			element.czas_konca = maszyna[i].czas_startu;
			element.czas_trwania = element.czas_konca - element.czas_startu;
			maszyna.insert(maszyna.begin() + i, element);
			i++;
			element.numer++;
		}
		if (maszyna[i - 1].maint_kara && maszyna[i].czas_startu > maszyna[i - 1].maint_kara)
		{
			element.czas_startu = maszyna[i - 1].maint_kara;
			element.czas_konca = maszyna[i].czas_startu;
			element.czas_trwania = element.czas_konca - element.czas_startu;
			maszyna.insert(maszyna.begin() + i, element);
			i++;
			element.numer++;
		}
	}
}

int main()
{
	srand(time(NULL));
	int licznik = 0;
	string nazwa = "../../Instancje/";
	string nazwa_rozw = "../../Rozwiazania/";
	fstream uchwyt("../../LiczbaInstancji.txt", ios::in);
	fstream par_heur("../../HeurystykaGenerowanie.txt", ios::in);
	fstream wynik;
	vector <obiekt> m1_pierwotne; //maszyna 1
	vector <obiekt> m2_pierwotne; //maszyna 2
	vector <obiekt> m1_operacje_pierwotne, m2_operacje_pierwotne, m1_operacje_drugie_pierwotne, m2_operacje_drugie_pierwotne;
	vector <obiekt> m1; //maszyna 1
	vector <obiekt> m2; //maszyna 2
	vector <obiekt> m1_operacje; //operacje dla m1
	vector <obiekt> m2_operacje; //operacje dla m2
	vector <obiekt> m1_operacje_drugie; //operacje drugie dla m1, uaktywniane (przenoszone do puli do losowania, po wykonaniu operacji 1-szej)
	vector <obiekt> m2_operacje_drugie; //operacje drugie dla m2
	vector <int> m1_kolejnosc; // x w kolejnych losowaniach przy insert_M dla m1
	vector <int> m2_kolejnosc; // x w kolejnych losowaniach przy insert_M dla m2
	tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > krotka;
	vector <tuple < vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <obiekt>, vector <int>, vector <int> > > inst;

	vector <int> Vglobal_procent_mutacji, Vglobal_procent_krzyzowania, Vliczba_iteracji, Vwielkosc_populacji, Vpoczatkowa_populacja;
	while (par_heur >> global_procent_mutacji >> global_procent_krzyzowania >> liczba_iteracji >> wielkosc_populacji >> poczatkowa_populacja)
	{
		Vglobal_procent_mutacji.push_back(global_procent_krzyzowania);
		Vglobal_procent_krzyzowania.push_back(liczba_iteracji);
		Vliczba_iteracji.push_back(liczba_iteracji);
		Vwielkosc_populacji.push_back(wielkosc_populacji);
		Vpoczatkowa_populacja.push_back(poczatkowa_populacja);
	}
	par_heur.close();
	// m1, m2, m1_operacje, m2_operacje, m1_operacje_drugie, m2_operacje_drugie, m1_kolejnosc, m2_kolejnosc
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
	int czas_poczatkowy;
	int suma_idle_M1;
	int suma_maint_M1;
	int suma_idle_M2;
	int suma_maint_M2;
	int co_mutowac;
	double procent_mutacji;
	double procent_krzyzowania;
	int z_czym_krzyzowac;
	uchwyt >> liczba_instancji;
	uchwyt.close();
	for (int i = 0;;)
	{
		if (licznik % 16 == 0) i++;
		if (i > liczba_instancji) break;
		global_procent_krzyzowania = Vglobal_procent_krzyzowania[licznik % 16];
		global_procent_mutacji = Vglobal_procent_mutacji[licznik % 16];
		liczba_iteracji = Vliczba_iteracji[licznik % 16];
		wielkosc_populacji = Vwielkosc_populacji[licznik % 16];
		poczatkowa_populacja = Vpoczatkowa_populacja[licznik % 16];
		cout << i << " / " << liczba_instancji << " ==> " << 1 + licznik % 16 << "\r";
		// global_procent_mutacji przez chwile przechowuje nr_inst, 
		
		uchwyt.open(nazwa + to_string(i) + ".txt", ios::in);
		uchwyt >> liczba_operacji;
		for (int k = 1; k <= liczba_operacji; k++)
		{
			uchwyt >> czas_operacji1 >> czas_operacji2 >> nmdop1 >> nmdop2;
			if (nmdop1 == 1) // operacja pierwsza na pierwszej maszynie
			{
				m1_operacje.emplace_back(k, czas_operacji1, 1);
				m2_operacje_drugie.emplace_back(k, czas_operacji2, 2);
			}
			else
			{
				m1_operacje_drugie.emplace_back(k, czas_operacji2, 2);
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
		m1_pierwotne = m1;
		m2_pierwotne = m2;
		m1_operacje_pierwotne = m1_operacje;
		m1_operacje_drugie_pierwotne = m1_operacje_drugie;
		m2_operacje_pierwotne = m2_operacje;
		m2_operacje_drugie_pierwotne = m2_operacje_drugie;
		//display(m1, "M1", m2, "M2");
		for (int n = 0; n < poczatkowa_populacja; n++)
		{
			m1 = m1_pierwotne;
			m2 = m2_pierwotne;
			m1_operacje = m1_operacje_pierwotne;
			m1_operacje_drugie = m1_operacje_drugie_pierwotne;
			m2_operacje = m2_operacje_pierwotne;
			m2_operacje_drugie = m2_operacje_drugie_pierwotne;
			insert_M(m1_operacje, m1_operacje_drugie, m1, m2_operacje, m2_operacje_drugie, m2, m1_kolejnosc, m2_kolejnosc);
			if (n == 0) // ustawia czas poczatkowy z 1_szej losowej 
			{
				czas_poczatkowy = max(m1[m1.size() - 1].czas_konca, m2[m2.size() - 1].czas_konca);
			}
			krotka = make_tuple(m1, m2, m1_operacje, m2_operacje, m1_operacje_drugie, m2_operacje_drugie, m1_kolejnosc, m2_kolejnosc);
			/*cout <<"-----"<< n << endl;
			display(m1, "M1", m2, "M2");*/
			inst.push_back(krotka); // zapisanie instancji
			m1.clear();
			m2.clear();
			m1_kolejnosc.clear();
			m2_kolejnosc.clear();
		} 
		//srand(time(NULL));
		procent_mutacji = global_procent_mutacji;
		procent_krzyzowania = global_procent_krzyzowania;

		for (int k = 0; k < liczba_iteracji; k++) // liczba iteracji na sztywno
		{
			for (int m = 0; m < inst.size(); m++) // po rozmiarze tablicy instancji
			{
				//cout << k << "\t" << m << endl;
				if (rand() % 100 < procent_krzyzowania)
				{
					// wysyla 2 instancje
					do
					{
						z_czym_krzyzowac = rand() % inst.size();
					} while (m == z_czym_krzyzowac);
					//cout << "m = " << m << "\t z_czym = " << z_czym_krzyzowac << endl;
					tie(ignore, ignore, m1_operacje, m2_operacje, m1_operacje_drugie, m2_operacje_drugie, m1_kolejnosc, m2_kolejnosc) = krzyzowanie(inst[m], inst[z_czym_krzyzowac]);

					m1 = m1_pierwotne;
					m2 = m2_pierwotne;
					insert_kolejnosc(m1_operacje, m1_operacje_drugie, m1, m2_operacje, m2_operacje_drugie, m2, m1_kolejnosc, m2_kolejnosc);
					/*	cout << "Po krzyzowaniu" << endl;
					display(m1, "M1", m2, "M2");*/
					krotka = make_tuple(m1, m2, m1_operacje, m2_operacje, m1_operacje_drugie, m2_operacje_drugie, m1_kolejnosc, m2_kolejnosc);
					inst.push_back(krotka);
				}
				if (rand() % 100 < procent_mutacji)
				{
					tie(ignore, ignore, m1_operacje, m2_operacje, m1_operacje_drugie, m2_operacje_drugie, m1_kolejnosc, m2_kolejnosc) = inst[m];
					co_mutowac = rand() % 2;
					//cout << "co_mutowac = " << co_mutowac << endl;
					switch (co_mutowac)
					{
						case 0:
						{
							mutacja(m1_operacje);
							break;
						}
						case 1:
						{
							mutacja(m2_operacje);
							break;
						}
					}
					m1 = m1_pierwotne;
					m2 = m2_pierwotne;
					insert_kolejnosc(m1_operacje, m1_operacje_drugie, m1, m2_operacje, m2_operacje_drugie, m2, m1_kolejnosc, m2_kolejnosc);
					//cout << "Po mutacji" << endl;
					//display(m1, "M1", m2, "M2");
					krotka = make_tuple(m1, m2, m1_operacje, m2_operacje, m1_operacje_drugie, m2_operacje_drugie, m1_kolejnosc, m2_kolejnosc);
					inst.push_back(krotka); 
				}				
				//cout << k << "\t" << m << "\t" << inst.size() << endl;	
			}
			inst = selekcja(inst, wielkosc_populacji);
			//if (true) // sterowanie procentami
			//{
				procent_mutacji *= 0.99;
				procent_krzyzowania *= 0.99;
			//}	 
		}
		inst = selekcja(inst, 1);
		//w tym momencie musi juz zostac tylko jedna najlepsza instancja
		m1 = get<0>(inst[0]); //znalezienie najlepszego w selekcji ... 
		m2 = get<1>(inst[0]);

		wypelnij_idle(m1);
		wypelnij_idle(m2);
		wynik.open(nazwa_rozw + to_string(i) + "___" + to_string(1 + licznik % 16) + ".txt", ios::out | ios::trunc);
		wynik << max(m1[m1.size() - 1].czas_konca, m2[m2.size() - 1].czas_konca) << ", " << czas_poczatkowy << endl;
		suma_idle_M1 = 0;
		suma_maint_M1 = 0;
		suma_idle_M2 = 0;
		suma_maint_M2 = 0;
		wynik << "M1:";
		for (int k = 0; k < m1.size(); k++)
		{
			if (m1[k].typ == "op")
			{
				wynik << " " << m1[k].typ << m1[k].nr_operacji << "_" << m1[k].numer << ", " << m1[k].czas_startu << ", " << m1[k].czas_instancji << ", " << m1[k].czas_trwania << ";";
			}
			else // maint lub idle
			{
				wynik << " " << m1[k].typ << m1[k].numer << "_M1, " << m1[k].czas_startu << ", " << m1[k].czas_trwania << ";";
				if (m1[k].typ == "maint")
				{
					suma_maint_M1 += m1[k].czas_trwania;
				}
				else
				{
					suma_idle_M1 += m1[k].czas_trwania;
				}
			}
		}
		wynik << endl << "M2:"; // druga maszyna
		for (int k = 0; k < m2.size(); k++)
		{
			if (m2[k].typ == "op")
			{
				wynik << " " << m2[k].typ << m2[k].nr_operacji << "_" << m2[k].numer << ", " << m2[k].czas_startu << ", " << m2[k].czas_instancji << ", " << m2[k].czas_trwania << ";";
			}
			else // maint lub idle
			{
				wynik << " " << m2[k].typ << m2[k].numer << "_M2, " << m2[k].czas_startu << ", " << m2[k].czas_trwania << ";";
				if (m2[k].typ == "maint")
				{
					suma_maint_M2 += m2[k].czas_trwania;
				}
				else
				{
					suma_idle_M2 += m2[k].czas_trwania;
				}
			}
		}
		wynik << endl << suma_maint_M1 << endl << suma_maint_M2 << endl << suma_idle_M1 << endl << suma_idle_M2;
		wynik.close();
		inst.clear();
		m1.clear();
		m2.clear();
		m1_operacje.clear();
		m2_operacje.clear();
		m1_operacje_drugie.clear();
		m2_operacje_drugie.clear();
		m1_kolejnosc.clear();
		m2_kolejnosc.clear();
		licznik++;
	}
}

