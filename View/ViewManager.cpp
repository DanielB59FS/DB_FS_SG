#include "pch.h"
#include "ViewManager.h"
#include "Renderer.h"
#include "Layer.h"
#include "../Model/BaseObject.h"
#include <cassert>
#include <wrl\client.h>


// OpenGL libraries
#pragma comment(lib, "opengl32.lib")

// DirectX libraries
#pragma comment (lib, "d3d11.lib")

#if _DEBUG
#pragma comment(lib, "Library_DEBUG_Win32.lib")
#else
#pragma comment(lib, "Library_RELEASE_Win32.lib")
#endif

#define NUM_LAYERS			6

// anonymous namespace used to store 
// current rendering implementation
// and render an object
namespace
{
	Renderer *implementation;

	std::map<const BaseObject *, unsigned int> objects;

	void RenderObject(const BaseObject *_object)
	{
		if(_object->GetActive())
			implementation->RenderObject(_object);
	}
}

ViewManager::ViewManager()
{
}

ViewManager::~ViewManager()
{
}

// interface methods
ViewManager &ViewManager::GetInstance()
{
	static ViewManager vm;
	return vm;
}
void ViewManager::AddObject(const BaseObject *_object, unsigned int _layer)
{
	if(_layer < layers.size() && layers[_layer]->GetActive() && !objects[_object])
	{
		layers[_layer]->AddObject(_object);
		objects[_object] = _layer + 1;
	}
}
bool ViewManager::HasObject(const BaseObject *_object)
{
	bool ret = false;
	if(objects[_object])
		ret = true;
	return ret;
}
void ViewManager::SetLayerActive(unsigned int _layer, bool _active)
{
	if(_layer < layers.size())
		layers[_layer]->SetActive(_active);
}
void ViewManager::RemoveObject(const BaseObject *_object)
{
	if(objects[_object])
	{
		layers[objects[_object] - 1]->RemoveObject(_object);
		objects[_object] = 0;
	}
}
void ViewManager::Init(Renderer *_implementation)
{
	assert(_implementation && "ViewManager must have a valid implementation.");
	implementation = _implementation;
	implementation->Init();

	// Layer data
	for(unsigned i = 0; i < NUM_LAYERS; ++i)
		layers.push_back(new Layer());
}
void ViewManager::BeginScene() const
{
	assert(implementation && "ViewManager must have a valid implementation.");
	implementation->BeginScene();
}
void ViewManager::RenderObjects() const
{
	assert(implementation && "ViewManager must have a valid implementation.");
	implementation->BeginObjects();
	for(unsigned int i = 0; i < layers.size(); ++i)
		layers[i]->Traverse(&RenderObject);
	implementation->EndObjects();
}
void ViewManager::RenderText(float _x, float _y, const std::string &_text, const Color4f &_color, bool _center) const
{
	assert(implementation && "ViewManager must have a valid implementation.");
	implementation->RenderText(_x, _y, _text, _color, _center);
}
void ViewManager::EndScene() const
{
	assert(implementation && "ViewManager must have a valid implementation.");
	implementation->EndScene();
}

void ViewManager::Shutdown()
{
	for(unsigned int i = 0; i < layers.size(); ++i)
		delete layers[i];
	layers.clear();
	if(implementation)
		implementation->Shutdown();
	implementation = 0;
	objects.clear();
}
void ViewManager::Restart(Renderer *_implementation)
{
	if(!implementation && !_implementation)
		assert("ViewManager must have a valid implementation.");
	else if(!implementation && _implementation)
		Init(_implementation);
	else if(_implementation && (implementation != _implementation))
	{
		implementation->Shutdown();
		implementation = _implementation;
		implementation->Init();
	}
	else
		implementation->Restart();
}