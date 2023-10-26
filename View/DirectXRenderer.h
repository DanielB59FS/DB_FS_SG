#ifndef DIRECTXRENDERER_H
#define DIRECTXRENDERER_H

#include "Renderer.h"

class DirectXRenderer : public Renderer
{

public:

	// interface methods
	void Init();										// Initializes DirectX
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