// Bullet.h
// Class for behavior of projectiles
// Code by Derek Bliss
// Created April 17, 2008
// Last updated September 18, 2008
//
#ifndef BULLET_H
#define BULLET_H

#include "MovingObject.h"

class Bullet : public MovingObject
{
	bool tracking;				// Adjust to hit the nearest target
	float damage;				// Damage this bullet causes to something it hits
	float speed;				// Speed of this bullet
	unsigned int animationID;	// Death animation for this bullet
public:
	Bullet();
	virtual ~Bullet();

	// mutators
	void SetTracking(bool _track);
	void SetDamage(float _damage);
	void SetSpeed(float _speed);
	void SetDeathAnimation(unsigned int _animationID);

	// accessors
	bool GetTracking() const;
	float GetDamage() const;
	float GetSpeed() const;
	unsigned int GetDeathAnimation() const;
	
	// interface methods
	virtual void Heartbeat(float _delta);
};

#endif