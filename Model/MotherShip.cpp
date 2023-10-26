#include "pch.h"
#include "MotherShip.h"

#include "../View/ViewManager.h"

#include "VelocityCommand.h"
#include "HeadingCommand.h"

void MotherShip::ProcessCommands(float _delta) {
	auto _iter = commands.begin();
	while (commands.end() != _iter) {
		BaseCommand* _cmd = *_iter;
		_cmd->SetDelay(_cmd->GetDelay() - _delta);
		if (_cmd->GetDelay() <= 0) {
			_cmd->Execute();
			_iter = commands.erase(_iter);
			delete _cmd;
		}
		else
			++_iter;
	}
}

void MotherShip::CommandsCleanUp() {
	for (BaseCommand* _cmd : commands) delete _cmd;
	commands.clear();

	for (Ship* _shadow : ships) {
		_shadow->SetVelocity(0.f, 0.f);
		_shadow->SetPosition(GetPosition());
		_shadow->SetHeading(GetHeading());
	}
}

void MotherShip::AddShadowShip(Ship* const _shadow) {
	if (_shadow) ships.push_back(_shadow);
}

void MotherShip::Heartbeat(float _delta) {
	Ship::Heartbeat(_delta);
	ProcessCommands(_delta);
	if (GetAfterburnerFlag()) {
		VelocityCommand* _velocity;
		HeadingCommand* _heading;
		float _delay = 0.05f;
		for (Ship* _shadow : ships) {
			_velocity = new VelocityCommand();
			_heading = new HeadingCommand();

			commands.push_back(_velocity);
			commands.push_back(_heading);

			_velocity->SetTarget(_shadow);
			_heading->SetTarget(_shadow);

			_velocity->SetVelocity(GetVelocity());
			_heading->SetHeading(GetHeading());

			_velocity->SetDelay(_delay);
			_heading->SetDelay(_delay);
			_delay += 0.05f;

			_shadow->Heartbeat(_delta);
		}
	}
	else
		CommandsCleanUp();
}

MotherShip::MotherShip(const Ship& _ship) : Ship(_ship) { }

MotherShip::~MotherShip() {
	CommandsCleanUp();

	ViewManager& _manager = ViewManager::GetInstance();
	for (Ship* _shadow : ships) {
		_manager.RemoveObject(_shadow);
		delete _shadow;
	}
	ships.clear();
}
