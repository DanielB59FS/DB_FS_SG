#include "pch.h"
#include "Layer.h"

Layer::Layer()
{
	SetActive(true);
}

Layer::~Layer()
{
}

// mutators
void Layer::SetActive(bool _active) {active = _active;}
// accessors
bool Layer::GetActive() const {return active;}

// interface methods
void Layer::AddObject(const BaseObject *_object)
{
	if(_object)
		objects.push_back(_object);
}
void Layer::RemoveObject(const BaseObject *_object)
{
	if(_object)
	{
		for(unsigned int i = 0; i < objects.size(); ++i)
		{
			if(objects[i] == _object)
			{
				objects.erase(objects.begin() + i);
				break;
			}
		}
	}
}
void Layer::Traverse(void (* method)(const BaseObject *_object))
{
	for(unsigned int i = 0; i < objects.size(); ++i)
		method(objects[i]);
}