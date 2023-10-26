#include "pch.h"
#include "Deflector.h"
#include "Ship.h"
#include "Bullet.h"
#include "../View/ViewManager.h"
#include "../LevelState.h"
#include "IDeflectionStrategy.h"

/* notes
deflection shield
-	stores size and position
-	drains energy
-	moves with player
-	activate - turns dll on and off
-	queries for position
levelstate
-	require flipping the collision call to ship version
ship
-	override collide to deflect first
*/

Deflector::Deflector(Ship *_ship, LevelState *ls) : 
	ship(_ship), state(ls), 
	consumptionRate(.5f), strategyNumber(0),
	deflectionStrategy(nullptr), dll(nullptr)
{	
	SetActive(true);
	SetDimensions(ship->GetDimensions()*2);
	SetTexture("Resources\\Images\\emptyroundedbox.png");
	SetPosition(ship->GetPosition());
	ViewManager::GetInstance().AddObject(this, 1);
}

void Deflector::DeactivateStrategy()
{
	if(deflectionStrategy)
	{
		deflectionStrategy->Destroy();
		deflectionStrategy = nullptr;
	}
	if(dll)
	{
		FreeLibrary(dll);
		dll = nullptr;
	}
}

Deflector::~Deflector(void)
{
	DeactivateStrategy();
	ViewManager::GetInstance().RemoveObject(this);
}

void Deflector::Deflect(Bullet *_projectile)
{
	// consume power
	ship->SetBattery(ship->GetBattery() - consumptionRate);

	// flip the projectile back at the enemy
	float heading = _projectile->GetHeading();
	if(deflectionStrategy)
		deflectionStrategy->Deflect(heading);
	else
		heading += 1;
	_projectile->SetHeading(heading);

	// ***********************************************************************
	// requires friend relationship
	std::vector<Bullet *>		&playerBullets = state->playerBullets;
	std::vector<Bullet *>		&enemyBullets = state->enemyBullets;
	// ***********************************************************************

	for(unsigned int loop=0;loop<enemyBullets.size();)
	{
		if(enemyBullets[loop] == _projectile)
		{
			// switch which container it is in
			playerBullets.push_back(enemyBullets[loop]);
			enemyBullets.erase(enemyBullets.begin() + loop);
			return;
		}
		else ++loop;
	}
}

void Deflector::Heartbeat(float _delta)
{
	// check if we still have enough power to deflect
	float energyLeft = ship->GetBattery();
	if(energyLeft < consumptionRate)
		SetActive(false);
	else
		SetPosition(ship->GetPosition());
}

typedef IDeflectionStrategy* (*sptr)();
void Deflector::ActivateStrategy(const char *DLL_filename)
{
	char str[255];

	// unload the old strategy & dll if there is one
	DeactivateStrategy();

	// try to load the library
	dll = LoadLibrary(DLL_filename);

	// failure
	if(!dll)
	{
		sprintf(str, "****** Unable to load DLL: %s ******\n", DLL_filename);
		OutputDebugString(str);
		return;
	}

	// success
	sptr p = reinterpret_cast<sptr>(GetProcAddress(dll, "CreateStrategy"));
	if(p)
		deflectionStrategy = p();
	else
	{
		sprintf(str, "*** Unable to find CreateStrategy function in %s ***\n", DLL_filename);
		OutputDebugString(str);
	}
}