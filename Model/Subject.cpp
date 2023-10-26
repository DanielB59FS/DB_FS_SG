#include "pch.h"
#include "Subject.h"

#include "Observer.h"

#include <algorithm>

void Subject::Attach(Observer* const _ob) {
	if (_ob)
		observers.push_back(_ob);
}

void Subject::Detach(Observer* const _ob) {
	auto iter = std::find(observers.begin(), observers.end(), _ob);
	if (iter != observers.end()) observers.erase(iter);
}

void Subject::Broadcast(const ObserverEvent _event) {
	for (Observer* ob : observers) ob->Notification(_event);
}

Subject::~Subject() {
	Broadcast(ObserverEvent::SUBJECT_DEATH);
}
