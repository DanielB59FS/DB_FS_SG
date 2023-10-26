#include "pch.h"
#include "LevelState.h"
#include "View/ViewManager.h"
#include "Model/HUD.h"
#include "Model/ProgressBar.h"
#include "Model/Enemy.h"
#include "Model/StickyBomb.h"
#include "Model/Ship.h"
#include "Model/PowerUp.h"
#include "Model/SpawnPoint.h"
#include "Model/Effect.h"
#include "Model/Boss.h"
#include "XML_Library/irrXML.h"

// Needed for rand()
#include <stdlib.h>

#include "StrategyOne/StrategyOne.h"
#include "StrategyTwo/StrategyTwo.h"

// Game Variables
#define POWER_UP_SPEED				25.0f
#define SCORE_POWER_UP_VALUE		5
#define MAX_WEAPON_UPGRADE			5
#define HUD_BAR_WIDTH				100.0f
#define TRACK_RATE					PI * 0.5f

// Lab2 includes
#include "./Model/MotherShip.h"

namespace	// Global to this file only
{
	// TODO Factory Method Lab: Place your method definitions for object creation here
	BaseObject* CreateMouseCursor();
	HUD* CreateHud();
	ProgressBar* CreateProgressBar();
	Ship* CreatePlayerShip();
	BaseObject* CreateBackground();
	PowerUp* CreatePowerUp(const PowerUp::TYPE type, const Vec2f& position, const Vec2f& velocity);

	BaseObject* CreateMouseCursor() {
		BaseObject* cursor = new BaseObject();
		ViewManager::GetInstance().AddObject(cursor, 5);

		cursor->SetTexture("Resources/images/reticle.png");
		cursor->SetDimensions(32, 32);
		cursor->SetColor(0.74f, 1, 1, 1);

		return cursor;
	}

	HUD* CreateHud() {
		return new HUD();
	}

	ProgressBar* CreateProgressBar() {
		ProgressBar* bar = new ProgressBar();
		bar->SetTexture("Resources/images/progressBar2.png");
		bar->SetDimensions(32.0f, 8.0f);
		return bar;
	}

