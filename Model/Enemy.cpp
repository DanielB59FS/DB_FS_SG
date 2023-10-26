#include "pch.h"
#include "../Model/Enemy.h"
#include "../Model/Ship.h"
#include "../View/ViewManager.h"

Enemy::Enemy()
{
	SetFireReady(false);
	SetRetargetReady(false);
	SetJoust(false);
	SetFireFrequency(0.5f);
	SetRetargetTime(2.0f);
	retargetTimer = 0;
	SetMinDistance(0);
	SetMaxDistance(BOUNDS_SIZE);
	SetMovementBehavior(NONE);
	SetWeaponLevel(0);
	SetScore(0);
	SetPowerUpID(0);
	SetPowerUpAmount(0);
	SetShipID(0);
	SetWeaponID(0);
	SetShieldID(0);
	SetGeneratorID(0);
	SetPropulsionID(0);
	SetShip(nullptr);
	tempBehavior = false;
}

Enemy::~Enemy()
{
	if(ship)
	{
		ViewManager::GetInstance().RemoveObject(ship);
		delete ship;
	}
}

// helper methods
void Enemy::ChangeBehaviorTemporarily(int _tempBehaviorID, unsigned int _movementBehavior, const Vec2f &_target)
{
	if(!tempBehavior) // only change behavior if we haven't already
	{
		tempBehaviorID = _tempBehaviorID;
		tempBehavior = true;
		SetRetargetReady(false);
		initialMovementBehavior = GetMovementBehavior();
		initialJoust = GetJoust();
		SetMovementBehavior(_movementBehavior);
		SetJoust(false);
		SetTarget(_target);
	}
	else if(tempBehavior && tempBehaviorID == _tempBehaviorID && (target.x != _target.x || target.y != _target.y))
	{
		ResetBehavior(_tempBehaviorID);
		ChangeBehaviorTemporarily(_tempBehaviorID, _movementBehavior, _target);
	}
}
void Enemy::ResetBehavior(int _tempBehaviorID)
{
	// only reset behavior if we are using a specific temporary behavior
	if(tempBehavior && tempBehaviorID == _tempBehaviorID)
	{
		tempBehavior = false;
		SetMovementBehavior(initialMovementBehavior);
		SetJoust(initialJoust);
		SetRetargetReady(true);
	}
}

// mutators
void Enemy::SetFireReady(bool _fire) {fireReady = _fire;}
void Enemy::SetRetargetReady(bool _retarget) {retargetReady = _retarget;}
void Enemy::SetRetargetTime(float _time) {retargetTime = _time;}
void Enemy::SetJoust(bool _joust) {joust = _joust;}
void Enemy::SetFireFrequency(float _frequency) {fireFrequency = _frequency;}
void Enemy::SetMinDistance(float _min) {minDistance = _min;}
void Enemy::SetMaxDistance(float _max) {maxDistance = _max;}
void Enemy::SetMovementBehavior(unsigned int _behavior) {movementBehavior = _behavior;}
void Enemy::SetWeaponLevel(unsigned int _level) {weaponLevel = _level;}
void Enemy::SetScore(unsigned int _score) {score = _score;}
void Enemy::SetPowerUpID(unsigned int _id) {powerupID = _id;}
void Enemy::SetPowerUpAmount(unsigned int _amount) {powerupAmount = _amount;}
void Enemy::SetShipID(unsigned int _id) {shipID = _id;}
void Enemy::SetWeaponID(unsigned int _id) {weaponID = _id;}
void Enemy::SetShieldID(unsigned int _id) {shieldID = _id;}
void Enemy::SetGeneratorID(unsigned int _id) {generatorID = _id;}
void Enemy::SetPropulsionID(unsigned int _id) {propulsionID = _id;}
void Enemy::SetTarget(const Vec2f &_target) {target = _target;}
void Enemy::SetShip(Ship *_ship) {ship = _ship;}

