// ShopState.h
// ShopState class for Managing Shop Entities.
// Code by Derek Bliss
// Created May 15, 2008
// Last updated October 15, 2008
// 
#ifndef SUBSHOPSTATE_H
#define SUBSHOPSTATE_H

#include "ShopState.h"
#include "../Model/BaseObject.h"
#include "../Model/Ship.h"
#include "../Model/HUD.h"
#include "../Model/Bullet.h"
#include "../Model/Effect.h"
#include "../Model/Menu.h"

class HUD;
class Ship;
class Effect;
class Bullet;

class SubShopState : public ShopState
{
public:

	// override to fix menu setup for testing
	void Init();
	void GenerateShopMenus();
};

#endif