#include "Weapon_Grenade.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "misc/utils.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
Grenade::Grenade(Raven_Bot* owner) :

	Raven_Weapon(type_grenade,
		script->GetInt("Grenade_DefaultRounds"),
		script->GetInt("Grenade_MaxRoundsCarried"),
		script->GetDouble("Grenade_FiringFreq"),
		script->GetDouble("Grenade_IdealRange"),
		script->GetDouble("Rocket_MaxSpeed"),
		owner)

{

	//setup the vertex buffer
	const int NumWeaponVerts = 8;
	const Vector2D weapon[NumWeaponVerts] = { Vector2D(0, 0),
											 Vector2D(0, -2),
											 Vector2D(10, -2),
											 Vector2D(10, 0),
											 Vector2D(0, 0),
											 Vector2D(0, 2),
											 Vector2D(10, 2),
											 Vector2D(10, 0)
	};


	for (int vtx = 0; vtx < NumWeaponVerts; ++vtx)
	{
		m_vecWeaponVB.push_back(weapon[vtx]);
	}

	//setup the fuzzy module
	InitializeFuzzyModule();

}

//------------------------------ ShootAt --------------------------------------

inline void Grenade::ShootAt(Vector2D pos)
{
	if (NumRoundsRemaining() > 0 && isReadyForNextShot())
	{
		//fire off a rocket!
		m_pOwner->GetWorld()->AddBombe(m_pOwner, pos);

		m_iNumRoundsLeft--;

		UpdateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("Grenade_SoundRange"));
	}
}

//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
inline double Grenade::GetDesirability(double DistToTarget)
{
	if (m_iNumRoundsLeft == 0)
	{
		m_dLastDesirabilityScore = 0;
	}
	else
	{
		//fuzzify distance and amount of ammo
		m_FuzzyModule.Fuzzify("DistanceToTarget", DistToTarget);
		m_FuzzyModule.Fuzzify("AmmoStatus", (double)m_iNumRoundsLeft);

		m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);
	}

	return m_dLastDesirabilityScore;
}

//--------------------------- InitializeFuzzyModule ---------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void Grenade::InitializeFuzzyModule()
{
	FuzzyVariable& DistanceToTarget = m_FuzzyModule.CreateFLV("DistanceToTarget");

	FzSet& Target_Close = DistanceToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
	FzSet& Target_Medium = DistanceToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
	FzSet& Target_Far = DistanceToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");

	FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
	FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

	FuzzyVariable& AmmoStatus = m_FuzzyModule.CreateFLV("AmmoStatus");
	FzSet& Ammo_Loads = AmmoStatus.AddRightShoulderSet("Ammo_Loads", 30, 60, 100);
	FzSet& Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 0, 30, 60);
	FzSet& Ammo_Low = AmmoStatus.AddTriangularSet("Ammo_Low", 0, 0, 30);


	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Loads), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Okay), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Low), VeryDesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Loads), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Okay), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Low), Undesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Loads), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Okay), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Low), Undesirable);
}

//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void Grenade::Render()
{
	m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
		m_pOwner->Pos(),
		m_pOwner->Facing(),
		m_pOwner->Facing().Perp(),
		m_pOwner->Scale());

	gdi->PinkPen();

	gdi->ClosedShape(m_vecWeaponVBTrans);

}
