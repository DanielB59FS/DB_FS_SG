#ifndef BUGS
#include "Defines.h"

#ifdef _BASEOBJECT_OBJ_
#include "../UnitTest++/UnitTest++.h"
#include "../SaiphApp.h"
#include "../Model/BaseObject.h"
#include <iostream>
using std::cout;

// positive black box test
// checks for simple color setting
TEST(BaseObject_SetColor)
{
	cout <<"Running Test: BaseObject_SetColor...\n\tResult: ";
	bool success;
	BaseObject base;
	float r = 0.5f;
	float g = 0.1f;
	float b = 0.4f;
	
	base.SetColor(1.0f,r,g,b);
	
	if(base.GetRed() == r && base.GetBlue() == b && base.GetGreen() == g)
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