#ifndef EXITSTATE_H
#define EXITSTATE_H
#include "AbstractState.h"

class BaseObject;

class ExitState : public AbstractState
{
	BaseObject *background;
	BaseObject *cursor;
public:
	virtual ~ExitState(){}
	unsigned int Heartbeat(float _delta);		// This method gives menu entities an update
	void Init();								// This method initializes the menu entities
	void RenderFrame() const;							// This method renders all entities to the drawing context
	void Shutdown();							// This method does any necessary clean-up
};

#endif