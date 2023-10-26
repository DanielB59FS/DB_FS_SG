#ifndef BUGS
#include "Defines.h"

#ifdef _ENEMY_OBJ_
#include "../UnitTest++/UnitTest++.h"
#include "../SaiphApp.h"
#include "../Model/Enemy.h"
#include <iostream>
using std::cout;

TEST (Enemy_Object)
{
	cout << "Running Test: Enemy_Object...\n\tResult: ";
	bool success;
	{
		Enemy e;
	}
	if(true)
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
}

TEST (Enemy_Clone)
{
	cout << "Running Test: Enemy_Clone...\n\tResult: ";
	bool success;
	Enemy *e = new Enemy;
	Enemy *copy = e->Clone();

	delete e;
	e = NULL;

	copy->SetFireFrequency(0.1234f);

	if(copy->GetFireFrequency() == 0.1234f)
	{
		success = true;
		cout << "SUCCESS!\n\n";
	}
	else
	{
		success = false;
		cout << "Failed...\n\n";
	}

	delete copy;
	copy = NULL;
	CHECK(success);
}

TEST(Enemy_SetScore)
{
	
	cout << "Running Test: Enemy_SetScore...\n\tResult: ";
	bool success;
	Enemy e;
	unsigned int score = 50;
	
	e.SetScore(50);
	if(e.GetScore() == score)
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
}
#endif
#endif BUGS