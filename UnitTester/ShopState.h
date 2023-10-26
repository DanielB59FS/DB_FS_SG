// ShopState.h
// ShopState class for Managing Shop Entities.
// Code by Derek Bliss
// Created May 15, 2008
// Last updated October 15, 2008
// 
#ifndef SHOPSTATE_H
#define SHOPSTATE_H

#include "../AbstractState.h"
#include "../Model/Menu.h"

class HUD;
class Ship;
class Effect;
class Bullet;

class ShopState : public AbstractState
{
public:

	// interface methods
	unsigned int Heartbeat(float _delta);		// This method gives shop entities an update
	void Init();								// This method initializes the shop entities
	void RenderFrame() const;					// This method renders all entities to the drawing context
	void Shutdown();							// This method does any necessary clean-up

protected:

	// menu variables
	std::map<int, Ship *> shiptypes;
	unsigned int numShownItems;
	float shieldTimer;
	const MenuItem *selected;
	VerticalMenuList menu, ships, weapons,
		shields, generators, props, scroll;
	VerticalMenuList *currentMenu;
	std::string submenu;

	// hud variables
	HUD *						hud;

	// mouse variables
	BaseObject *				cursor;

	// model variables
	Ship *						playerShip;
	std::vector<Effect *>		effects;
	std::vector<Bullet *>		bullets;

	// helper methods
	BaseObject* CreateMouseCursor();
	HUD* CreateHud();
	MenuItem* CreateMenuItem(const Color4f &_backgroundColor = Color4f(),const std::string &_texture = "Resources\\images\\menubox.png");
	void GenerateShopMenus();
	void ComputeHudInformation();
	void ResetPlayer();
	void GarbageCollect();
	void FireWeapon();
	unsigned int PerformSelection(bool _menu);
	void PlayerShieldEffect(const Vec2f &_impactorPosition);

	//Friend classes for testing
	friend class TestShopState_Shutdown;
	friend class TestShopState_Init;
	friend class TestShopState_CreateMouseCursor;
	friend class TestShopState_ResetPlayer;
	friend class TestShopState_PlayerShieldEffect;
	friend class TestShopState_GarbageCollect;
	friend class TestShopState_FireWeapon;
	friend class TestShopState_ComputeHudInformation;
	friend class TestShopState_PerformSelection;
};

#endif