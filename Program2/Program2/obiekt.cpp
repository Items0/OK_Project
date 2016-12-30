#include "stdafx.h"
#include "obiekt.h"
#include <cmath>


obiekt::obiekt()
{
}

obiekt::obiekt(int numer, int czas_instancji)
{
	this->numer = numer;
	this->czas_instancji = czas_instancji;
	this->czas_z_kara = ceil(1.25 * czas_instancji);
	this->typ = "op";
	this->maint_kara = 0;
	// czas_trwania i czas_konca - trzeba wybrac
}

obiekt::obiekt(int numer, int czas_trwania, int czas_startu)
{
	this->numer = numer;
	this->czas_trwania = czas_trwania;
	this->typ = "maint";
	this->czas_startu = czas_startu;
	this->czas_konca = czas_startu + czas_trwania;
	this->czas_z_kara = 0;
	this->czas_instancji = 0;
	this->maint_kara = 0;
}

obiekt::~obiekt()
{
}
