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
		if (m[i].numer == numer)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		cout << "error";
	}
	else
	{
		return index;
	}
}

void insert_M(vector <obiekt> &m1_operacje, vector <obiekt> &m1_operacje_drugie, vector <obiekt> &maszyna1, vector <obiekt> &m2_operacje, vector <obiekt> &m2_operacje_drugie, vector <obiekt> &maszyna2)
{
	//trzeba jednoczesnie 2 maszyny obsluzyc, -> przeplatanie sie w do-while
	srand(time(NULL));
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
			if (m1_operacje[x].nr_operacji == 1)
			{
				if (m1_operacje[x].czas_instancji <= maszyna1[0].czas_startu) // trafil w pierwszy przedzial <0, czas startu 1-szego>
				{
					m1_operacje[x].czas_startu = 0;
					m1_operacje[x].czas_konca = m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
					maszyna1.insert(maszyna1.begin(), m1_operacje[x]);
					//cout << "poczatek" << endl;
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
							//cout << "--przedzial---" << endl;
							break;
						}
						else
						{
							// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
							if (m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca
								&& maszyna1.size() > i + 1 && maszyna1[i].typ == "maint" && maszyna1[i].maint_kara == 0
								&& maszyna1[i + 1].czas_startu - maszyna1[i].czas_konca > m1_operacje[x].czas_z_kara - (maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca))
							{
								m1_operacje[x].czas_trwania = m1_operacje[x].czas_z_kara;
								maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
								//cout << "--przedzial-WZNAwiialne---" << endl;
								break;
							}
						}
					}
					if (i == maszyna1.size()) //przelecial i nie zmiescil, wiec dodaje na koniec
					{
						m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
						maszyna1.push_back(m1_operacje[x]);
						//cout << "koniec" << endl;
					}
				}
				m2_operacje.push_back(m2_operacje_drugie[zwroc_index(m2_operacje_drugie, m1_operacje[x].numer)]);
				m2_operacje_drugie.erase(m2_operacje_drugie.begin() + zwroc_index(m2_operacje_drugie, m1_operacje[x].numer));
				m1_operacje.erase(m1_operacje.begin() + x);
				//display(maszyna, "Przed");
				update(maszyna1);
				//display(maszyna, "Pokaz");
			}
			else
			{
				czy = true;
				koniec_pierwszej_op = maszyna2[zwroc_index(maszyna2, m1_operacje[x].numer)].czas_konca; // trzeba poprawnie to ustawic
																						 //cout << " -- " << x + 1 << "    " << koniec_na_pierwszej << endl;
				if (koniec_pierwszej_op <= maszyna1[0].czas_startu && m1_operacje[x].czas_instancji <= maszyna1[0].czas_startu)
				{
					m1_operacje[x].czas_startu = koniec_pierwszej_op;
					m1_operacje[x].czas_trwania = m1_operacje[x].czas_instancji;
					m1_operacje[x].czas_konca = m1_operacje[x].czas_startu + m1_operacje[x].czas_trwania;
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
							//cout << " -- " << x + 1 << "    " << koniec_pierwszej_op << endl;
							//cout << "--przedzial---" << endl;
							break;
						}
						else
						{
							// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
							if (koniec_pierwszej_op <= maszyna1[i - 1].czas_konca && m1_operacje[x].czas_z_kara - m1_operacje[x].czas_instancji < maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca
								&& maszyna1.size() > i + 1 && maszyna1[i].typ == "maint" && maszyna1[i].maint_kara == 0
								&& maszyna1[i + 1].czas_startu - maszyna1[i].czas_konca > m1_operacje[x].czas_z_kara - (maszyna1[i].czas_startu - maszyna1[i - 1].czas_konca))
							{
								m1_operacje[x].czas_trwania = m1_operacje[x].czas_z_kara;
								maszyna1.insert(maszyna1.begin() + i, m1_operacje[x]);
								czy = false;
								//cout << "--przedzial-WZNAwiialne---" << endl;
								break;
							}
						}
					}
					if (i == maszyna1.size()) //przelecial i nie zmiescil, wiec dodaje na koniec
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
							//m1_operacje[x].czas_konca = m1_operacje[x].czas_startu + m1_operacje[x].czas_trwania;
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
			x = rand() % m2_operacje.size(); //losowy z losowego generatora
			if (m2_operacje[x].nr_operacji == 1)
			{
				if (m2_operacje[x].czas_instancji <= maszyna2[0].czas_startu) // trafil w pierwszy przedzial <0, czas startu 1-szego>
				{
					m2_operacje[x].czas_startu = 0;
					m2_operacje[x].czas_konca = m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
					maszyna2.insert(maszyna2.begin(), m2_operacje[x]);
					//cout << "poczatek" << endl;
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
							//cout << "--przedzial---" << endl;
							break;
						}
						else
						{
							// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
							if (m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca
								&& maszyna2.size() > i + 1 && maszyna2[i].typ == "maint" && maszyna2[i].maint_kara == 0
								&& maszyna2[i + 1].czas_startu - maszyna2[i].czas_konca > m2_operacje[x].czas_z_kara - (maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca))
							{
								m2_operacje[x].czas_trwania = m2_operacje[x].czas_z_kara;
								maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
								//cout << "--przedzial-WZNAwiialne---" << endl;
								break;
							}
						}
					}
					if (i == maszyna2.size()) //przelecial i nie zmiescil, wiec dodaje na koniec
					{
						m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
						maszyna2.push_back(m2_operacje[x]);
						//cout << "koniec" << endl;
					}
				}
				m1_operacje.push_back(m1_operacje_drugie[zwroc_index(m1_operacje_drugie, m2_operacje[x].numer)]);
				m1_operacje_drugie.erase(m1_operacje_drugie.begin() + zwroc_index(m1_operacje_drugie, m2_operacje[x].numer));
				m2_operacje.erase(m2_operacje.begin() + x);
				//display(maszyna, "Przed");
				update(maszyna2);
				//display(maszyna, "Pokaz");
			}
			else
			{
				czy = true;
				koniec_pierwszej_op = maszyna1[zwroc_index(maszyna1, m2_operacje[x].numer)].czas_konca; // trzeba poprawnie to ustawic
																						 //cout << " -- " << x + 1 << "    " << koniec_na_pierwszej << endl;
				if (koniec_pierwszej_op <= maszyna2[0].czas_startu && m2_operacje[x].czas_instancji <= maszyna2[0].czas_startu)
				{
					m2_operacje[x].czas_startu = koniec_pierwszej_op;
					m2_operacje[x].czas_trwania = m2_operacje[x].czas_instancji;
					m2_operacje[x].czas_konca = m2_operacje[x].czas_startu + m2_operacje[x].czas_trwania;
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
							//cout << " -- " << x + 1 << "    " << koniec_pierwszej_op << endl;
							//cout << "--przedzial---" << endl;
							break;
						}
						else
						{
							// roznica czasow (kara - normalny) bedzie mniejsza, niz wolne miejsce, wiec skonczy wczesniej
							if (koniec_pierwszej_op <= maszyna2[i - 1].czas_konca && m2_operacje[x].czas_z_kara - m2_operacje[x].czas_instancji < maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca
								&& maszyna2.size() > i + 1 && maszyna2[i].typ == "maint" && maszyna2[i].maint_kara == 0
								&& maszyna2[i + 1].czas_startu - maszyna2[i].czas_konca > m2_operacje[x].czas_z_kara - (maszyna2[i].czas_startu - maszyna2[i - 1].czas_konca))
							{
								m2_operacje[x].czas_trwania = m2_operacje[x].czas_z_kara;
								maszyna2.insert(maszyna2.begin() + i, m2_operacje[x]);
								czy = false;
								//cout << "--przedzial-WZNAwiialne---" << endl;
								break;
							}
						}
					}
					if (i == maszyna2.size()) //przelecial i nie zmiescil, wiec dodaje na koniec
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
							//m2_operacje[x].czas_konca = m2_operacje[x].czas_startu + m2_operacje[x].czas_trwania;
							m2_operacje[x].ruszaj = false;
							maszyna2.push_back(m2_operacje[x]);
							czy = false;
						}
						
						//cout << "koniec" << endl;
						//warunek aby dodalo, po 
					}

				}
				if (czy == false)
				{
					m2_operacje.erase(m2_operacje.begin() + x);
					update(maszyna2);
				}
			}
		}
		
		//display(maszyna1, "M1", maszyna2, "M2");
	} while (m2_operacje.size() != 0 || m1_operacje.size() != 0 || m2_operacje_drugie.size() != 0 || m1_operacje_drugie.size() != 0);
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
		koniec_na_pierwszej = pierwsza[zwroc_index(pierwsza, x + 1)].czas_konca; // trzeba poprawnie to ustawic
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
	vector <obiekt> m1_operacje_drugie; //operacje drugie dla m1, uaktywniane (przenoszone do puli do losowania, po wykonaniu operacji 1-szej)
	vector <obiekt> m2_operacje_drugie; //operacje drugie dla m2
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
		display(m1, "M1", m2, "M2");
		//insert_m1(m1_operacje, m1);	
		//insert_m2(m2_operacje, m2, m1);
		insert_M(m1_operacje, m1_operacje_drugie, m1, m2_operacje, m2_operacje_drugie, m2);
		display(m1, "M1", m2, "M2");
		m1.clear();
		m2.clear();
	}
}

