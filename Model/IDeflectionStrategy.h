#pragma once


class IDeflectionStrategy
{
public:
	virtual void Deflect(float &bulletHeading) const = 0;
	virtual void Destroy() = 0;
};
