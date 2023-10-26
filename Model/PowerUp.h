// PowerUp.h
// PowerUp class for distinguishing different power up items
// Code by Derek Bliss
// Created April 17, 2008
// Last updated September 24, 2008
//
#ifndef POWERUP_H
#define POWERUP_H

#include "MovingObject.h"

class PowerUp : public MovingObject
{
	bool flicker;		// Boolean used for flicker effect
	int type;			// A powerup can be one of four types (check enum below)
	float timer;		// Internal life timer; no accessor/mutator needed

public:

	PowerUp();
	PowerUp(int _type);
	virtual ~PowerUp();

	enum TYPE { SCORE, ENERGY, ARMOR, WEAPON, NUMTYPES };
	
	// mutators
	void SetType(int _type);

	// accessors
	int GetType() const;

	// interface
	virtual void Heartbeat(float _delta);
};

#endif