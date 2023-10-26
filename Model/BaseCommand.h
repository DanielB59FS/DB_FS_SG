#ifndef	BASECOMMAND_H
#define	BASECOMMAND_H

class Ship;

class BaseCommand {
private:
	Ship* target;
	float delay;

public:
	Ship* GetTarget() const;
	float GetDelay() const;

	void SetTarget(Ship* const _target);
	void SetDelay(const float _delay);

	virtual void Execute() = 0;

	BaseCommand();
};

#endif	// BASECOMMAND_H