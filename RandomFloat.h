// RandomFloat.h
// Class that helps with generating random floats within a set range
// Code by Derek Bliss
// Created August 11, 2007
// Last updated April 5, 2008
//
#ifndef RANDOMFLOAT_H
#define RANDOMFLOAT_H

class RandomFloat
{
	float low, delta;		// Range of generated numbers
	bool noVariation;		// Set to true when low == high and RandomFloat is no longer random
public:
	RandomFloat();
	RandomFloat(float _low, float _high);

	// interface methods
	void SetLimits(float _low, float _high);		// Set the range of random numbers generated
	float GenerateValue() const;					// Generate and return a random number
};

#endif