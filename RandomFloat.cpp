#include "pch.h"
#include "RandomFloat.h"
#include "SaiphApp.h"

RandomFloat::RandomFloat(void)
{
	low = delta = 0;
	noVariation = true;
}

RandomFloat::RandomFloat(float _low, float _high)
{
	SetLimits(_low, _high);
}

// interface methods
void RandomFloat::SetLimits(float _low, float _high)
{
	int x = 3;
	low = _low;
	delta = _high - low;
	if(delta > -FLT_EPSILON && delta < FLT_EPSILON)
		noVariation = true;
	else
		noVariation = false;
}
float RandomFloat::GenerateValue(void) const
{
	if(!noVariation)
		return (rand() / float(RAND_MAX)) * delta + low;
	return low;
}