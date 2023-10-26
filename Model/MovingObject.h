// MovingObject.h
// MovingObject class for moving game entities
// Code by Nick Penney and Derek Bliss
// Created October 20, 2006
// Last Updated July 29, 2008
// 
#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include "BaseObject.h"

class MovingObject : public BaseObject
{
	Vec2f velocity;					//	Vector for object movement

public:
	MovingObject();
	virtual ~MovingObject();

	// mutators
	void SetVelocity(const Vec2f &_velocity);
	void SetVelocity(float _xVelocity, float _yVelocity);
	void SetXVelocity(float _xVelocity);
	void SetYVelocity(float _yVelocity);

	// accessors
	const Vec2f &GetVelocity() const;
	float GetXVelocity() const;
	float GetYVelocity() const;
	void ClearForces();
	
	// interface methods
	virtual void Heartbeat(float _delta);				// update to add movement
};

#endif