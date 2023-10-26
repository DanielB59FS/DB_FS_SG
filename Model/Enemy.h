// Enemy.h
// Enemy class for managing enemy data and functionality
// Code by Derek Bliss
// Created April 17, 2008
// Last updated September 18, 2008
// 
#ifndef ENEMY_H
#define ENEMY_H

#include "Subject.h"

class Ship;

class Enemy : public Subject
{
	bool tempBehavior;				// true = temporary behavior is in use, false = initial behavior
	bool fireReady;					// The enemy is ready to fire
	bool retargetReady;				// The enemy is ready to re-acquire the target
	bool joust;						// Toggle between follow and flee movement behavior
	bool initialJoust;
	int tempBehaviorID;				// Integer value to indentify the temporary behavior the enemy is
									// is currently using
	float fireFrequency;			// Inclination to fire 1.0 == 100% (all the time)
	float retargetTime;				// Time between target checks
	float retargetTimer;			// Amount of time since last target check
	float minDistance;				// Minimum distance from target
	float maxDistance;				// Maximum distance from target
	unsigned int movementBehavior;	// NONE		(0) == no behavior
									// FOLLOW	(1) == follow target until min distance reached
									// FLEE		(2) == flee target until max distance reached
	unsigned int initialMovementBehavior;
	unsigned int weaponLevel;		// Level of current weapon
	unsigned int score;				// Amount of points granted to the player upon defeat
	unsigned int powerupID;			// Type of powerup(s) released upon defeat
	unsigned int powerupAmount;		// Number of powerups to release

	// flyweight data
	unsigned int shipID;
	unsigned int weaponID;
	unsigned int shieldID;
	unsigned int generatorID;
	unsigned int propulsionID;

	Vec2f target;					// Target to track when following/fleeing

	Ship *ship;						// Craft enemy will use to combat the player

	// helper methods

	// Use the following method to temporarily change this enemy's behavior. Be sure to use the
	// ResetBehavior method when the enemy needs to revert back to its normal behavior and use the
	// _tempBehaviorID parameter to ensure that the behavior is not reverted prematurely.
	void ChangeBehaviorTemporarily(int _tempBehaviorID, unsigned int _movementBehavior, const Vec2f &_target);
	// The following method reverts behavior back to what it was before using ChangeBehaviorTemporarily
	void ResetBehavior(int _tempBehaviorID);
public:

	enum MOVEMENT {NONE, FOLLOW, FLEE};
	enum POWERUP {SCORE, ENERGY, ARMOR, WEAPON};

	Enemy();
	virtual ~Enemy();

	// mutators
	virtual void SetFireReady(bool _fire);
	virtual void SetRetargetReady(bool _retarget);
	virtual void SetJoust(bool _joust);
	virtual void SetRetargetTime(float _time);
	virtual void SetFireFrequency(float _frequency);
	virtual void SetMinDistance(float _min);
	virtual void SetMaxDistance(float _max);
	virtual void SetMovementBehavior(unsigned int _behavior);
	virtual void SetWeaponLevel(unsigned int _level);
	virtual void SetScore(unsigned int _score);
	virtual void SetPowerUpID(unsigned int _id);
	virtual void SetPowerUpAmount(unsigned int _amount);
	virtual void SetShipID(unsigned int _id);
	virtual void SetWeaponID(unsigned int _id);
	virtual void SetShieldID(unsigned int _id);
	virtual void SetGeneratorID(unsigned int _id);
	virtual void SetPropulsionID(unsigned int _id);
	virtual void SetTarget(const Vec2f &_target);
	virtual void SetShip(Ship *_ship);

	// accessors
	virtual bool GetFireReady() const;
	virtual bool GetRetargetReady() const;
	virtual bool GetJoust() const;
	virtual float GetFireFrequency() const;
	virtual float GetMinDistance() const;
	virtual float GetMaxDistance() const;
	virtual unsigned int GetMovementBehavior() const;
	virtual unsigned int GetWeaponLevel() const;
	virtual unsigned int GetScore() const;
	virtual unsigned int GetPowerUpID() const;
	virtual unsigned int GetPowerUpAmount() const;
	virtual unsigned int GetShipID() const;
	virtual unsigned int GetWeaponID() const;
	virtual unsigned int GetShieldID() const;
	virtual unsigned int GetGeneratorID() const;
	virtual unsigned int GetPropulsionID() const;
	virtual const Vec2f &GetTarget() const;
	virtual Ship *GetShip() const;

	// interface methods
	virtual void Heartbeat(float _delta);
	virtual Enemy *Clone() const;
};

#endif