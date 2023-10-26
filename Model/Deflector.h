#pragma once
#include "BaseObject.h"

class LevelState;
class Ship;
class IDeflectionStrategy;
class Bullet;

class Deflector :
	public BaseObject
{
	const float consumptionRate;
	int strategyNumber;
	Ship *ship;
	LevelState *state;
	HINSTANCE dll;
	IDeflectionStrategy *deflectionStrategy;

public:
	Deflector(Ship *_ship, LevelState *ls);
	~Deflector(void);

	void Deflect(Bullet *projectile);
	void Heartbeat(float _delta);
	void ActivateStrategy(const char *DLL_filename);
	void DeactivateStrategy();
};
