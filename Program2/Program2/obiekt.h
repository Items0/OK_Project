#pragma once

#include <string>
using namespace std;
class obiekt
{
public:
	int numer;
	int czas_startu;
	int czas_trwania;
	int czas_konca;
	int czas_instancji; // only op
	int czas_z_kara; //only op
	
	string typ; //  {op, maint, idle} a'la enum
	
	obiekt(int numer, int czas_instancji);
	obiekt(int numer, int czas_trwania, int czas_startu);
	~obiekt();
};

