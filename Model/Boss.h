#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"

class Boss : public Enemy
{
	float spazTime;
	unsigned int currentWaypoint;
	std::vector<Vec2f> waypoints;
public:
	Boss();
	virtual ~Boss();

	// interface
	void AddWaypoint(const Vec2f &_waypoint);
	void Heartbeat(float _delta);
};

#endif