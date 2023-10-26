#ifndef	VELOCITYCOMMAND_H
#define	VELOCITYCOMMAND_H

#include "BaseCommand.h"

class VelocityCommand : public BaseCommand {
private:
	Vec2f velocity;

public:
	Vec2f GetVelocity() const;
	
	void SetVelocity(const Vec2f _velocity);
	void SetVelocity(const float _x, const float _y);

	void Execute();

	VelocityCommand();
};

#endif	// VELOCITYCOMMAND_H