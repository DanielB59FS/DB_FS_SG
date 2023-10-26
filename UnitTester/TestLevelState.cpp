#include "Defines.h"

#ifdef _LEVELSTATE_OBJ_ // to end of file
#include "..\\UnitTest++\\UnitTest++.h"
#include "..\\SaiphApp.h"
#include <iostream>
using std::vector;
using std::cout;
#include "..\\LevelState.h"


#ifdef _EFFECT_OBJ_
#ifdef _BASEOBJECT_OBJ_
#include "..\\Model\\Effect.h"

TEST(LevelState_CleanObjectEffects)
{
	cout << "Running Test: LevelState_CleanObjectEffects...\n\tResult: ";
	// Setup
	LevelState tls;
	tls.Init();
	Effect* e1 = new Effect();
	Effect* e2 = new Effect();
	BaseObject* bo1 = new BaseObject();
	BaseObject* bo2 = new BaseObject();
	e2->SetTarget(bo2);
	e1->SetTarget(bo1);
	bool success;

	tls.effects.push_back(e1);
	tls.effects.push_back(e2);

	// Run
	tls.CleanObjectEffects(bo2);

	// Test
	if(tls.effects[1]->GetTarget() == NULL  && tls.effects[0]->GetTarget() != NULL)
	{
		// Success!!
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		// Failed
		success = false;
		cout << "Failed...\n\n";
	}

	CHECK(success);

	// Cleanup
	tls.Shutdown();
	delete bo1;
	delete bo2;

}
#endif
#endif

#ifdef _BULLET_OBJ_
#include "..\\Model\\Bullet.h"
TEST(LevelState_GarbageCollect)
{
	cout << "Running Test: LevelState_GarbageCollect...\n\tResult: ";
	const unsigned int numBullets = 3;
	Bullet* bullets [numBullets];
	Bullet* enemyBullets [numBullets];
	LevelState tls;
	tls.Init();
	bool success;

	// Setup
	for(int i = 0; i < numBullets; ++i)
	{
		bullets[i] = new Bullet();
		enemyBullets[i] = new Bullet();

		bullets[i]->SetActive(false);
		enemyBullets[i]->SetActive(false);

		tls.playerBullets.push_back(bullets[i]);
		tls.enemyBullets.push_back(enemyBullets[i]);
	}

	// Run
	tls.GarbageCollect();

	// Test
	if(tls.playerBullets.size() == 0 && tls.enemyBullets.size() == 0)
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		cout << "Failed...\n\n";
	}

	CHECK(success);

	// Cleanup
	tls.Shutdown();
}
#endif

#ifdef _SHIP_OBJ_
#include "..\\Model\\Ship.h"
TEST(LevelState_UpdateGameplayObjects)
{
	cout << "Running Test: LevelState_UpdateGameplayObjects...\n\tResult: ";
	// Setup
	LevelState tls;
	tls.Init();
	float lastFired = tls.playerShip->GetLastFired();
	float delta = 0.15f;
	bool success;

	// Run
	tls.UpdateGameplayObjects(delta);

	// Test
	if(tls.playerShip->GetLastFired() == lastFired + delta)
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		cout << "Failed...\n\n";
	}

	CHECK(success);

	// Cleanup
	tls.Shutdown();
}
#endif
#endif	// #ifdef _LEVELSTATE_OBJ_
