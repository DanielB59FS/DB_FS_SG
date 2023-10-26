#ifndef	HEADINGCOMMAND_H
#define	HEADINGCOMMAND_H

#include "BaseCommand.h"

class HeadingCommand : public BaseCommand{
private:
	float heading;

public:
	float GetHeading() const;
	
	void SetHeading(const float _heading);

	void Execute();

	HeadingCommand();
};

#endif	// HEADINGCOMMAND_H