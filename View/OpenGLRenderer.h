// OpenGLRenderer.h
// Interface for OpenGL rendering implementation.
// Code by Derek Bliss
// Created May 5, 2008
// Last updated October 15, 2008
//
#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "Renderer.h"

class OpenGLRenderer : public Renderer
{

public:

	// interface methods
	void Init();										// Initializes OpenGL
	void BeginScene();									// Clears the screen
	void BeginObjects();								// Enables 2D Textures	(text cannot be rendered)
	void RenderObject(const BaseObject *_object) const;	// Renders one object
	void EndObjects();									// Disables 2D Textures (allows for text to be rendered)
	void RenderText(float _x, float _y, const std::string &_text,
		const Color4f &_color, bool _center) const;		// Renders text at the specified coordinates
	void EndScene();									// Presents the screen
	void Shutdown();									// Cleans up texture memory
	void Restart();										// Invokes Init and Shutdown and reloads all texture memory
};

#endif