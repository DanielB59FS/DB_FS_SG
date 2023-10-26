#ifndef	MOTHERSHIP
#define	MOTHERSHIP

#include "Ship.h"

class BaseCommand;

class MotherShip : public Ship {
private:
	std::vector<Ship*> ships;
	std::vector<BaseCommand*> commands;

	void ProcessCommands(float _delta);
	void CommandsCleanUp();

public:
	void AddShadowShip(Ship* const _shadow);

	void Heartbeat(float _delta);

	MotherShip(const Ship& _ship);
	~MotherShip();
};

#endif	// MOTHERSHIP