	Ship* CreatePlayerShip() {
		Ship* playerShip = SaiphApp::CloneShipFromPrototype(SaiphApp::GetShipID());
		playerShip->SetShield(SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength);
		playerShip->SetBattery(SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
		playerShip->SetPosition(BOUNDS_SIZE / 2.0f, BOUNDS_SIZE / 2.0f);
		playerShip->SetAfterburner(SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
		return playerShip;
	}

	BaseObject* CreateBackground() {
		BaseObject* background = new BaseObject();
		ViewManager::GetInstance().AddObject(background, 0);

		background->SetTexture("Resources/images/background.png");
		background->SetDimensions(1024, 1024);
		background->SetPosition(background->GetWidth() / 2.0f, background->GetHeight() / 2.0f);

		return background;
	}

	PowerUp* CreatePowerUp(const PowerUp::TYPE type, const Vec2f& position, const Vec2f& velocity) {
		PowerUp* pup = new PowerUp(type);
		ViewManager::GetInstance().AddObject(pup, 2);
		
		switch (type) {
			case PowerUp::SCORE:
				pup->SetTexture("Resources/images/pupscore.png");
				break;
			case PowerUp::ENERGY:
				pup->SetTexture("Resources/images/pupenergy.png");
				break;
			case PowerUp::ARMOR:
				pup->SetTexture("Resources/images/puparmor.png");
				break;
			case PowerUp::WEAPON:
				pup->SetTexture("Resources/images/pupweapon.png");
				break;
		}

		pup->SetDimensions(32, 32);
		pup->SetPosition(position);
		pup->SetVelocity(velocity);

		return pup;
	}

	Effect *StartShieldEffect(Ship *_ship, const Vec2f &_impactorPosition, unsigned int _animationID)
	{
		Effect *shieldEffect = SaiphApp::CloneEffectFromPrototype(_animationID);
		float bigger = _ship->GetWidth();
		if(bigger < _ship->GetHeight())
			bigger = _ship->GetHeight();
		shieldEffect->SetDimensions(bigger, bigger);
		shieldEffect->SetTarget(_ship);
		shieldEffect->SetPosition(_ship->GetPosition());
		Vec2f direction = _impactorPosition - _ship->GetPosition();
		direction.Normalize();
		shieldEffect->SetHeading(SaiphApp::GetHeadingFromUnitVector(direction));
		return shieldEffect;
	}
	Effect *StartBulletEffect(const Bullet *_bullet)
	{
		// explosion
		Effect *bulletEffect = SaiphApp::CloneEffectFromPrototype(_bullet->GetDeathAnimation());
		bulletEffect->SetDimensions(_bullet->GetHeight() * 2, _bullet->GetHeight() * 2);
		bulletEffect->SetPosition(_bullet->GetPosition() + SaiphApp::GetUnitVectorFromHeading(_bullet->GetHeading()) * _bullet->GetWidth() * 0.5f);
		bulletEffect->SetHeading(RandomFloat(0, 2*PI).GenerateValue());
		return bulletEffect;
	}
}	// end anonymous namespace


// private helper methods
StickyBomb* LevelState::CreateStickyBomb(Enemy* const _enemy) {
	StickyBomb* bomb = nullptr;
	if (_enemy) {
		bomb = new StickyBomb();
		bomb->SetTarget(_enemy);
		_enemy->Attach(bomb);
		bomb->SetPosition(playerShip->GetPosition());
	}
	return bomb;
}

Effect *LevelState::DealDamageToShip(Ship *_ship, float _damage, const Vec2f &_impactorPosition, unsigned int _animationID)
{
	Effect *shieldEffect = nullptr;

	if (_damage <= 0)
		return shieldEffect;

	// deal damage to shields first
	if(_ship->GetShield())
	{
		// display shield hit effect
		shieldEffect = StartShieldEffect(_ship, _impactorPosition, _animationID);

		if(_damage > _ship->GetShield())
		{
			_damage -= _ship->GetShield();
			_ship->SetShield(0);
		}
		else
		{
			_ship->SetShield(_ship->GetShield() - _damage);
			_damage = 0;
		}
	}
	// deal remaining damage to armor
	if(_damage != 0)
	{
		_ship->SetArmor(_ship->GetArmor() - _damage);
	}
	return shieldEffect;
}

void LevelState::BulletTracking(Bullet *_bullet, float _delta, bool _enemyBullet)
{
	const BaseObject *target = nullptr;
	if(_enemyBullet)
		target = playerShip;
	else
	{
		const Enemy *enemy = GetNearestEnemy(_bullet->GetPosition());
		if(enemy)
			target = enemy->GetShip();
	}
	if(target)
	{
		// use toTarget as a temporary for forward heading vector
		Vec2f toTarget = SaiphApp::GetUnitVectorFromHeading(_bullet->GetHeading())
			// rotate forward vector 90 degrees clockwise to get rightward heading vector
			, bulletRight = Vec2f(toTarget.y, -toTarget.x);
		// set up a vector to the target
		toTarget = target->GetPosition() - _bullet->GetPosition();
		// compute the dot product of target vector and rightward vector for half-space test
		float dot = toTarget * bulletRight;
		if(dot < 0) // go right
			_bullet->SetHeading(_bullet->GetHeading() + TRACK_RATE * _delta);
		else if(dot > 0) // go left
			_bullet->SetHeading(_bullet->GetHeading() - TRACK_RATE * _delta);
	}
}

void LevelState::CleanObjectEffects(const BaseObject *_bo)
{
	for(unsigned int i = 0; i < effects.size(); ++i)
	{
		if(effects[i]->GetTarget() == _bo)
			effects[i]->SetTarget(nullptr);
	}
}

const Enemy* LevelState::GetNearestEnemy(const Vec2f &_location) const
{
	const Enemy *nearestEnemy = nullptr;
	float dotProduct, distanceSquared = 100000.0f;
	Vec2f vec;
	if(enemies.size())
	{
		for(unsigned int i = 0; i < enemies.size(); ++i)
		{
			if(enemies[i]->GetShip()->GetActive())
			{
				vec = enemies[i]->GetShip()->GetPosition() - _location;
				dotProduct = vec * vec;
				if(dotProduct < distanceSquared)
				{
					distanceSquared = dotProduct;
					nearestEnemy = enemies[i];
				}
			}
		}
	}
	return nearestEnemy;
}

void LevelState::CheckCollision()
{
	unsigned int score = 0, loop = 0;

	// Check collision for enemy bullets
	for(loop=0;loop<enemyBullets.size();loop++)
	{
		if(playerShip->Collide(*enemyBullets[loop]))
		{
			hud->SetTotalDamageReceived(hud->GetTotalDamageReceived() + enemyBullets[loop]->GetDamage());
			effects.push_back(StartBulletEffect(enemyBullets[loop]));
			enemyBullets[loop]->CollisionResponse();
			Effect *shieldEffect = DealDamageToShip(playerShip, enemyBullets[loop]->GetDamage(),
				enemyBullets[loop]->GetPosition(), SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).animationID);
			if(shieldEffect)
				effects.push_back(shieldEffect);
		}
	}

	// Check collision for enemies
	cursor->SetColor(0.5f, 1, 1, 1);
	for(unsigned cur=0;cur<enemies.size();cur++)
	{
		if(enemies[cur]->GetShip() && enemies[cur]->GetShip()->GetActive())
		{
			// Reticle collision
			if(enemies[cur]->GetShip()->Collide(*cursor))
			{
				cursor->SetColor(0.5f, 1, 0, 0);
				enemyHealth->Show();
				if(enemies[cur]->GetShip()->GetShield())
					enemyHealth->SetPercent((enemies[cur]->GetShip()->GetArmor() + enemies[cur]->GetShip()->GetShield()) /
					(SaiphApp::GetShipStartArmor(enemies[cur]->GetShipID()) + SaiphApp::GetShieldFlyweight(enemies[cur]->GetShieldID()).strength));
				else
					enemyHealth->SetPercent(enemies[cur]->GetShip()->GetArmor() / SaiphApp::GetShipStartArmor(enemies[cur]->GetShipID()));
				enemyHealth->SetBarColor(0.75f, 1.0f - enemyHealth->GetPercent(), enemyHealth->GetPercent(), 0);
				enemyHealth->SetWidth(enemies[cur]->GetShip()->GetWidth());
				enemyHealth->SetBarMaxDimensions(enemyHealth->GetWidth() - 2.0f, enemyHealth->GetHeight() - 2.0f);
				enemyHealth->SetPosition(enemies[cur]->GetShip()->GetXPosition()
					,enemies[cur]->GetShip()->GetYPosition() - enemies[cur]->GetShip()->GetHeight());
			}

			// Bullet collision
			for(loop=0;loop<playerBullets.size();loop++)
			{
				if(playerBullets[loop]->GetActive())
				{
					if(playerBullets[loop]->Collide(*enemies[cur]->GetShip()))
					{
						if (playerBullets[loop]->GetDeathAnimation() != 99)
							effects.push_back(StartBulletEffect(playerBullets[loop]));
						playerBullets[loop]->CollisionResponse();
						hud->SetNumHits(hud->GetNumHits() + 1.0f);
						Effect *shieldEffect = DealDamageToShip(enemies[cur]->GetShip(), playerBullets[loop]->GetDamage(),
							playerBullets[loop]->GetPosition(), SaiphApp::GetShieldFlyweight(enemies[cur]->GetShieldID()).animationID);
						if(shieldEffect)
							effects.push_back(shieldEffect);
						if(enemies[cur]->GetShip()->GetArmor() < 0)
						{
							enemies[cur]->GetShip()->CollisionResponse();
							SaiphApp::SetScore(SaiphApp::GetScore() + enemies[cur]->GetScore());
							GeneratePowerUps(enemies[cur]);
						}
					}
				}
			}
			if(playerShip)
			{
				if(enemies[cur]->GetShip()->Collide(*playerShip))
				{
					float enemyTotal = enemies[cur]->GetShip()->GetShield() + enemies[cur]->GetShip()->GetArmor();
					hud->SetTotalDamageReceived(hud->GetTotalDamageReceived() + enemyTotal);
					if(enemyTotal > playerShip->GetArmor() + playerShip->GetShield())
					{
						playerShip->SetShield(0);
						playerShip->SetArmor(-1);
					}
					else
					{
						enemies[cur]->GetShip()->CollisionResponse();
						SaiphApp::SetScore(SaiphApp::GetScore() + enemies[cur]->GetScore());
						Effect *shieldEffect = DealDamageToShip(playerShip, enemyTotal,
							enemies[cur]->GetShip()->GetPosition(), SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).animationID);
						if(shieldEffect)
							effects.push_back(shieldEffect);
						GeneratePowerUps(enemies[cur]);
					}
				}
			}
		}
	}
	// Check collision for powerups
	if(playerShip)
	{
		for(unsigned index = 0; index < powerUps.size(); ++index)
		{
			if(powerUps[index]->GetActive())
			{
				if(powerUps[index]->Collide(*playerShip))
				{
					powerUps[index]->SetActive(false);
					switch(powerUps[index]->GetType())
					{
					case PowerUp::SCORE:
						SaiphApp::SetScore(SaiphApp::GetScore() + SCORE_POWER_UP_VALUE);
						break;
					case PowerUp::ENERGY:
						playerShip->SetBattery(SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
						playerShip->SetAfterburner(SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
						break;
					case PowerUp::WEAPON:
						if(SaiphApp::GetWeaponLevel() < SaiphApp::GetWeaponFlyweight(SaiphApp::GetWeaponID()).levels.size() - 1)
							SaiphApp::SetWeaponLevel(SaiphApp::GetWeaponLevel() + 1);
						else
						{
							playerShip->SetBattery(SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
							playerShip->SetAfterburner(SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
							SaiphApp::SetScore(SaiphApp::GetScore() + static_cast<unsigned int>(SaiphApp::GetScore()*SCORE_POWER_UP_VALUE));
						}
						break;
					case PowerUp::ARMOR:
						playerShip->SetArmor(playerShip->GetArmor() + RandomFloat(5,30).GenerateValue());
						break;
					default:
						break;
					};
				}
			}
		}
	}
}
void LevelState::FireWeapon(Ship *_ship, float _heading, unsigned int _weaponID, unsigned int _level)
{
	Bullet *projectile;
	const WeaponLevelFlyweight &wlf = SaiphApp::GetWeaponFlyweight(_weaponID).levels[_level];
	if((_ship->GetBattery() > wlf.energy) && (_ship->GetLastFired() >= wlf.reloadTime))
	{
		_ship->SetBattery(_ship->GetBattery() - wlf.energy);
		_ship->SetLastFired(0);
		for(unsigned int i = 0; i < wlf.bullets.size(); ++i)
		{
			projectile = SaiphApp::CloneBulletFromPrototype(wlf.bullets[i]);
			projectile->SetPosition(_ship->GetPosition());
			projectile->SetHeading(projectile->GetHeading() + _heading);
			if(_ship == playerShip)
				playerBullets.push_back(projectile);
			else
				enemyBullets.push_back(projectile);
		}
	}
}

void LevelState::UpdateGameplayObjects(float _delta)
{
	unsigned int cur;

	// Update ship
	if(playerShip)
	{
		playerShip->SetLastFired(playerShip->GetLastFired() + _delta);
		if(playerShip->GetBattery() > SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).energy * _delta)
		{
			if(playerShip->GetAfterburnerFlag())
			{
				if(playerShip->GetAfterburner() > SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).energy * _delta)
					playerShip->SetAfterburner(playerShip->GetAfterburner() - SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).energy * _delta);
				else
					playerShip->SetAfterburnerFlag(false);
			}
			playerShip->SetBattery(playerShip->GetBattery() - SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).energy * _delta);
		}
		else
			playerShip->SetVelocity(Vec2f());
		if(playerShip->GetAfterburnerFlag())
			playerShip->SetVelocity(playerShip->GetVelocity()*2);
		playerShip->Heartbeat(_delta);

		// update shielding
		if(playerShip->GetShield() != SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength)
		{
			if(playerShip->GetBattery() > SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).energy * _delta)
			{
				playerShip->SetBattery(playerShip->GetBattery() - SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).energy * _delta);
				playerShip->SetShield(playerShip->GetShield() + SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).recharge * _delta);
				if(playerShip->GetShield() > SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength)
					playerShip->SetShield(SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength);
			}
		}
		// update battery
		playerShip->SetBattery(playerShip->GetBattery() + SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).energy * _delta);
		if(playerShip->GetBattery() > SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery)
		{
			if(!playerShip->GetAfterburnerFlag())
			{
				playerShip->SetAfterburner(playerShip->GetAfterburner() + playerShip->GetBattery() - SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
				if(playerShip->GetAfterburner() > SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy)
					playerShip->SetAfterburner(SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
			}
			playerShip->SetBattery(SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
		}
	}

	// Update power ups
	for(cur=0;cur<powerUps.size();++cur)
		if(powerUps[cur]->GetActive())
			powerUps[cur]->Heartbeat(_delta);

	// Update bullets
	for(cur=0;cur<playerBullets.size();++cur)
	{
		if(playerBullets[cur]->GetActive())
		{
			// Perform tracking for bullets that track
			if(playerBullets[cur]->GetTracking())
				BulletTracking(playerBullets[cur], _delta);
			playerBullets[cur]->Heartbeat(_delta);
		}
	}
	for(cur=0;cur<enemyBullets.size();++cur)
	{
		if(enemyBullets[cur]->GetActive())
		{
			if(enemyBullets[cur]->GetTracking())
				BulletTracking(enemyBullets[cur], _delta, true);
			enemyBullets[cur]->Heartbeat(_delta);
		}
	}
	// Update enemies
	enemyHealth->Hide();
	for(cur=0;cur<enemies.size();++cur)
	{
		if(!enemies[cur]->GetShip() || enemies[cur]->GetShip()->GetActive())
		{
			enemies[cur]->Heartbeat(_delta);

			if(enemies[cur]->GetFireReady())
			{
				if(enemies[cur]->GetMovementBehavior() == Enemy::FLEE)
					FireWeapon(enemies[cur]->GetShip(), enemies[cur]->GetShip()->GetHeading() + PI,
					enemies[cur]->GetWeaponID(), enemies[cur]->GetWeaponLevel());
				else
					FireWeapon(enemies[cur]->GetShip(), enemies[cur]->GetShip()->GetHeading(),
					enemies[cur]->GetWeaponID(), enemies[cur]->GetWeaponLevel());
				enemies[cur]->SetFireReady(false);
			}
			if(enemies[cur]->GetRetargetReady())
			{
				enemies[cur]->SetTarget(playerShip->GetPosition());
				enemies[cur]->SetRetargetReady(false);
			}
		}
	}

	// Update effects
	for(cur=0;cur<effects.size();++cur)
	{
		if(effects[cur]->GetActive())
			effects[cur]->Heartbeat(_delta);
	}

	// Update spawn points
	for(unsigned int i=0;i<spawnPoints.size();++i)
	{
		if(spawnPoints[i]->GetWaitTime() > 0)
			spawnPoints[i]->SetWaitTime(spawnPoints[i]->GetWaitTime() - _delta);
		if(spawnPoints[i]->GetWaitTime() <= 0)
		{
			Enemy *enemy = spawnPoints[i]->Spawn(_delta);
			if(enemy)
				enemies.push_back(enemy);
		}
	}

	// Prevent wait time filler
	if(!enemies.size() && spawnPoints.size())
	{
		float leastTime = spawnPoints[0]->GetWaitTime();
		unsigned int index = 0;
		for(unsigned int i=0;i<spawnPoints.size();++i)
		{
			if(leastTime > spawnPoints[i]->GetWaitTime())
			{
				leastTime = spawnPoints[i]->GetWaitTime();
				index = i;
			}
		}
		spawnPoints[index]->SetWaitTime(0);
	}
}
void LevelState::LoadLevel(const std::string &_filename)
{
	irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename.c_str());
	while(xmlReader->read())
	{
		if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
		{
			if(!strcmp("Spawn", xmlReader->getNodeName()))
			{
				SpawnPoint *spawnPoint = new SpawnPoint(xmlReader->getAttributeValueAsFloat("wait"),
					xmlReader->getAttributeValueAsFloat("interval"), xmlReader->getAttributeValueAsInt("enemy"),
					xmlReader->getAttributeValueAsInt("amount"), 
					Vec2f(xmlReader->getAttributeValueAsFloat("x"), xmlReader->getAttributeValueAsFloat("y")));
				spawnPoints.push_back(spawnPoint);
			}
			else if(!strcmp("Boss", xmlReader->getNodeName()))
			{
				if(boss == NULL)
					boss = new Boss();
			}
			else if(!strcmp("Waypoint", xmlReader->getNodeName()))
			{
				if(boss != NULL)
				{
					Vec2f waypoint;
					const char *value = xmlReader->getAttributeValue("random");
					if(value != NULL && !strcmp("true", value))
					{
						waypoint.x = RandomFloat(0, BOUNDS_SIZE).GenerateValue();
						waypoint.y = RandomFloat(0, BOUNDS_SIZE).GenerateValue();
					}
					else
					{
						waypoint.x = xmlReader->getAttributeValueAsFloat("x");
						waypoint.y = xmlReader->getAttributeValueAsFloat("y");
					}
					boss->AddWaypoint(waypoint);
				}
			}
		}
	}
	delete xmlReader;
}

