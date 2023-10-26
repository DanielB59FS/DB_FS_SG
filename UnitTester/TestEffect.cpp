#ifndef BUGS
#include "Defines.h"

#ifdef _EFFECT_OBJ_
#include "../UnitTest++/UnitTest++.h"
#include "../SaiphApp.h"
#include "../Model/Effect.h"
#include <iostream>
using std::cout;

TEST (Effect_Effect)
{
	cout << "Running Test: Effect_Effect...\n\tResult: ";
	bool success;
	Effect * e = new Effect();
	if(e->GetTarget() == NULL)
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
	delete e;
}

TEST (Effect_Clone)
{
	cout << "Running Test: Effect_Clone...\n\tResult: ";
	bool success;
	Effect shield;
	shield.AddFrame("Resources\\images\\shield\\frame1.png", 0.1f);
	shield.AddFrame("Resources\\images\\shield\\frame2.png", 0.1f);
	shield.AddFrame("Resources\\images\\shield\\frame3.png", 0.1f);
	shield.AddFrame("Resources\\images\\shield\\frame4.png", 0.1f);
	shield.AddFrame("Resources\\images\\shield\\frame5.png", 0.1f);
	shield.SetColor(0.25f, 0.5f, 1.0f, 1.0f);
	shield.SetDimensions(32,32);

	// unresolved????
	//shield.SetLooping(false);

	Effect *greenShield = new Effect(shield);
	greenShield->SetColor(1.0f, 0.0f, 1.0f, 0.0f);
	//greenShield->SetLooping(true);

	if(greenShield)
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
	delete greenShield;
}

#endif
#endif BUGS