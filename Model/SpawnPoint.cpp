#include "pch.h"
#include "../Model/SpawnPoint.h"
#include "../Model/Enemy.h"
#include "../Model/Ship.h"

namespace
{
	RandomFloat r(0,600);
}

SpawnPoint::SpawnPoint()
{
	waitTime = spawnTime = spawnTimer = 0;
	enemyID = enemiesRemaining = 0;
}

SpawnPoint::SpawnPoint(float _waitTime, float _spawnTime, unsigned int _enemyID, unsigned int _numEnemies, const Vec2f &_position)
: waitTime(_waitTime)
, spawnTime(_spawnTime)
, enemyID(_enemyID)
, enemiesRemaining(_numEnemies)
, position(_position)
{
	spawnTimer = 0;
}

SpawnPoint::~SpawnPoint()
{
}

// accessors
float SpawnPoint::GetWaitTime() const {return waitTime;}
unsigned int SpawnPoint::GetEnemiesRemaining() const {return enemiesRemaining;}

// mutators
void SpawnPoint::SetWaitTime(float _time) {waitTime = _time;}

Enemy *SpawnPoint::Spawn(float _delta)
{
	Enemy *ret = nullptr;
	// Only spawn if there is no longer
	// any time to wait and there are 
	// enemies left to spawn
	if(waitTime <= 0 && enemiesRemaining)
	{
		if(spawnTimer < spawnTime)
			spawnTimer += _delta;
		else
		{
			spawnTimer = 0;
			Enemy *e = SaiphApp::CloneEnemyFromPrototype(enemyID);
			e->GetShip()->SetPosition(position);
			e->SetTarget(Vec2f(r.GenerateValue(), r.GenerateValue()));
			e->GetShip()->SetShield(SaiphApp::GetShieldFlyweight(e->GetShieldID()).strength);
			e->GetShip()->SetBattery(SaiphApp::GetGeneratorFlyweight(e->GetGeneratorID()).battery);
			ret = e;
			enemiesRemaining--;
		}
	}

	return ret;
}