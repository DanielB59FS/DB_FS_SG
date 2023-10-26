#ifndef BUGS
#include "Defines.h"

#ifdef _MOVINGOBJECT_OBJ_
#include "../UnitTest++/UnitTest++.h"
#include "../SaiphApp.h"
#include <iostream>

#include "../Model/MovingObject.h"
using std::cout;


TEST(MovingObject_Heartbeat)
{
	cout << "Running Test: MovingObject_Heartbeat...\n\tResult: ";
	bool success;
	MovingObject tmo;

	// Setup
	tmo.SetVelocity(2.5f, 1.0f);

	// Run
	tmo.Heartbeat(1.0f);

	// Test
	Vec2f pos = tmo.GetPosition();
	if(pos.x == 2.5f && pos.y == 1.0f)
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