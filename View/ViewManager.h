// ViewManager.h
// Interface for managing render lists and the rendering implementation
// Code by Derek Bliss
// Created May 5, 2008
// Last updated October 24, 2008
//
#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

class Renderer;
class Layer;
class BaseObject;

class ViewManager
{
	/*
	A Layer represents a collection or render list of objects
	Layers should be used as follows:
	Layer 0 Background layer
	Layer 1 Projectiles
	Layer 2 Characters and Power Ups
	Layer 3 Special Effects
	Layer 4 Menus and H.U.D.
	Layer 5 Cursor / Menus and H.U.D. 2
	Text displays in relation to where RenderText is called.
	*/
	std::vector<Layer *>				layers;

	// default constructor
	ViewManager();

	// copy constructor
	ViewManager(const ViewManager &);

	// destructor
	~ViewManager();

public:

	// interface methods
	
	// Provides access to this singleton
	// Note: This returns a reference and not a pointer.
	static ViewManager &GetInstance();
	
	// Adds the specified object to a specified layer to be rendered.
	// Note: This will do nothing if _layer is out of bounds or if
	// the object has already been added
	void AddObject(const BaseObject *_object, unsigned int _layer);

	// Returns true if specified object is in a render list.
	bool HasObject(const BaseObject *_object);

	// Sets the active flag of the layer at the given index
	// Only active layers are available to be drawn/manipulated
	// Note: This will do nothing if _layer is out of bounds.
	void SetLayerActive(unsigned int _layer, bool _active);

	// Removes object from any layer if exists
	// (object will no longer be rendered)
	void RemoveObject(const BaseObject *_object);

	// Initializes the ViewManager to use a specified implementation
	// Check Renderer.h for interface requirements for render implementations
	// Note: This is invoked for you, DO NOT CALL THIS METHOD
	void Init(Renderer *_implementation);

	// Begins the current frame for rendering
	void BeginScene() const;

	// Attempts to render all objects of all layers
	// Note: This must be invoked between a BeginScene() and EndScene()
	void RenderObjects() const;

	// Displays text at the specified coordinates
	// Color is optional and the default is white
	void RenderText(float _x, float _y, const std::string &_text,
		const Color4f &_color = Color4f(), bool _center = false) const;

	// Ends the current frame for rendering
	void EndScene() const;

	// Removes all objects and invokes shutdown on the current render
	// implementation.
	// Note: This is invoked for you, DO NOT CALL THIS METHOD
	void Shutdown();

	// If _implementation is not NULL then this will invoke shutdown
	// on the current implementation, change the current implemation
	// to the new provided _implementation and invokes init on the
	// new implementation.  If _implementation is NULL then this will
	// invoke restart on the current implemention to allow for
	// changes in render target size (switch to/from fullscreen)
	// Note: This is invoked for you, DO NOT CALL THIS METHOD
	void Restart(Renderer *_implementation = NULL);
};

#endif