void LevelState::GeneratePowerUps(Enemy *_enemy)
{
	// This method is invoked automatically by the CheckCollision method.
	// TODO Factory Method Lab: create power ups based on enemy information (see Enemy.h)
	for(unsigned int i = 0; i < _enemy->GetPowerUpAmount(); ++i)
	{
		// Use the following random velocity to set the velocity of the power up
		Vec2f randomVelocity = SaiphApp::GetUnitVectorFromHeading(RandomFloat(0, 2 * PI).GenerateValue()) * POWER_UP_SPEED;
		PowerUp* pup = CreatePowerUp(static_cast<PowerUp::TYPE>(_enemy->GetPowerUpID()), _enemy->GetShip()->GetPosition(), randomVelocity);
		powerUps.push_back(pup);
	}
}
void LevelState::ComputeHudInformation()
{
	if(playerShip)
	{
		hud->SetBarPercent(HUD::ENERGY, playerShip->GetBattery() / SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
		hud->SetBarPercent(HUD::SHIELD, playerShip->GetShield() / SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength);
		hud->SetBarPercent(HUD::ARMOR, playerShip->GetArmor() / MAX_ARMOR);
		hud->SetBarPercent(HUD::AFTERBURNER, playerShip->GetAfterburner() / SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
	}
	hud->Update();
	enemyHealth->Update();
}
void LevelState::ResetPlayer()
{
	if(playerShip)
	{
		ViewManager::GetInstance().RemoveObject(playerShip);
		delete playerShip;
	}
	
	if(hud)
	{
		hud->SetNumHits(0);
		hud->SetNumMisses(0);
		hud->SetTotalDamageReceived(0);
	}

	// ship creation
	Ship* _prototype = CreatePlayerShip();
	MotherShip* _motherShip = new MotherShip(*_prototype);
	_motherShip->SetDeflector(new Deflector(_motherShip, this));	// The only thing I needed to fix due to merging

	ViewManager::GetInstance().RemoveObject(_prototype);
	delete _prototype;
	ViewManager::GetInstance().AddObject(playerShip = _motherShip, 2);

	for (int i = 0; i < 5; ++i) {
		_prototype = SaiphApp::CloneShipFromPrototype(SaiphApp::GetShipID(), 1);
		_prototype->SetColor(0.2f, 0.8f, 0.8f, 0.8f);
		_motherShip->AddShadowShip(_prototype);
	}
}

void LevelState::GarbageCollect()
{
	static unsigned int loop;

	//	Clean up Bullet objects
	for(loop=0;loop<playerBullets.size();)
	{
		if(!playerBullets[loop]->GetActive())
		{
			// if a bullet dies due to being off the screen then consider
			// it a "miss" for statistics
			if(playerBullets[loop]->GetYPosition() >= BOUNDS_SIZE * 1.125f ||
				playerBullets[loop]->GetYPosition() <= -BOUNDS_SIZE * 1.125f ||
				playerBullets[loop]->GetXPosition() >= BOUNDS_SIZE * 1.125f ||
				playerBullets[loop]->GetXPosition() <= -BOUNDS_SIZE * 1.125f)
				hud->SetNumMisses(hud->GetNumMisses() + 1.0f);

			ViewManager::GetInstance().RemoveObject(playerBullets[loop]);
			delete playerBullets[loop];
			playerBullets.erase(playerBullets.begin() + loop);
		}
		else ++loop;
	}

	for(loop=0;loop<enemyBullets.size();)
	{
		if(!enemyBullets[loop]->GetActive())
		{
			ViewManager::GetInstance().RemoveObject(enemyBullets[loop]);
			delete enemyBullets[loop];
			enemyBullets.erase(enemyBullets.begin() + loop);
		}
		else ++loop;
	}

	//	Clean up Enemy objects
	for(loop=0;loop<enemies.size();)
	{
		if(!enemies[loop]->GetShip()->GetActive())
		{
			CleanObjectEffects(enemies[loop]->GetShip());
			delete enemies[loop];	// view taken care of inside enemy destructor
			enemies.erase(enemies.begin() + loop);
		}
		else ++loop;
	}

	// Clean up Power Up objects
	for(loop=0;loop<powerUps.size();)
	{
		if(!powerUps[loop]->GetActive())
		{
			ViewManager::GetInstance().RemoveObject(powerUps[loop]);
			delete powerUps[loop];
			powerUps.erase(powerUps.begin() + loop);
		}
		else ++loop;
	}

	// Clean up spawn points
	for(loop=0;loop<spawnPoints.size();)
	{
		if(!spawnPoints[loop]->GetEnemiesRemaining())
		{
			delete spawnPoints[loop];
			spawnPoints.erase(spawnPoints.begin() + loop);
		}
		else ++loop;
	}

	// Clean up effects
	for(loop=0;loop<effects.size();)
	{
		if(!effects[loop]->GetActive())
		{
			ViewManager::GetInstance().RemoveObject(effects[loop]);
			delete effects[loop];
			effects.erase(effects.begin() + loop);
		}
		else ++loop;
	}
}

unsigned int LevelState::CheckForInput()
{
	unsigned int stateChange = NOCHANGE;
	static float fReloadTimer = 0;
	Vec2f velocity;
	playerShip->SetAfterburnerFlag(false);

	if(GetAsyncKeyState(VK_ESCAPE) & 0x1)
		stateChange = MENU;

	// Toggle HUD information
	if(GetAsyncKeyState('H') & 0x1) detailedHud = !detailedHud;

	// Toggle pause of game
	if(GetAsyncKeyState('P') & 0x1)
	{
		// don't allow them to unpause if they lost or won
		if(playerShip->GetActive() && (spawnPoints.size() || enemies.size() || powerUps.size()))
			gamePaused = !gamePaused;
	}

	if (GetAsyncKeyState('C') & 0x1) {
		for (Enemy* enemy : enemies)
			if (enemy->GetShip()->Collide(*cursor))
				if (10 <= playerShip->GetBattery()) {
					StickyBomb* bomb = CreateStickyBomb(enemy);
					if (bomb) {
						playerShip->SetBattery(playerShip->GetBattery() - 10);
						ViewManager::GetInstance().AddObject(bomb, 1);
						playerBullets.push_back(bomb);
					}
				}
	}

	if (GetAsyncKeyState('X') & 0x1)
		for (Subject* sub : enemies)
			sub->Broadcast(ObserverEvent::ACTIVATE);
	
	if (GetAsyncKeyState('E') & 0x1) {
		Deflector* deflector = playerShip->GetDeflector();
		if (deflector) deflector->SetActive(!deflector->GetActive());
	}

	if (GetAsyncKeyState('F') & 0x1) {
		Deflector* deflector = playerShip->GetDeflector();
		if (deflector) deflector->ActivateStrategy("StrategyOne.dll");
	}

	if (GetAsyncKeyState('G') & 0x1) {
		Deflector* deflector = playerShip->GetDeflector();
		if (deflector) deflector->ActivateStrategy("StrategyTwo.dll");
	}

	if(!gamePaused)
	{

		// Ship movement
		if(playerShip)
		{
			// If Shift Key OR Right Mouse Button is clicked...
			if((GetAsyncKeyState(VK_SHIFT) || GetAsyncKeyState(VK_RBUTTON)))
				playerShip->SetAfterburnerFlag(true);
			if(SaiphApp::GetControl())	// using cartesian style
			{
				if(GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) 
					velocity.x += 1;
				else if(GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) 
					velocity.x += -1;
				if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) 
					velocity.y += -1;
				else if(GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) 
					velocity.y += 1;
			}
			else				// using polar style
			{
				if(GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) 
					velocity += SaiphApp::GetUnitVectorFromHeading(playerShip->GetHeading() + PI/2);
				else if(GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) 
					velocity += SaiphApp::GetUnitVectorFromHeading(playerShip->GetHeading() - PI/2);
				if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) 
					velocity += SaiphApp::GetUnitVectorFromHeading(playerShip->GetHeading());
				else if(GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) 
					velocity += SaiphApp::GetUnitVectorFromHeading(playerShip->GetHeading() + PI);
			}

			velocity.Normalize();
			velocity *= SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).force;
			playerShip->SetVelocity(velocity);

			// Fire a bullet
			if((GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_LBUTTON)) &&
				playerShip->GetLastFired() > SaiphApp::GetWeaponFlyweight(SaiphApp::GetWeaponID()).levels[SaiphApp::GetWeaponLevel()].reloadTime)
				FireWeapon(playerShip, playerShip->GetHeading(), SaiphApp::GetWeaponID(), SaiphApp::GetWeaponLevel());
		}
	}

	return stateChange;
}

