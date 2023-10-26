#include "pch.h"
#include "HeadingCommand.h"

#include "Ship.h"

float HeadingCommand::GetHeading() const {
	return heading;
}

void HeadingCommand::SetHeading(const float _heading) {
	heading = _heading;
}

void HeadingCommand::Execute() {
	Ship* target = GetTarget();
	if (target) target->SetHeading(heading);
}

HeadingCommand::HeadingCommand() : heading(0.f) { }
