#include "pch.h"
#include "../Model/PowerUp.h"
#include "../View/ViewManager.h"

#define POWER_UP_LIFETIME	8.0f
#define FLICKER_TIME		3.0f

PowerUp::PowerUp()
{
	SetType(0);
	timer = POWER_UP_LIFETIME;
	flicker = false;
}

PowerUp::PowerUp(int _type)
{
	SetType(0);
	SetType(_type);
	timer = POWER_UP_LIFETIME;
	flicker = false;
}

PowerUp::~PowerUp() {}

// mutators
void PowerUp::SetType(int _type)
{
	// validate type before assignment
	if(_type >= SCORE && _type < NUMTYPES)
		type = _type;
}

// accessors
int PowerUp::GetType() const {return type;}

// interface
void PowerUp::Heartbeat(float _delta)
{
	// invoke parent method
	MovingObject::Heartbeat(_delta);

	// update internal timer
	timer -= _delta;

	if(timer <= 0)
		SetActive(false);
	else if(timer < FLICKER_TIME)
	{
		flicker = !flicker;
		if(flicker)
			ViewManager::GetInstance().RemoveObject(this);
		else
			ViewManager::GetInstance().AddObject(this, 1);
	}

	// bounds check
	if(GetXPosition() < 0)
	{
		SetXPosition(0);
		if(GetXVelocity() < 0)
			SetXVelocity(-GetXVelocity());
	}
	if(GetXPosition() > BOUNDS_SIZE)
	{
		SetXPosition(BOUNDS_SIZE);
		if(GetXVelocity() > 0)
			SetXVelocity(-GetXVelocity());
	}
	if(GetYPosition() < 0)
	{
		SetYPosition(0);
		if(GetYVelocity() < 0)
			SetYVelocity(-GetYVelocity());
	}
	if(GetYPosition() > BOUNDS_SIZE)
	{
		SetYPosition(BOUNDS_SIZE);
		if(GetYVelocity() > 0)
			SetYVelocity(-GetYVelocity());
	}
}