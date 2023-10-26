#include "pch.h"
#include "BaseCommand.h"

Ship* BaseCommand::GetTarget() const {
	return target;
}

float BaseCommand::GetDelay() const {
	return delay;
}

void BaseCommand::SetTarget(Ship* const _target) {
	target = _target;
}

void BaseCommand::SetDelay(const float _delay) {
	delay = _delay;
}

BaseCommand::BaseCommand() : target(NULL), delay(0.f) { }
