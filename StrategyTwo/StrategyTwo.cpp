// StrategyTwo.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "StrategyTwo.h"

#define PI			3.141592654f

void CStrategyTwo::Deflect(float &bulletHeading) const {
	bulletHeading += PI;
}

void CStrategyTwo::Destroy() {
	delete this;
}

extern "C" STRATEGYTWO_API IDeflectionStrategy* CreateStrategy() {
	return new CStrategyTwo();
}
