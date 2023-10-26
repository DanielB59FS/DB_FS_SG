#include "../UnitTester/Defines.h"

#ifdef _SHOPSTATE_OBJ_
#include "../UnitTest++/UnitTest++.h"
#include "../SaiphApp.h"
#include "SubShopState.h"
#include "../Model/HUD.h"
#include "../Model/BaseObject.h"
#include "../Model/Bullet.h"
#include <iostream>
using std::vector;
using std::cout;

int numShopStateTests = 10;

TEST(ShopState_Init)
{
	SaiphApp::LoadShips("Resources\\shipPrototypes.xml");

	cout << "Running Test: (14) ShopState_Init...\n\tResult: ";

	bool success = false;
	ShopState shop;

	shop.Init();

	if(shop.cursor != NULL)
	{
		success = true;
		cout<<"SUCCESS!!\n\n";
	}
	else
	{
		success = false;
		cout<<"Failure!\n\n";
	}
	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();
}

TEST(ShopState_CreateMouseCursor)
{
	SaiphApp::LoadShips("Resources\\shipPrototypes.xml");

	cout << "Running Test: (46) ShopState_CreateMouseCursor...\n\tResult: ";

	bool success = false;
	ShopState shop;
	BaseObject *cursor = shop.CreateMouseCursor();
	Vec2f dimensions = cursor->GetDimensions();
	if (dimensions.x != 10000 &&
		dimensions.y != 10000)
	{
		success = true;
		cout << "SUCCESS!!\n\n";
	}
	else
	{
		success = false;
		cout << "Failure!\n\n";
	}
	CHECK(success);
	ViewManager::GetInstance().RemoveObject(cursor);
	delete cursor;
	SaiphApp::CleanUp();
}
#include "../Model/Ship.h"
TEST(ShopState_ResetPlayer)
{
	SaiphApp::LoadShips("Resources\\shipPrototypes.xml");
	cout << "Running Test: (30) ShopState_ResetPlayer...\n\tResult: ";
	bool success = false;
	ShopState shop;
	shop.Init();
	shop.ResetPlayer();
	Ship *playerShip = shop.playerShip;
	if (playerShip && playerShip->GetYPosition() == (WINDOW_HEIGHT / 2.0f))
	{
		success = true;
		cout << "SUCCESS!!\n\n";
	}
	else
	{
		success = false;
		cout << "Failure!\n\n";
	}
	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();
}

TEST(ShopState_PlayerShieldEffect)
{
	cout << "Running Test: (47) ShopState_PlayerShieldEffect...\n\tResult: ";
	bool success = false;
	ShopState shop;
	shop.Init();
	SaiphApp::LoadConfigData("Resources/config.xml");
	SaiphApp::LoadAnimations("Resources/anims.xml");
	SaiphApp::LoadFlyweights("Resources/flyweights.xml");
	SaiphApp::LoadShips("Resources/shipPrototypes.xml");
	Vec2f impactPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	int effectSize = shop.effects.size();
	shop.PlayerShieldEffect(impactPosition);
	if (shop.effects.size() != effectSize+1)
	{
		success = false;
		cout << "Failure!!\n\n";
	}
	else
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}

	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();

}

TEST(ShopState_Shutdown)
{
	SaiphApp::LoadShips("Resources\\shipPrototypes.xml");

	cout << "Running Test: (7) ShopState_Shutdown...\n\tResult: ";

	bool success = false;
	_CrtMemState s1, s2;
	SubShopState shop;
	_CrtMemCheckpoint(&s1);
	shop.Init();
	shop.Shutdown();
	_CrtMemCheckpoint(&s2);

	if(shop.shiptypes.begin() != shop.shiptypes.end() 
		|| s1.lCounts[_CLIENT_BLOCK] != s2.lCounts[_CLIENT_BLOCK])
	{
		success = false;
		cout<<"Failure!!\n\n";
	}
	else
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}

	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();
}

