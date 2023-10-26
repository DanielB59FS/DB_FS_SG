#include "pch.h"
#include "VelocityCommand.h"

#include "Ship.h"

Vec2f VelocityCommand::GetVelocity() const {
	return velocity;
}

void VelocityCommand::SetVelocity(const Vec2f _velocity) {
	velocity = _velocity;
}

void VelocityCommand::SetVelocity(const float _x, const float _y) {
	velocity.x = _x;
	velocity.y = _y;
}

void VelocityCommand::Execute() {
	Ship* target = GetTarget();
	if (target) target->SetVelocity(velocity);
}

VelocityCommand::VelocityCommand() : velocity(0.f, 0.f) { }
