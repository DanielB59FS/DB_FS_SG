// StrategyOne.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "StrategyOne.h"

#include <cstdlib>

#define PI			3.141592654f

void CStrategyOne::Deflect(float &bulletHeading) const {
	bulletHeading = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2*PI;
}

void CStrategyOne::Destroy() {
	delete this;
}

extern "C" STRATEGYONE_API IDeflectionStrategy* CreateStrategy() {
	return new CStrategyOne();
}
