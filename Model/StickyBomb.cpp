#include "pch.h"
#include "../Model/StickyBomb.h"
#include "Enemy.h"
#include "Ship.h"

// used for multiplicative damage in Notification function when bombs are clustered
static std::map<Enemy*, int> numberOfStuckBombsPerEnemy;

StickyBomb::StickyBomb()
{
	SetDamage(0);
	SetDeathAnimation(99);
	SetTexture("Resources/images/missile2.png");
	SetDimensions(16, 16);
	SetColor(1, 1, 1, 1);
	SetSpeed(400);
	stuck = false;
}

StickyBomb::~StickyBomb()
{
	if (target)
		target->Detach(this); // disable future notifications

	Disappear();
}

void StickyBomb::Disappear()
{
	if (target)
	{
		if (stuck)
		{
			numberOfStuckBombsPerEnemy[target]--; // no longer stuck to enemy
			stuck = false;
		}
		target = nullptr; // stop tracking
	}
	SetActive(false); // stop drawing and garbage collect memory
}

void StickyBomb::SetTarget(Enemy* _target)
{
	target = _target;
}

void StickyBomb::Heartbeat(float _delta)	// seek or stick
{
	if (target)
	{
		if (!stuck) // seek
		{
			Vec2f forward = target->GetShip()->GetPosition() - GetPosition();
			forward.Normalize();
			SetHeading(SaiphApp::GetHeadingFromUnitVector(forward));

			Bullet::Heartbeat(_delta);
		}
		else // keep sticking
		{
			SetPosition(target->GetShip()->GetPosition() + offset);
		}
	}
}

void StickyBomb::CollisionResponse()
{
	if (!stuck && target &&
		target->GetShip()->Collide(*this)) // only stick to target
	{
		offset = GetPosition() - target->GetShip()->GetPosition();
		offset *= .8f;	// bring stickybomb closer so it appears on the enemy
		stuck = true;
		numberOfStuckBombsPerEnemy[target]++;
	}
	else if (stuck && GetDamage() > 0) // explosion was triggered
	{
		if (target)
			target->Detach(this); // disable future notifications
		Disappear();
	}
	else
	{
		//otherwise keep sticking and colliding to no effect
	}
}

void StickyBomb::Notification(const ObserverEvent _event)	// explode
{
	if (_event == ObserverEvent::SUBJECT_DEATH) // enemy died via regular bullets
	{
		Disappear();
	}
	else // event == ACTIVATE
	{
		if (!stuck) // detonated mid-flight
		{
			if (target)
				target->Detach(this); // no more notifications
			Disappear();
		}
		else
		{
			float damage = 5;
			int numBombs = numberOfStuckBombsPerEnemy[target];

			// bombs do multiplicative damage when they are grouped together on one subject
			float multiplied = (float)pow(damage, numBombs); // calculate total damage
			damage = multiplied / numBombs; // divide damage across all local stickybombs
			SetDamage(damage); // apply portion of damage to individual stickybomb

			// temporarily inflate bomb to make explosion larger
			SetDimensions(GetDimensions() * (float)numBombs);
			SetDeathAnimation(2);
		}
	}
}
