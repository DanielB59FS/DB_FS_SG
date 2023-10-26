#include "pch.h"
#include "../Model/Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

// mutators
void Bullet::SetTracking(bool _track) {tracking = _track;}
void Bullet::SetDamage(float _damage) {damage = _damage;}
void Bullet::SetSpeed(float _speed) {speed = _speed;}
void Bullet::SetDeathAnimation(unsigned int _animationID) {animationID = _animationID;}

// accessors
bool Bullet::GetTracking() const {return tracking;}
float Bullet::GetDamage() const {return damage;}
float Bullet::GetSpeed() const {return speed;}
unsigned int Bullet::GetDeathAnimation() const {return animationID;}

// interface methods
void Bullet::Heartbeat(float _delta)
{
	// bullets always move forward
	SetVelocity(SaiphApp::GetUnitVectorFromHeading(GetHeading()) * speed);

	MovingObject::Heartbeat(_delta);

	// If the bullet ventures off the screen, respond
	// as if it is a collision with any other object
	if (GetYPosition() >= BOUNDS_SIZE * 1.125f || GetYPosition() <= -BOUNDS_SIZE * 0.125f ||
		GetXPosition() >= BOUNDS_SIZE * 1.125f || GetXPosition() <= -BOUNDS_SIZE * 0.125f)
		CollisionResponse();
}