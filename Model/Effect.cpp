#include "pch.h"
#include "../Model/Effect.h"

Effect::Effect()
{
	looping = false;
	target = nullptr;
	currentFrame = 0;
	currentTime = 0;
}
Effect::Effect(const Effect &_e)
{
	looping = _e.looping;
	target = _e.target;
	currentFrame = 0;
	currentTime = 0;
	for(unsigned int i = 0; i< _e.frames.size(); ++i)
	{
		times.push_back(_e.times[i]);
		frames.push_back(_e.frames[i]);
	}
}
Effect::~Effect()
{
}

// mutators
void Effect::SetTarget(const BaseObject *_bo) {target = _bo;}

// accessors
const BaseObject *Effect::GetTarget() const {return target;}

// interface methods
void Effect::AddFrame(const std::string &_filename, float _time)
{
	times.push_back(_time);
	frames.push_back(_filename);
}
void Effect::Heartbeat(float _delta)
{
	if(target && target->GetActive())
	{
		SetPosition(target->GetPosition());
	}
	if(frames.size())
	{
		currentTime += _delta;
		if(currentTime >= times[currentFrame])
			currentTime -= times[currentFrame++];
		if(currentFrame >= frames.size())
		{
			if(looping)
				currentFrame = 0;
			else
			{
				currentFrame = (unsigned int)frames.size()-1;
				SetActive(false);
			}
		}
		SetTexture(frames[currentFrame]);
	}
}