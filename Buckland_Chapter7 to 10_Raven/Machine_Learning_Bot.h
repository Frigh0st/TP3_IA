#ifndef MACHINE_LEARNING_BOT_H
#define MACHINE_LEARNING_BOT_H

#include "Raven_Bot.h"
#include "Raven_Game.h"
#include "CData.h"
#include "CNeuralNet.h"

class Machine_Learning_Bot : public Raven_Bot
{
private:
	CNeuralNet m_ModeleAppris;

public:

	Machine_Learning_Bot(Raven_Game* world, Vector2D pos);
	~Machine_Learning_Bot();


	void Update();


};



#endif