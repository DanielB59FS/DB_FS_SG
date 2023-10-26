#ifndef	OBSERVER_H
#define	OBSERVER_H

class Subject;

enum class ObserverEvent { ACTIVATE, SUBJECT_DEATH };

class Observer {
public:
	virtual void Notification(const ObserverEvent _event) = 0;

	virtual ~Observer() = 0;
};

#endif	// !OBSERVER_H
