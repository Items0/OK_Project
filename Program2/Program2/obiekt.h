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
	int maint_kara; //only maint, czas konca poprzedniego wznawianego zadania
	
	string typ; //  {op, maint, idle} a'la enum
	obiekt();
	obiekt(int numer, int czas_instancji);
	obiekt(int numer, int czas_trwania, int czas_startu);
	~obiekt();
};

