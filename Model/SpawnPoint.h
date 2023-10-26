// SpawnPoint.h
// SpawnPoint class for creating enemy objects at a specified location
// Code by Derek Bliss
// Created June 2, 2008
// Last updated August 28, 2008
// 
#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

class Enemy;

class SpawnPoint
{
	float waitTime;						// Time to wait before spawning enemies
	float spawnTimer;					// Time elapsed since last spawn
	float spawnTime;					// Time to wait between each spawn (in seconds)
	unsigned int enemyID;				// ID of enemy prototype for spawning
	unsigned int enemiesRemaining;		// Total number of enemies left to spawn;
	Vec2f position;						// Position to spawn enemies at
public:
	SpawnPoint();
	SpawnPoint(float _waitTime, float _spawnTime, unsigned int _enemyID, unsigned int _numEnemies, const Vec2f &_position);
	virtual ~SpawnPoint();

	// accessors
	float GetWaitTime(void) const;
	unsigned int GetEnemiesRemaining(void) const;

	// mutators
	void SetWaitTime(float _time);

	// interface methods
	virtual Enemy *Spawn(float _delta);
};

#endif