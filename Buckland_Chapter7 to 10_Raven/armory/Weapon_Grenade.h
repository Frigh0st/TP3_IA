#ifndef GRENADE_H
#define GRENADE_H
#pragma warning (disable:4786)

#include "Raven_Weapon.h"


class  Raven_Bot;



class Grenade : public Raven_Weapon
{
private:

	void     InitializeFuzzyModule();

	//how much shot the each shell contains
	int      m_iNumBallsInShell;

	//how much the shot spreads out when a cartridge is discharged
	double    m_dSpread;

public:

	Grenade(Raven_Bot* owner);

	void  Render();

	void  ShootAt(Vector2D pos);

	double GetDesirability(double DistToTarget);
};



#endif