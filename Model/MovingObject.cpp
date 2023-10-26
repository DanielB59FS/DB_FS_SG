#include "pch.h"
#include "../Model/MovingObject.h"

MovingObject::MovingObject()
{
}

MovingObject::~MovingObject()
{
}

// mutators
void MovingObject::SetVelocity(const Vec2f &_velocity) {velocity = _velocity;}
void MovingObject::SetVelocity(float _xVelocity, float _yVelocity)
{
	velocity.x = _xVelocity;
	velocity.y = _yVelocity;
}
void MovingObject::SetXVelocity(float _xVelocity) {velocity.x = _xVelocity;}
void MovingObject::SetYVelocity(float _yVelocity) {velocity.y = _yVelocity;}

// accessors
const Vec2f &MovingObject::GetVelocity() const {return velocity;}
float MovingObject::GetXVelocity() const { return velocity.x; }
float MovingObject::GetYVelocity() const { return velocity.y; }

// interface methods
void MovingObject::Heartbeat(float _delta)
{
	// move object
	float xPos = GetXPosition() + GetXVelocity() * _delta;
	float yPos = GetYPosition() + GetYVelocity() * _delta;

	SetPosition(xPos, yPos);
}