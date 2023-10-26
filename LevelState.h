// LevelState.h
// LevelState class for Managing Game Entities.
// Code by Nick Penney and Derek Bliss
// Created January 6, 2007
// Last updated August 27, 2010
// 
#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include "AbstractState.h"

#include "Model/Deflector.h"

// Always forward declare classes whenever possible
// to reduce compile times and inclusion errors
class HUD;
class ProgressBar;
class SpawnPoint;
class BaseObject;
class Effect;
class Bullet;
class PowerUp;
class Ship;
class Enemy;
class Boss;

class StickyBomb;

class LevelState : public AbstractState
{
	//////////////////////
	// control variables
	bool						win;				// level is won?
	bool						gamePaused;			// Is the game paused?
	bool						detailedHud;		// Show detailed HUD information?
	bool						endLevels;			// Have we reached a non-existant level?
	unsigned int				scoreAtBeginning;	// Score before level begins
	//////////////////////

	//////////////////////
	// model variables
	HUD *						hud;				// Models for hud
	ProgressBar *				enemyHealth;		// Model for displaying enemy health on mouse-over
	BaseObject *				cursor;
	BaseObject *				background;
	// gameplay objects
	Ship *						playerShip;
	Boss *						boss;
	std::vector<Effect *>		effects;
	std::vector<Bullet *>		playerBullets;
	std::vector<SpawnPoint *>	spawnPoints;
	std::vector<Enemy *>		enemies;
	std::vector<Bullet *>		enemyBullets;
	std::vector<PowerUp *>		powerUps;
	//////////////////////

	//////////////////////
	// private helper methods
	StickyBomb* CreateStickyBomb(Enemy* const _enemy);

	// This method updates a ship's shields and/or armor values when delt damage and
	// creates a damage effect and/or text effect if necessary.
	Effect *DealDamageToShip(Ship *_ship, float _damage, const Vec2f &_impactorPosition, unsigned int _animationID);
	// This method creates spawn points needed for the level to function
	void LoadLevel(const std::string &_filename);
	// This method generates power up objects based on the given defeated enemy
	void GeneratePowerUps(Enemy *_enemy);
	// This method computes values used when displaying the H.U.D.
	void ComputeHudInformation();
	// This method deletes the existing player ship if necessary and allocates a new
	// one based on currently active upgrades
	void ResetPlayer();
	// This method handles keyboard and mouse input
	unsigned int CheckForInput();
	// This method allows bullets to track the closest enemy or the player depending on
	// whether it is a player bullet or an enemy bullet respectively
	void BulletTracking(Bullet *_bullet, float _delta, bool _enemyBullet = false);
	// This method returns a pointer to the closest Enemy object to the given location
	const Enemy *GetNearestEnemy(const Vec2f &_location) const;
	// This method sets all effects that relate to the specified object to inactive
	void CleanObjectEffects(const BaseObject *_bo);
	// This method generates a bullet for the player or the enemies
	void FireWeapon(Ship *_ship, float _heading, unsigned int _weaponID, unsigned int _level);
	// This method deletes inactive game entities
	void GarbageCollect();
	// This method performs an update on all game related entities
	void UpdateGameplayObjects(float _delta);
	// This method checks for meaningful game entity collisions
	void CheckCollision();
	//////////////////////

	friend void Deflector::Deflect(Bullet *projectile);

public:
	//////////////////////
	// interface methods
	unsigned int Heartbeat(float _delta);			// This method gives all state related entities an update
	void Init();									// This method initializes the game entities
	void RenderFrame() const;						// This method renders all entities to the drawing context
	void Shutdown();								// This method does any necessary clean-up
	//////////////////////
};

#endif