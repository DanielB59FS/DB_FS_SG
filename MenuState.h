// MenuState.h
// MenuState class for Managing Menu Entities.
// Code by Derek Bliss
// Created April 17, 2008
// Last updated October 15, 2008
// 
#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "AbstractState.h"

class MenuState : public AbstractState
{
	unsigned int PerformSelection();
public:
	// interface methods
	unsigned int Heartbeat(float _delta);		// This method gives menu entities an update
	void Init();								// This method initializes the menu entities
	void RenderFrame() const;					// This method renders all entities to the drawing context
	void Shutdown();							// This method does any necessary clean-up
};

#endif