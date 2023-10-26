#ifndef	SUBJECT_H
#define	SUBJECT_H

enum class ObserverEvent;
class Observer;

class Subject {
private:
	std::vector<Observer*> observers;

public:
	void Attach(Observer* const _ob);
	void Detach(Observer* const _ob);

	void Broadcast(const ObserverEvent _event);

	virtual ~Subject() = 0;
};

#endif	// !SUBJECT_H
