// Effect.h
// Effect class for managing animation or other special objects
// Code by Derek Bliss
// Created July 14, 2008
// Last updated August 27, 2010
//
#ifndef EFFECT_H
#define EFFECT_H

#include "BaseObject.h"

class Effect : public BaseObject
{
	bool looping;							// True = this effect repeats
	const BaseObject *target;				// Object to "attach" this effect to, set to nullptr if not attaching to an object
	float currentTime;						// Internal timer used to determine the current frame
	unsigned int currentFrame;				// Current frame of this animation to be used for rendering
	std::vector<float> times;				// Contains all times for each frame of the animation
	std::vector<std::string> frames;		// Contains all textures for each frame of the animation
public:
	Effect();
	Effect(const Effect &_e);
	virtual ~Effect();

	// mutators
	void SetLooping(bool _loop);
	void SetTarget(const BaseObject *_bo);

	// accessors
	const BaseObject *GetTarget() const;

	// interface methods
	void AddFrame(const std::string &_filename, float _time);
	virtual void Heartbeat(float _delta);
};

#endif