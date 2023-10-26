#ifndef ENTRYSTATE_H
#define ENTRYSTATE_H

#include "AbstractState.h"

class BaseObject;

class EntryState : public AbstractState
{
	BaseObject *background;
	BaseObject *title;
	BaseObject *cursor;
public:
	// interface methods
	virtual unsigned int Heartbeat(float _delta);		// This method gives game entities an update
	virtual void Init();								// This method initializes the game entities
	virtual void RenderFrame() const;					// This method renders all entities to the drawing context
	virtual void Shutdown();							// This method does any necessary clean-up
};

#endif