TEST(ShopState_ComputeHudInformation)
{
	cout << "Running Test: (34) ShopState_ComputeHudInformation...\n\tResult: ";
	SaiphApp::LoadFlyweights("Resources/flyweights.xml");

	bool success = false;
	SubShopState shop;
	shop.Init();
	SaiphApp::SetShieldID(2);
	SaiphApp::SetGeneratorID(2);
	shop.ComputeHudInformation();

	if(shop.hud && shop.playerShip // pre-requisites
		&& shop.hud->GetBarPercent(HUD::ENERGY) == shop.playerShip->GetBattery() /
		SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery
		&& shop.hud->GetBarPercent(HUD::SHIELD) == shop.playerShip->GetShield() /
		SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength)
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		cout << "Failure!\n\n";
	}

	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();
} 

TEST(ShopState_GarbageCollect)
{
	cout << "Running Test: (43) ShopState_GarbageCollect...\n\tResult: ";

	bool success = false;
	ShopState shop;
	Bullet* someBullets[4];
	for(unsigned int iBullet = 0; iBullet < 4; iBullet++)
	{
		someBullets[iBullet] = new Bullet();
		someBullets[iBullet]->SetActive(false);
		shop.bullets.push_back(someBullets[iBullet]);
	}
	
	shop.GarbageCollect();

	if(shop.bullets.size() == 0)
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		cout<<"Failure!\n\n";
	}

	CHECK(success);
}

extern int alteredScore;
TEST(ShopState_RenderFrame)
{
	SaiphApp::LoadShips("Resources\\shipPrototypes.xml");

	cout << "Running Test: (6) ShopState_RenderFrame...\n\tResult: ";

	bool success = false;
	SubShopState shop;
	shop.Init();
	alteredScore = SaiphApp::GetScore()+6;
	shop.RenderFrame();


	if (alteredScore != SaiphApp::GetScore())
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		cout << "Failure!\n\n";
	}

	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();
}

TEST(ShopState_FireWeapon)
{
	cout << "Running Test: (13) ShopState_FireWeapon...\n\tResult: ";
	bool success = false;
	ShopState shop;
	shop.Init();
	SaiphApp::LoadConfigData("Resources/config.xml");
	SaiphApp::LoadAnimations("Resources/anims.xml");
	SaiphApp::LoadFlyweights("Resources/flyweights.xml");
	SaiphApp::LoadShips("Resources/shipPrototypes.xml");
	const WeaponLevelFlyweight &wlf = SaiphApp::GetWeaponFlyweight(SaiphApp::GetWeaponID()).levels[SaiphApp::GetWeaponLevel()];
	shop.playerShip->SetBattery(wlf.energy + 1);
	shop.playerShip->SetLastFired(wlf.reloadTime - 1);
	int bulletCount = shop.bullets.size();
	shop.FireWeapon();
	if (shop.bullets.size() == bulletCount)
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		cout << "Failure!\n\n";
	}
	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();
}

TEST(ShopState_PerformSelection)
{
	cout << "Running Test: (45) ShopState_PerformSelection...\n\tResult: ";
	bool success = false;
	SaiphApp::LoadConfigData("Resources/config.xml");
	SaiphApp::LoadAnimations("Resources/anims.xml");
	SaiphApp::LoadFlyweights("Resources/flyweights.xml");
	SaiphApp::LoadShips("Resources/shipPrototypes.xml");
	SubShopState shop;
	shop.Init();

	alteredScore = 19790;
	shop.submenu = "Weapon";
	shop.currentMenu = &shop.weapons;
	shop.currentMenu->SelectItem(6);
	unsigned int temp = atoi(shop.weapons.GetSelectedItem()->GetActionValue().c_str());
	shop.PerformSelection(true);

	if (SaiphApp::GetWeaponID() == temp / 100)
	{
		success = true;
		std::cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		std::cout << "Failure!\n\n";
	}

	CHECK(success);
	shop.Shutdown();
	SaiphApp::CleanUp();
}

#endif _SHOPSTATE_OBJ_
