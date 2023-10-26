// Renderer.h
// Abstract interface for specific rendering implementation.
// Code by Derek Bliss
// Created May 5, 2008
// Last updated October 7, 2008
//
#ifndef RENDERER_H
#define RENDERER_H

class BaseObject;

class Renderer
{

public:
	// destructor
	virtual ~Renderer() = 0 {}

	// interface methods
	virtual void Init() = 0;										// Initializes this implementation
	virtual void BeginScene() = 0;									// Clears the screen and readies the implementation for a frame
	virtual void BeginObjects() = 0;								// Readies the implementation for rendering views
	virtual void RenderObject(const BaseObject *_object) const = 0;	// Renders one object
	virtual void EndObjects() = 0;									// Resets the implementation once all views have been rendered
	virtual void RenderText(float _x, float _y, const std::string &_text,
		const Color4f &_color, bool _center) const = 0;				// Renders text at the specified coordinates
	virtual void EndScene() = 0;									// Presents the screen and resets render perameters
	virtual void Shutdown() = 0;									// Cleans up any memory used in this implementation
	virtual void Restart() = 0;										// Invokes Init and Shutdown and reloads all texture memory
};

#endif