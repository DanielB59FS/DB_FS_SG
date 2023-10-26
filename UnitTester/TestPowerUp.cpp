#ifndef BUGS
#include "Defines.h"

#ifdef _POWERUP_OBJ_
#include "../SaiphApp.h"
#include "../Model/PowerUp.h"
#include "../UnitTest++/UnitTest++.h"
#include <iostream>
using std::cout;

// negative black box test
// checks for mutator constraints
TEST(PowerUp_SetType)
{
	cout << "Running Test: PowerUp_SetType...\n\tResult: ";
	PowerUp tpu;
	int type;
	bool success;

	// Setup
	tpu.SetType(1);
	tpu.SetType(-1);
	tpu.SetType(42);

	// Test
	type = tpu.GetType();

	if(type == 1)
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
#endif