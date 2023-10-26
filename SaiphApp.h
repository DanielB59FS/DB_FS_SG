#ifndef SAIPHAPP_H
#define SAIPHAPP_H

#define WIN32_LEAN_AND_MEAN			// Exclude rarely-used stuff from Windows headers

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#include <windows.h>
#include <string>
#include <float.h>
#include <map>
#include <cmath>
#include <vector>
#include <ctime>
#include <list>

// app specific defines/enums
#define PI			3.141592654f
#define COLLISION_BOXES		0

// Window Variables
#define WINDOW_TITLE	"Saiph"
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define BOUNDS_SIZE		600.0f

// common classes
#include "RandomFloat.h"
class BaseObject;
class Ship;
class Enemy;
class Bullet;
class Effect;
class VerticalMenuList;

// common primatives
struct Vec2f
{
	float x, y;
	Vec2f();
	Vec2f(float _x, float _y);
	// interface
	float Magnitude() const;
	void Normalize();
	void RotateZ(float angle);
	// operators
	Vec2f operator +(const Vec2f &) const;
	Vec2f operator -() const;
	Vec2f operator -(const Vec2f &) const;
	float operator *(const Vec2f &) const;
	Vec2f operator *(float) const;
	Vec2f operator /(float) const;
	const Vec2f &operator +=(const Vec2f &);
	const Vec2f &operator -=(const Vec2f &);
	const Vec2f &operator *=(float);
	const Vec2f &operator /=(float);
};
struct Color4f
{
	float r, g, b, a;
	Color4f();
	Color4f(float _a, float _r, float _g, float _b);
};

// Prototypes
struct BulletPrototype
{
	bool variableHeading;		// the heading of this bullet is random between +/- headingOffset + bullet's source's heading
	bool tracking;				// adjust to hit the nearest target
	float damage;				// damage this bullet does to whatever it hits
	float speed;				// speed of this bullet
	float headingOffset;		// add to bullet's source's heading to allow multidirectional shots
	unsigned int animationID;	// death animation for this bullet
	Vec2f dimensions;			// width and height of image
	Color4f color;
	std::string image;			// image to use for this bullet
	BulletPrototype();
};

// Flyweights
struct WeaponLevelFlyweight
{
	int cost;
	float energy;
	float reloadTime;
	std::vector<BulletPrototype> bullets;
	WeaponLevelFlyweight();
};

struct WeaponFlyweight
{
	std::string name;
	std::vector<WeaponLevelFlyweight> levels;
	WeaponFlyweight();
};

struct ShieldFlyweight
{
	int cost;
	float energy;
	float strength;
	float recharge;
	unsigned int animationID;
	ShieldFlyweight();
};

struct GeneratorFlyweight
{
	int cost;
	float energy;
	float battery;
	GeneratorFlyweight();
};

struct PropulsionFlyweight
{
	int cost;
	float energy;
	float afterburnerEnergy;
	float force;
	PropulsionFlyweight();
};

// App specific functions
namespace SaiphApp
{
	// global math functions
	float GetHeadingFromUnitVector(const Vec2f &);
	Vec2f GetUnitVectorFromHeading(float _heading);
	bool TestCollision(const BaseObject &_left, const BaseObject &_right);

	// App Data accessors
	unsigned int GetShipID();
	unsigned int GetGeneratorID();
	unsigned int GetPropulsionID();
	unsigned int GetShieldID();
	unsigned int GetWeaponID();
	unsigned int GetWeaponLevel();
	unsigned int GetLevel();
	unsigned int GetScore();
	bool GetFullscreen();
	bool GetControl();			// false = polar, true = cartesian
	bool GetRenderer();			// true = directx, false = opengl

	// App Data mutators
	void SetShipID(unsigned int _id);
	void SetGeneratorID(unsigned int _id);
	void SetPropulsionID(unsigned int _id);
	void SetShieldID(unsigned int _id);
	void SetWeaponID(unsigned int _id);
	void SetWeaponLevel(unsigned int _level);
	void SetLevel(unsigned int _level);
	void SetScore(unsigned int _score);
	void SetFullscreen(bool _fullscreen);
	void SetControl(bool _control);
	
	// Sets ViewManager to use directx implementation, previous implementation is discarded	
	void UseDirectX();
	// Sets ViewManager to use opengl implementation, previous implementation is discarded
	void UseOpenGL();

	// Prototype functions
	Ship *CloneShipFromPrototype(unsigned int _shipID, unsigned int _layer = 2);
	std::string GetShipName(unsigned int _shipID);
	float GetShipStartArmor(unsigned int _shipID);
	unsigned int GetNumberOfShipPrototypes();
	void AddEnemyPrototype(Enemy *_enemyPrototype);
	Enemy *CloneEnemyFromPrototype(unsigned int _enemyID, unsigned int _layer = 2);
	unsigned int GetNumberOfEnemyPrototypes();
	Bullet *CloneBulletFromPrototype(const BulletPrototype &_bullet, unsigned int _layer = 1);
	Effect *CloneEffectFromPrototype(unsigned int _animationID, unsigned int _layer = 3);

	// Flyweight functions
	const WeaponFlyweight &GetWeaponFlyweight(unsigned int _weaponID);
	unsigned int GetNumberOfWeaponFlyweights();
	const ShieldFlyweight &GetShieldFlyweight(unsigned int _shieldID);
	unsigned int GetNumberOfShieldFlyweights();
	const GeneratorFlyweight &GetGeneratorFlyweight(unsigned int _generatorID);
	unsigned int GetNumberOfGeneratorFlyweights();
	const PropulsionFlyweight &GetPropulsionFlyweight(unsigned int _propulsionID);
	unsigned int GetNumberOfPropulsionFlyweights();

	// Load functions
	void LoadConfigData(const char * _filename);
	void LoadAnimations(const char * _filename);
	void LoadFlyweights(const char * _filename);
	void LoadEnemyPrototypes(const char *_filename);
	void LoadShips(const char * _filename);
	VerticalMenuList *LoadVerticalMenuList(const char * _filename);
	void LoadVerticalMenuList(const char * _filename, VerticalMenuList &_menu);

	// Clean up functions
	void DestroyEnemyPrototypes();
	void CleanUp();
};

#endif