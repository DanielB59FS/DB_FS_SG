#ifndef ABSTRACTSTATE_H
#define ABSTRACTSTATE_H

enum STATES {ENDGAME, NOCHANGE, ENTRY, MENU, SHOP, LEVEL, EXIT};

class AbstractState
{
public:
	virtual ~AbstractState() = 0{}

	// interface methods
	virtual unsigned int Heartbeat(float _delta) = 0;		// This method gives game entities an update
	virtual void Init() = 0;								// This method initializes the game entities
	virtual void RenderFrame() const = 0;					// This method renders all entities to the drawing context
	virtual void Shutdown() = 0;							// This method does any necessary clean-up
};

#endif