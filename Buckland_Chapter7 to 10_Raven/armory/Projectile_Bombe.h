#ifndef BOMBE_H
#define BOMBE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Rocket.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   class to implement a rocket
//
//-----------------------------------------------------------------------------

#include "Raven_Projectile.h"

class Raven_Bot;

class Bombe : public Raven_Projectile
{
private:

  //the radius of damage, once the rocket has impacted
  double    m_dBlastRadius;

  //this is used to render the splash when the rocket impacts
  double    m_dCurrentBlastRadius;

  // sert de minuteur
  clock_t temps;
  
  //If the rocket has impacted we test all bots to see if they are within the 
  //blast radius and reduce their health accordingly
  void InflictDamageOnBotsWithinBlastRadius();

    //tests the trajectory of the shell for an impact
  void TestForImpact();
  
  // Verifie si la grenade doit exploser
  bool TempsEcoule();

public:

  Bombe(Raven_Bot* shooter, Vector2D target);
  
  void Render();

  void Update();
  
};


#endif