// interface methods
void LevelState::Init()
{
	win = false;
	gamePaused = false;
	endLevels = false;
	srand((unsigned int)time(0));	// seed the random number generator

	// background
	// TODO Factory Method Lab: Replace NULL with call to Factory method
	background = CreateBackground();

	// hud
	hud = CreateHud();
	
	// progress bar
	enemyHealth = CreateProgressBar();

	// mouse
	cursor = CreateMouseCursor();

	// boss
	boss = NULL;

	// ship
	playerShip = NULL; // don't modify this
	ResetPlayer();

	// setup level
	SaiphApp::LoadEnemyPrototypes("Resources/enemyPrototypes.xml");
	char name[256] = {0};
	sprintf(name, "Resources/level%d.xml", SaiphApp::GetLevel());
	LoadLevel(name);
	if(!spawnPoints.size())
	{
		SaiphApp::SetLevel(0);
		endLevels = true;
	}
	scoreAtBeginning = SaiphApp::GetScore();
}
void LevelState::RenderFrame() const
{
	char displayTextBuffer[80] = {0};
	ViewManager::GetInstance().BeginScene();

	// render all views
	ViewManager::GetInstance().RenderObjects();

	// render hud text on top of everything else
	hud->Render();
	if(detailedHud)
	{
		if(playerShip)
		{
			sprintf(displayTextBuffer, "%.2f", playerShip->GetArmor());
			ViewManager::GetInstance().RenderText(725, 54, displayTextBuffer, Color4f(1,0,1,0));
			sprintf(displayTextBuffer, "%.2f", playerShip->GetShield());
			ViewManager::GetInstance().RenderText(725, 107, displayTextBuffer, Color4f(1,0,1,1));
			sprintf(displayTextBuffer, "%.2f", playerShip->GetAfterburner());
			ViewManager::GetInstance().RenderText(725, 160, displayTextBuffer, Color4f(1,1,0,0));
			sprintf(displayTextBuffer, "%.2f", playerShip->GetBattery());
			ViewManager::GetInstance().RenderText(725, 213, displayTextBuffer, Color4f(1,1,1,0));
		}
		sprintf(displayTextBuffer, "Enemies: %.4d Bullets: %.3d", enemies.size(), playerBullets.size());
		ViewManager::GetInstance().RenderText(608, 406, displayTextBuffer);
		sprintf(displayTextBuffer, "Effects: %d", effects.size());
		ViewManager::GetInstance().RenderText(608, 459, displayTextBuffer);
	}
	if(gamePaused)
	{
		if(!spawnPoints.size() && !enemies.size() && !powerUps.size())
		{
			sprintf(displayTextBuffer, "Hits: %.0f Misses: %.0f", hud->GetNumHits(), hud->GetNumMisses());
			ViewManager::GetInstance().RenderText(100, 100, displayTextBuffer);
			sprintf(displayTextBuffer, "Accuracy: %.2f%%", hud->GetAccuracy());
			ViewManager::GetInstance().RenderText(100, 120, displayTextBuffer);
			sprintf(displayTextBuffer, "Total Damage Withstood: %.0f", hud->GetTotalDamageReceived());
			ViewManager::GetInstance().RenderText(100, 140, displayTextBuffer);
			sprintf(displayTextBuffer, "Press 'ESC' to return to the main menu.");
		}
		else if(!playerShip->GetActive())
			sprintf(displayTextBuffer, "You Lose! Press 'ESC' to return to the main menu.");
		else
			sprintf(displayTextBuffer, "Game paused! Press P to unpause.");

		ViewManager::GetInstance().RenderText(WINDOW_WIDTH*.2f, WINDOW_HEIGHT/2, displayTextBuffer);
	}

	ViewManager::GetInstance().EndScene();
}
unsigned int LevelState::Heartbeat(float _delta)
{
	unsigned int stateChange = NOCHANGE;

	if(endLevels)
		return MENU;

	// update hud info
	ComputeHudInformation();

	// update reticle (mouse cursor)
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(FindWindow(WINDOW_TITLE, WINDOW_TITLE), &mouse);
	cursor->SetXPosition(float(mouse.x));
	cursor->SetYPosition(float(mouse.y));

	// lose condition
	if(playerShip->GetArmor() < 0)
	{
		gamePaused = true;
		playerShip->CollisionResponse();
		ViewManager::GetInstance().RemoveObject(playerShip);
	}

	// force player ship to face reticle
	if(playerShip && !gamePaused)
		playerShip->TurnToLocation(cursor->GetPosition());


	// Check for input
	stateChange = CheckForInput();

	// If the game is paused, do nothing else
	if(!gamePaused)
	{

		UpdateGameplayObjects(_delta);

		CheckCollision();
		GarbageCollect();

		// Win condition
		if(!spawnPoints.size() && !enemies.size() && !powerUps.size())
		{
			if(boss != NULL)
			{
				boss->GetShip()->SetActive(true);
				enemies.push_back(boss);
				boss = NULL;
			}
			else
			{
				win = true;
				gamePaused = true;
				SaiphApp::SetLevel(SaiphApp::GetLevel() + 1);
			}
		}
	}

	return stateChange;
}
void LevelState::Shutdown()
{
	// reset score if didn't win
	if(!win)
		SaiphApp::SetScore(scoreAtBeginning);

	// Clean up Model data
	for(unsigned int i = 0; i < enemyBullets.size(); i++)
	{
		ViewManager::GetInstance().RemoveObject(enemyBullets[i]);
		delete enemyBullets[i];
	}
	enemyBullets.clear();

	for(unsigned int i = 0; i < playerBullets.size(); i++)
	{
		ViewManager::GetInstance().RemoveObject(playerBullets[i]);
		delete playerBullets[i];
	}
	playerBullets.clear();

	// boss never got added to enemy vector (exit before spawn)
	if(boss)
	{
		delete boss;
		boss = 0;
	}

	for(unsigned int i = 0; i < enemies.size(); i++)
		delete enemies[i];
	enemies.clear();

	for(unsigned int i = 0; i < powerUps.size(); ++i)
	{
		ViewManager::GetInstance().RemoveObject(powerUps[i]);
		delete powerUps[i];
	}
	powerUps.clear();

	for(unsigned int i = 0; i < spawnPoints.size(); ++i)
		delete spawnPoints[i];
	spawnPoints.clear();

	for(unsigned int i = 0; i < effects.size(); ++i)
	{
		ViewManager::GetInstance().RemoveObject(effects[i]);
		delete effects[i];
	}
	effects.clear();

	// player ship data
	ViewManager::GetInstance().RemoveObject(playerShip);
	delete playerShip;
	playerShip = NULL;

	// reticle
	ViewManager::GetInstance().RemoveObject(cursor);
	delete cursor;
	cursor = NULL;

	// hud data
	delete hud;
	hud = NULL;

	delete enemyHealth;
	enemyHealth = NULL;

	// background
	ViewManager::GetInstance().RemoveObject(background);
	delete background;
	background = NULL;

	// Clean up enemy prototypes
	SaiphApp::DestroyEnemyPrototypes();
}