// accessors
bool Enemy::GetFireReady() const {return fireReady; }
bool Enemy::GetRetargetReady() const {return retargetReady;}
bool Enemy::GetJoust() const {return joust;}
float Enemy::GetFireFrequency() const {return fireFrequency;}
float Enemy::GetMinDistance() const {return minDistance;}
float Enemy::GetMaxDistance() const {return maxDistance;}
unsigned int Enemy::GetMovementBehavior() const {return movementBehavior;}
unsigned int Enemy::GetWeaponLevel() const {return weaponLevel;}
unsigned int Enemy::GetScore() const {return score;}
unsigned int Enemy::GetPowerUpID() const {return powerupID;}
unsigned int Enemy::GetPowerUpAmount() const {return powerupAmount;}
unsigned int Enemy::GetShipID() const {return shipID;}
unsigned int Enemy::GetWeaponID() const {return weaponID;}
unsigned int Enemy::GetShieldID() const {return shieldID;}
unsigned int Enemy::GetGeneratorID() const {return generatorID;}
unsigned int Enemy::GetPropulsionID() const {return propulsionID;}
const Vec2f &Enemy::GetTarget() const {return target;}
Ship *Enemy::GetShip() const {return ship;}

// interface methods
void Enemy::Heartbeat(float _delta)
{
	if(ship)
	{
		if(!retargetReady && !tempBehavior)
		{
			retargetTimer += _delta;
			if(retargetTimer >= retargetTime)
			{
				retargetTimer = 0;
				retargetReady = true;
			}
		}

		Vec2f forward;
		switch(movementBehavior)
		{
		case FOLLOW:
			forward = target - ship->GetPosition();
			if(joust && forward * forward < minDistance * minDistance)
				SetMovementBehavior(FLEE);
			forward.Normalize();
			ship->SetHeading(SaiphApp::GetHeadingFromUnitVector(forward));
			forward *= SaiphApp::GetPropulsionFlyweight(propulsionID).force;
			break;
		case FLEE:
			forward = ship->GetPosition() - target;
			if(joust && forward * forward > maxDistance * maxDistance)
				SetMovementBehavior(FOLLOW);
			forward.Normalize();
			ship->SetHeading(SaiphApp::GetHeadingFromUnitVector(forward));
			forward *= SaiphApp::GetPropulsionFlyweight(propulsionID).force;
			break;
		default:
			break;
		}
		ship->SetVelocity(forward);
		ship->Heartbeat(_delta);
	}
	else
		ship = SaiphApp::CloneShipFromPrototype(shipID);

	// update firing
	if(RandomFloat(0,1).GenerateValue() < fireFrequency)
		SetFireReady(true);
	ship->SetLastFired(ship->GetLastFired() + _delta);

	// update shield
	if(SaiphApp::GetShieldFlyweight(shieldID).strength)
	{
		if(ship->GetBattery() > SaiphApp::GetShieldFlyweight(shieldID).energy * _delta)
		{
			ship->SetBattery(ship->GetBattery() - SaiphApp::GetShieldFlyweight(shieldID).energy * _delta);
			ship->SetShield(ship->GetShield() + SaiphApp::GetShieldFlyweight(shieldID).recharge * _delta);
			if(ship->GetShield() > SaiphApp::GetShieldFlyweight(shieldID).strength)
				ship->SetShield(SaiphApp::GetShieldFlyweight(shieldID).strength);
		}
	}

	// update battery
	ship->SetBattery(ship->GetBattery() + SaiphApp::GetGeneratorFlyweight(generatorID).energy * _delta);
	if(ship->GetBattery() > SaiphApp::GetGeneratorFlyweight(generatorID).battery)
		ship->SetBattery(SaiphApp::GetGeneratorFlyweight(generatorID).battery);
}

Enemy *Enemy::Clone() const
{
	Enemy *ret = new Enemy(*this);
	ret->SetShip(nullptr);
	return ret;	
}