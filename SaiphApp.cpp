#include "pch.h"
#include <fstream>
#include "XML_Library/irrXML.h"
#include "View/ViewManager.h"
#include "View/OpenGLRenderer.h"
#include "View/DirectXRenderer.h"
#include "Model/Ship.h"
#include "Model/Bullet.h"
#include "Model/Menu.h"
#include "Model/Effect.h"
#include "Model/Enemy.h"

namespace
{
	// render implemenations
	OpenGLRenderer openGLRenderer;			// OpenGL specific render implementation for this application
	DirectXRenderer directXRenderer;		// DirectX specific render implementation for this application

	// App data variables
	bool directX;
	bool fullscreen;
	bool control;
	unsigned int shipID;
	unsigned int generatorID;
	unsigned int propulsionID;
	unsigned int shieldID;
	unsigned int weaponID;
	unsigned int weaponLevel;
	unsigned int level;
	unsigned int score;

	// prototypes
	std::vector<Ship *> shipPrototypes;
	std::vector<Enemy *> enemyPrototypes;
	std::vector<Effect *> animationPrototypes;
	std::vector<std::string> shipNames;

	// flyweights
	WeaponFlyweight weaponDefault;
	ShieldFlyweight shieldDefault;
	GeneratorFlyweight generatorDefault;
	PropulsionFlyweight propulsionDefault;
	std::vector<WeaponFlyweight> weaponFlyweights;
	std::vector<ShieldFlyweight> shieldFlyweights;
	std::vector<GeneratorFlyweight> generatorFlyweights;
	std::vector<PropulsionFlyweight> propulsionFlyweights;
	
	// vector for initial loading of textures
	std::vector<BaseObject *> views;

	MenuItem *CreateMenuItem(const std::string &_texture)
	{
		MenuItem *menuItem = new MenuItem();
		menuItem->SetTexture(_texture);
		menuItem->SetDimensions(10,10);
		return menuItem;
	}

}

// Vec2f
Vec2f::Vec2f()
{
	x = y = 0;
}
Vec2f::Vec2f(float _x, float _y)
{
	x = _x;
	y = _y;
}
float Vec2f::Magnitude() const
{
	return sqrtf(this->operator *(*this));
}
void Vec2f::Normalize()
{
	float m = Magnitude();
	if(m) *this = this->operator /(m);
}
void Vec2f::RotateZ(float angle)
{
	float temp, cosine = cosf(angle), sine = sinf(angle);
	temp = x;
	x = temp * cosine - y * sine;
	y = temp * sine + y * cosine;
}
Vec2f Vec2f::operator +(const Vec2f &_r) const
{
	return Vec2f(x + _r.x, y + _r.y);
}
Vec2f Vec2f::operator -() const
{
	return Vec2f(-x, -y);
}
Vec2f Vec2f::operator -(const Vec2f &_r) const
{
	return Vec2f(x - _r.x, y - _r.y);
}
float Vec2f::operator *(const Vec2f &_r) const
{
	return x * _r.x + y * _r.y;
}
Vec2f Vec2f::operator *(float _r) const
{
	return Vec2f(x * _r, y *_r);
}
Vec2f Vec2f::operator /(float _r) const
{
	return Vec2f(x / _r, y / _r);
}
const Vec2f &Vec2f::operator +=(const Vec2f &_r)
{
	x += _r.x;
	y += _r.y;
	return *this;
}
const Vec2f &Vec2f::operator -=(const Vec2f &_r)
{
	x -= _r.x;
	y -= _r.y;
	return *this;
}
const Vec2f &Vec2f::operator *=(float _r)
{	
	return (*this) = this->operator *(_r);
}
const Vec2f &Vec2f::operator /=(float _r)
{
	return (*this) = this->operator /(_r);
}

// Color4f
Color4f::Color4f()
{
	a = r = g = b = 1;
}
Color4f::Color4f(float _a, float _r, float _g, float _b)
{
	a = _a;
	r = _r;
	g = _g;
	b = _b;
}

// Prototypes

BulletPrototype::BulletPrototype()
:variableHeading()
,tracking()
,speed()
,headingOffset()
,animationID()
{}

// Flyweights

WeaponFlyweight::WeaponFlyweight()
{
	levels.push_back(WeaponLevelFlyweight());
}

WeaponLevelFlyweight::WeaponLevelFlyweight()
:cost()
,energy()
,reloadTime()
{}

ShieldFlyweight::ShieldFlyweight()
:cost()
,energy()
,strength()
,recharge()
,animationID()
{}

GeneratorFlyweight::GeneratorFlyweight()
:cost()
,energy(1)
,battery(10)
{}

PropulsionFlyweight::PropulsionFlyweight()
:cost()
,energy(0.1f)
,afterburnerEnergy()
,force(75)
{}

namespace SaiphApp
{
	// App data accessors
	bool GetFullscreen() {return fullscreen;}
	bool GetControl() {return control;}
	unsigned int GetShipID() {return shipID;}
	unsigned int GetGeneratorID() {return generatorID;}
	unsigned int GetPropulsionID() {return propulsionID;}
	unsigned int GetShieldID() {return shieldID;}
	unsigned int GetWeaponID() {return weaponID;}
	unsigned int GetWeaponLevel() {return weaponLevel;}
	unsigned int GetLevel() {return level;}
	unsigned int GetScore() {return score;}
	bool GetRenderer() {return directX;}

	// App data mutators
	void SetFullscreen(bool _fullscreen)
	{
		fullscreen = _fullscreen;
		if(GetRenderer())
			UseDirectX();
		else
			UseOpenGL();
	}
	void SetControl(bool _control) {control = _control;}
	void SetShipID(unsigned int _id) {shipID = _id;}
	void SetGeneratorID(unsigned int _id) {generatorID = _id;}
	void SetPropulsionID(unsigned int _id) {propulsionID = _id;}
	void SetShieldID(unsigned int _id) {shieldID = _id;}
	void SetWeaponID(unsigned int _id) {weaponID = _id;}
	void SetWeaponLevel(unsigned int _level) {weaponLevel = _level;}
	void SetLevel(unsigned int _level) {level = _level;}
	void SetScore(unsigned int _score) {score = _score;}

	// Sets ViewManager to use directX implementation, previous implementation is discarded	
	void UseDirectX()
	{
		directX = true;
		if(SaiphApp::GetFullscreen())
			// resize the window so it takes up the entire screen
			MoveWindow(FindWindow(WINDOW_TITLE, WINDOW_TITLE)
				, 0, 0, GetSystemMetrics(SM_CXSCREEN)
				, GetSystemMetrics(SM_CYSCREEN), TRUE);
		else
			// resize the window to fit WINDOW_WIDTH and WINDOW_HEIGHT
			MoveWindow(FindWindow(WINDOW_TITLE, WINDOW_TITLE)
				, (GetSystemMetrics(SM_CXSCREEN)>>1) - (WINDOW_WIDTH>>1)
				, (GetSystemMetrics(SM_CYSCREEN)>>1) - (WINDOW_HEIGHT>>1)
				, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
		ViewManager::GetInstance().Restart(&directXRenderer);
	}
	// Sets ViewManager to use openGL implementation, previous implementation is discarded
	void UseOpenGL()
	{
		directX = false;
		if(SaiphApp::GetFullscreen())
			// resize the window so it takes up the entire screen
			MoveWindow(FindWindow(WINDOW_TITLE, WINDOW_TITLE)
				, 0, 0, GetSystemMetrics(SM_CXSCREEN)
				, GetSystemMetrics(SM_CYSCREEN), TRUE);
		else
			// resize the window to fit WINDOW_WIDTH and WINDOW_HEIGHT
			MoveWindow(FindWindow(WINDOW_TITLE, WINDOW_TITLE)
				, (GetSystemMetrics(SM_CXSCREEN)>>1) - (WINDOW_WIDTH>>1)
				, (GetSystemMetrics(SM_CYSCREEN)>>1) - (WINDOW_HEIGHT>>1)
				, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
		ViewManager::GetInstance().Restart(&openGLRenderer);
	}

	// global math functions
	float GetHeadingFromUnitVector(const Vec2f &_v)
	{
		float ret= 0;
		if(_v.y > 0)
			ret = acosf(_v.x);
		else
			ret = -acosf(_v.x);
		// Make sure that the heading is not -1.#IND00000
		// This means that it is indefinate (not a number)
		if(_isnan(ret))
			ret = 0;
		return ret;
	}
	Vec2f GetUnitVectorFromHeading(float _heading)
	{
		return Vec2f(cosf(_heading), sinf(_heading));
	}
	// collision algorithm obtained from http://www.ragestorm.net/tutorial?id=22
	// altered and refactored for SWA use
	bool TestCollision(const BaseObject &_left, const BaseObject &_right)
	{
		Vec2f RotatedA, RotatedB,							// vertices of the rotated rr1
			INCenter,										// center of rr2
			INBottomLeft, INTopRight;						// vertices of rr2 (bottom-left, top-right)
		float INHeading = _left.GetHeading() - _right.GetHeading(),	// heading for rr1
			cosine = cosf(INHeading),						// precalculated trigonometic -
			sine = sinf(INHeading),							// - values for repeated use
			temp1, temp2, temp3,							// temporaries for various uses
			deltaX,											// deltaX for linear equations
			minVertical, maxVertical;						// min/max vertical values

		Vec2f forward(cosine, sine);

		// move rr2 to make rr1 cannonic
		INCenter = _right.GetPosition() - _left.GetPosition();

		// rotate rr2 clockwise by rr2->ang to make rr2 axis-aligned
		INCenter.RotateZ(_right.GetHeading());
		// calculate vertices of (moved and axis-aligned := 'ma') rr2
		INBottomLeft = INTopRight = INCenter;
		INBottomLeft -= _right.GetDimensions();
		INTopRight += _right.GetDimensions();

		// calculate vertices of (rotated := 'r') rr1
		RotatedA.x = -_left.GetHeight() * 0.333f * sine + _left.GetWidth() * 0.333f  * cosine;
		RotatedA.y = _left.GetHeight() * 0.333f  * cosine + _left.GetWidth() * 0.333f  * sine;
		RotatedB.x = -_left.GetHeight() * 0.333f  * sine - _left.GetWidth() * 0.333f  * cosine;
		RotatedB.y = _left.GetHeight() * 0.333f  * cosine - _left.GetWidth() * 0.333f  * sine;

		// verify that A is vertical min/max, B is horizontal min/max
		temp1 = sine * cosine;
		if(temp1 < 0)
		{
			Vec2f temp = RotatedA;
			RotatedA = RotatedB;
			RotatedB = temp;
		}

		// verify that B is horizontal minimum (leftest-vertex)
		if(sine < 0)
		{
			RotatedB = -RotatedB;
		}

		// if rr2(ma) isn't in the horizontal range of
		// colliding with rr1(r), collision is impossible
		if(RotatedB.x > INTopRight.x || RotatedB.x > -INBottomLeft.x)
			return false;

		// if rr1(r) is axis-aligned, vertical min/max are easy to get
		if (!temp1)
		{
			minVertical = RotatedA.y;
			maxVertical = -minVertical;
		}
		// else, find vertical min/max in the range [BL.x, TR.x]
		else
		{
			temp2 = INBottomLeft.x - RotatedA.x;
			temp3 = INTopRight.x - RotatedA.x;
			minVertical = RotatedA.y;
			// if the first vertical min/max isn't in (BL.x, TR.x), then
			// find the vertical min/max on BL.x or on TR.x
			if (temp3*temp2 > 0)
			{
				deltaX = RotatedA.x;
				if (temp2 < 0)
				{
					deltaX -= RotatedB.x;
					minVertical -= RotatedB.y;
					temp2 = temp3;
				}
				else
				{
					deltaX += RotatedB.x;
					minVertical += RotatedB.y;
				}
				minVertical *= temp2;
				minVertical /= deltaX;
				minVertical += RotatedA.y;
			}

			temp2 = INBottomLeft.x + RotatedA.x;
			temp3 = INTopRight.x + RotatedA.x;
			maxVertical = -RotatedA.y;
			// if the second vertical min/max isn't in (BL.x, TR.x), then
			// find the local vertical min/max on BL.x or on TR.x
			if (temp3*temp2 > 0)
			{
				deltaX = -RotatedA.x;
				if (temp2 < 0)
				{
					deltaX -= RotatedB.x;
					maxVertical -= RotatedB.y;
					temp2 = temp3;
				}
				else
				{
					deltaX += RotatedB.x;
					maxVertical += RotatedB.y;
				}
				maxVertical *= temp2;
				maxVertical /= deltaX;
				maxVertical -= RotatedA.y;
			}
		}

		// check whether rr2(ma) is in the vertical range of colliding with rr1(r)
		// (for the horizontal range of rr2)
		return !((minVertical < INBottomLeft.y && maxVertical < INBottomLeft.y) ||
			(minVertical > INTopRight.y && maxVertical > INTopRight.y));
	}


	// Prototype methods
	Ship *CloneShipFromPrototype(unsigned int _shipID, unsigned int _layer)
	{
		Ship *ret = NULL;
		if(_shipID < shipPrototypes.size())
		{
			ret = new Ship(*shipPrototypes[_shipID]);
			ret->SetLastFired(0);
			ViewManager::GetInstance().AddObject(ret, _layer);
		}
		else
			ret = new Ship();
		return ret;
	}
	std::string GetShipName(unsigned int _shipID)
	{
		if(_shipID < shipPrototypes.size())
			return shipNames[_shipID];
		return "";
	}
	float GetShipStartArmor(unsigned int _shipID)
	{
		if(_shipID < shipPrototypes.size())
			return shipPrototypes[_shipID]->GetArmor();
		return 0;
	}
	unsigned int GetNumberOfShipPrototypes()
	{
		return (unsigned int)shipPrototypes.size();
	}
	Enemy *CloneEnemyFromPrototype(unsigned int _enemyID, unsigned int _layer)
	{
		Enemy *ret = NULL;
		if(_enemyID < enemyPrototypes.size())
		{
			ret = enemyPrototypes[_enemyID]->Clone();
			ret->SetShip(CloneShipFromPrototype(ret->GetShipID(), _layer));
		}
		else
			ret = new Enemy();
		return ret;
	}
	unsigned int GetNumberOfEnemyPrototypes()
	{
		return (unsigned int)enemyPrototypes.size();
	}
	Bullet *CloneBulletFromPrototype(const BulletPrototype &_bullet, unsigned int _layer)
	{
		Bullet *ret = new Bullet();
		ViewManager::GetInstance().AddObject(ret, _layer);
		ret->SetTexture(_bullet.image);
		ret->SetColor(_bullet.color);
		ret->SetDimensions(_bullet.dimensions);
		ret->SetDamage(_bullet.damage);
		if(_bullet.variableHeading)
			ret->SetHeading(RandomFloat(-_bullet.headingOffset, _bullet.headingOffset).GenerateValue());
		else
			ret->SetHeading(_bullet.headingOffset);
		ret->SetTracking(_bullet.tracking);
		ret->SetSpeed(_bullet.speed);
		ret->SetDeathAnimation(_bullet.animationID);
		return ret;
	}
	Effect *CloneEffectFromPrototype(unsigned int _animationID, unsigned int _layer)
	{
		Effect *effect = NULL;
		if(_animationID < animationPrototypes.size())
		{
			effect = new Effect(*animationPrototypes[_animationID]);
			ViewManager::GetInstance().AddObject(effect, _layer);
			effect->Heartbeat(0);
		}
		return effect;
	}

	// Flyweight methods
	const WeaponFlyweight &GetWeaponFlyweight(unsigned int _weaponID)
	{
		if(_weaponID < weaponFlyweights.size())
			return weaponFlyweights[_weaponID];
		return weaponDefault;
	}
	unsigned int GetNumberOfWeaponFlyweights() {return (unsigned int)weaponFlyweights.size();}
	const ShieldFlyweight &GetShieldFlyweight(unsigned int _shieldID)
	{
		if(_shieldID < shieldFlyweights.size())
			return shieldFlyweights[_shieldID];
		return shieldDefault;
	}
	unsigned int GetNumberOfShieldFlyweights() {return (unsigned int)shieldFlyweights.size();}
	const GeneratorFlyweight &GetGeneratorFlyweight(unsigned int _generatorID)
	{
		if(_generatorID < generatorFlyweights.size())
			return generatorFlyweights[_generatorID];
		return generatorDefault;
	}
	unsigned int GetNumberOfGeneratorFlyweights() {return (unsigned int)generatorFlyweights.size();}
	const PropulsionFlyweight &GetPropulsionFlyweight(unsigned int _propulsionID)
	{
		if(_propulsionID < propulsionFlyweights.size())
			return propulsionFlyweights[_propulsionID];
		return propulsionDefault;
	}
	unsigned int GetNumberOfPropulsionFlyweights() {return (unsigned int)propulsionFlyweights.size();}


	// Load functions
	void LoadConfigData(const char * _filename)
	{
		//const char *valueAsString;
		//std::string valueAsString;
		//double valueAsDouble;
		irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename);
		while(xmlReader->read())
		{
			if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
			{
				if(!strcmp("Window", xmlReader->getNodeName()))
				{
					if(xmlReader->getAttributeValue("renderer"))
					{
						if(!strcmp(xmlReader->getAttributeValue("renderer"), "directx"))
							UseDirectX();
						else
							UseOpenGL();
					}
					if(xmlReader->getAttributeValue("fullscreen"))
					{
						if(!strcmp(xmlReader->getAttributeValue("fullscreen"), "true"))
							SetFullscreen(true);
						else
							SetFullscreen(false);
					}
				}
				else if(!strcmp("StartData", xmlReader->getNodeName()))
				{
					SetLevel(xmlReader->getAttributeValueAsInt("level"));
					SetScore(xmlReader->getAttributeValueAsInt("score"));
					SetShipID(xmlReader->getAttributeValueAsInt("ship"));
					SetWeaponID(xmlReader->getAttributeValueAsInt("weapon"));
					SetWeaponLevel(xmlReader->getAttributeValueAsInt("weaponLevel"));
					SetGeneratorID(xmlReader->getAttributeValueAsInt("generator"));
					SetShieldID(xmlReader->getAttributeValueAsInt("shield"));
					SetPropulsionID(xmlReader->getAttributeValueAsInt("propulsion"));
				}
				else if(!strcmp("Control", xmlReader->getNodeName()))
				{
					if(xmlReader->getAttributeValue("style"))
					{
						if(!strcmp(xmlReader->getAttributeValue("style"), "cartesian"))
							SetControl(true);
						else
							SetControl(false);
					}
				}
			}
		}
		delete xmlReader;
		if(SaiphApp::GetFullscreen())
		{
			// resize the window so it takes up the entire screen
			MoveWindow(FindWindow(WINDOW_TITLE, WINDOW_TITLE)
				, 0, 0, GetSystemMetrics(SM_CXSCREEN)
				, GetSystemMetrics(SM_CYSCREEN), TRUE);
			ViewManager::GetInstance().Restart();
		}
	}
	void LoadAnimations(const char *_filename)
	{
		views.clear();
		bool tag = false;
		irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename);
		Effect *anim;
		while(xmlReader->read())
		{
			if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
			{
				if(tag)
				{
					if(!strcmp("Frame", xmlReader->getNodeName()))
					{
						if(xmlReader->getAttributeValue("filename"))
						{
							anim->AddFrame(xmlReader->getAttributeValue("filename"), xmlReader->getAttributeValueAsFloat("time"));
							//views.push_back(new BaseObject());
							//ViewManager::GetInstance().AddObject(views.back(), 0);
							//views.back()->SetTexture(xmlReader->getAttributeValue("filename"));
						}
					}
				}
				else
				{
					if(!strcmp("Animation", xmlReader->getNodeName()))
					{
						anim = new Effect();
						anim->SetColor(xmlReader->getAttributeValueAsFloat("alpha")
							,xmlReader->getAttributeValueAsFloat("red")
							,xmlReader->getAttributeValueAsFloat("green")
							,xmlReader->getAttributeValueAsFloat("blue"));
						anim->SetDimensions(xmlReader->getAttributeValueAsFloat("width")
							,xmlReader->getAttributeValueAsFloat("height"));
						tag = true;
					}
				}
			}
			else if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT_END)
			{
				if(!strcmp("Animation", xmlReader->getNodeName()))
				{
					animationPrototypes.push_back(anim);
					anim = NULL;
					tag = false;
				}
			}
		}
		delete xmlReader;
		//ViewManager::GetInstance().BeginScene();
		//ViewManager::GetInstance().RenderObjects();
		//ViewManager::GetInstance().EndScene();
		//for(unsigned int i = 0; i < views.size(); ++i)
		//{
		//	ViewManager::GetInstance().RemoveObject(views[i]);
		//	delete views[i];
		//}
		//views.clear();
	}
	void LoadFlyweights(const char *_filename)
	{
		//views.clear();

		enum {NONE, WEAPON, LEVEL};
		unsigned int tagFlag = NONE;
		WeaponFlyweight currentWeapon;
		WeaponLevelFlyweight currentWeaponLevel;
		irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename);
		while(xmlReader->read())
		{
			if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
			{
				switch(tagFlag)
				{
				case NONE:
					if(!strcmp("Weapon", xmlReader->getNodeName()))
					{
						tagFlag = WEAPON;
						currentWeapon.levels.clear();
						xmlReader->getAttributeValue("name") ?
							currentWeapon.name = xmlReader->getAttributeValue("name")
							: currentWeapon.name = "";
					}
					else if(!strcmp("Shield", xmlReader->getNodeName()))
					{
						ShieldFlyweight sf;
						sf.cost = xmlReader->getAttributeValueAsInt("cost");
						sf.energy = xmlReader->getAttributeValueAsFloat("energy");
						sf.strength = xmlReader->getAttributeValueAsFloat("strength");
						sf.recharge = xmlReader->getAttributeValueAsFloat("recharge");
						sf.animationID = xmlReader->getAttributeValueAsInt("animation");
						shieldFlyweights.push_back(sf);
					}
					else if(!strcmp("Propulsion", xmlReader->getNodeName()))
					{
						PropulsionFlyweight pf;
						pf.cost = xmlReader->getAttributeValueAsInt("cost");
						pf.energy = xmlReader->getAttributeValueAsFloat("energy");
						pf.afterburnerEnergy = xmlReader->getAttributeValueAsFloat("afterburner");
						pf.force = xmlReader->getAttributeValueAsFloat("force");
						propulsionFlyweights.push_back(pf);
					}
					else if(!strcmp("Generator", xmlReader->getNodeName()))
					{
						GeneratorFlyweight gf;
						gf.cost = xmlReader->getAttributeValueAsInt("cost");
						gf.battery = xmlReader->getAttributeValueAsFloat("battery");
						gf.energy = xmlReader->getAttributeValueAsFloat("energy");
						generatorFlyweights.push_back(gf);
					}
					break;
				case WEAPON:
					if(!strcmp("Level", xmlReader->getNodeName()))
					{
						currentWeaponLevel.bullets.clear();
						currentWeaponLevel.cost = xmlReader->getAttributeValueAsInt("cost");
						currentWeaponLevel.energy = xmlReader->getAttributeValueAsFloat("energy");
						currentWeaponLevel.reloadTime = xmlReader->getAttributeValueAsFloat("reload");
						tagFlag = LEVEL;
					}
					break;
				case LEVEL:
					if(!strcmp("Bullet", xmlReader->getNodeName()))
					{
						BulletPrototype bp;
						bp.variableHeading = (!strcmp("true", xmlReader->getAttributeValue("variable")));
						bp.tracking = (!strcmp("true", xmlReader->getAttributeValue("tracking")));
						bp.damage = xmlReader->getAttributeValueAsFloat("damage");
						bp.headingOffset = xmlReader->getAttributeValueAsFloat("offset");
						bp.speed = xmlReader->getAttributeValueAsFloat("speed");
						bp.animationID = xmlReader->getAttributeValueAsInt("animation");
						bp.dimensions.x = xmlReader->getAttributeValueAsFloat("width");
						bp.dimensions.y = xmlReader->getAttributeValueAsFloat("height");
						bp.image = xmlReader->getAttributeValue("texture");
						Color4f c(xmlReader->getAttributeValueAsFloat("alpha"),
							xmlReader->getAttributeValueAsFloat("red"),
							xmlReader->getAttributeValueAsFloat("green"),
							xmlReader->getAttributeValueAsFloat("blue"));
						if(c.a || c.r || c.g || c.b)
							bp.color = c;
						currentWeaponLevel.bullets.push_back(bp);
						//views.push_back(new BaseObject());
						//ViewManager::GetInstance().AddObject(views.back(), 0);
						//views.back()->SetTexture(bp.image);
						//views.back()->SetDimensions(bp.dimensions);
					}
					break;
				default:
					break;
				};
			}
			else if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT_END)
			{
				if(!strcmp("Weapon", xmlReader->getNodeName()) && tagFlag == WEAPON)
				{
					tagFlag = NONE;
					weaponFlyweights.push_back(currentWeapon);
				}
				else if(!strcmp("Level", xmlReader->getNodeName()) && tagFlag == LEVEL)
				{
					tagFlag = WEAPON;
					currentWeapon.levels.push_back(currentWeaponLevel);
				}
			}
		}
		delete xmlReader;
		//ViewManager::GetInstance().BeginScene();
		//ViewManager::GetInstance().RenderObjects();
		//ViewManager::GetInstance().EndScene();
		//for(unsigned int i = 0; i < views.size(); ++i)
		//{
		//	ViewManager::GetInstance().RemoveObject(views[i]);
		//	delete views[i];
		//}
		//views.clear();
	}
	void LoadShips(const char *_filename)
	{
		irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename);
		while(xmlReader->read())
		{
			if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
			{
				if(!strcmp("Ship", xmlReader->getNodeName()))
				{
					Ship *newship = new Ship();
					newship->SetCost(xmlReader->getAttributeValueAsInt("cost"));
					newship->SetArmor(xmlReader->getAttributeValueAsFloat("armor"));
					newship->SetTexture(xmlReader->getAttributeValue("texture"));
					newship->SetDimensions(xmlReader->getAttributeValueAsFloat("width")
						,xmlReader->getAttributeValueAsFloat("height"));
					newship->SetColor(xmlReader->getAttributeValueAsFloat("alpha")
						,xmlReader->getAttributeValueAsFloat("red")
						,xmlReader->getAttributeValueAsFloat("green")
						,xmlReader->getAttributeValueAsFloat("blue"));
					shipPrototypes.push_back(newship);
					if(xmlReader->getAttributeValue("name"))
						shipNames.push_back(xmlReader->getAttributeValue("name"));
					else
						shipNames.push_back("");
				}
			}
		}
		delete xmlReader;
	}
	void LoadEnemyPrototypes(const char *_filename)
	{
		// This method loads information necessary for creation of enemies
		irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename);
		while(xmlReader->read())
		{
			if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
			{
				if(!strcmp("Enemy", xmlReader->getNodeName()))
				{
					Enemy *e = new Enemy();
					e->SetShip(NULL);
					e->SetScore(xmlReader->getAttributeValueAsInt("score"));
					e->SetShipID(xmlReader->getAttributeValueAsInt("ship"));
					e->SetWeaponID(xmlReader->getAttributeValueAsInt("weapon"));
					e->SetWeaponLevel(xmlReader->getAttributeValueAsInt("level"));
					e->SetShieldID(xmlReader->getAttributeValueAsInt("shield"));
					e->SetPropulsionID(xmlReader->getAttributeValueAsInt("propulsion"));
					e->SetGeneratorID(xmlReader->getAttributeValueAsInt("generator"));
					e->SetFireFrequency(xmlReader->getAttributeValueAsFloat("fire"));
					e->SetMinDistance(xmlReader->getAttributeValueAsFloat("min"));
					e->SetMaxDistance(xmlReader->getAttributeValueAsFloat("max"));
					std::string behavior;
					if(xmlReader->getAttributeValue("behavior"))
						behavior = xmlReader->getAttributeValue("behavior");
					if(behavior == "flee")
						e->SetMovementBehavior(Enemy::FLEE);
					else if(behavior == "follow")
						e->SetMovementBehavior(Enemy::FOLLOW);
					else if(behavior == "joust")
					{
						e->SetMovementBehavior(Enemy::FOLLOW);
						e->SetJoust(true);
					}
					std::string powerup = xmlReader->getAttributeValue("powerUp");
					if(powerup == "score")
						e->SetPowerUpID(Enemy::SCORE);
					else if(powerup == "energy")
						e->SetPowerUpID(Enemy::ENERGY);
					else if(powerup == "weapon")
						e->SetPowerUpID(Enemy::WEAPON);
					else if(powerup == "armor")
						e->SetPowerUpID(Enemy::ARMOR);
					e->SetPowerUpAmount(xmlReader->getAttributeValueAsInt("powerUpAmount"));
					enemyPrototypes.push_back(e);
				}
			}
		}
		delete xmlReader;
	}
	void LoadVerticalMenuList(const char *_filename, VerticalMenuList &_menu)
	{
		irr::io::IrrXMLReader *xmlReader = irr::io::createIrrXMLReader(_filename);
		unsigned int numberShownItems = 0;

		while(xmlReader->read())
		{
			if(xmlReader->getNodeType() == irr::io::EXN_ELEMENT)
			{
				if(!strcmp("VerticalMenu", xmlReader->getNodeName()))
				{
					_menu.SetPosition(xmlReader->getAttributeValueAsFloat("xpos")
						, xmlReader->getAttributeValueAsFloat("ypos"));
					_menu.SetName(xmlReader->getAttributeValue("name"));
					numberShownItems = xmlReader->getAttributeValueAsInt("numberShownItems");
				}
				else if(!strcmp("MenuItem", xmlReader->getNodeName()))
				{
					MenuItem *menuItem = CreateMenuItem(xmlReader->getAttributeValue("background") ?
						xmlReader->getAttributeValue("background") : "");
					menuItem->SetText(xmlReader->getAttributeValue("text"));
					menuItem->SetActionType(xmlReader->getAttributeValueAsInt("actionType"));
					menuItem->SetActionValue(xmlReader->getAttributeValue("actionValue"));
					menuItem->SetTextNormalColor(xmlReader->getAttributeValueAsFloat("normalTextAlpha")
						, xmlReader->getAttributeValueAsFloat("normalTextRed")
						, xmlReader->getAttributeValueAsFloat("normalTextGreen")
						, xmlReader->getAttributeValueAsFloat("normalTextBlue"));
					menuItem->SetTextHighlightColor(xmlReader->getAttributeValueAsFloat("highlightTextAlpha")
						, xmlReader->getAttributeValueAsFloat("highlightTextRed")
						, xmlReader->getAttributeValueAsFloat("highlightTextGreen")
						, xmlReader->getAttributeValueAsFloat("highlightTextBlue"));
					menuItem->SetDimensions(xmlReader->getAttributeValueAsFloat("width")
						, xmlReader->getAttributeValueAsFloat("height"));
					_menu.AddItem(menuItem);
				}
			}
		}

		_menu.SetNumberShownItems(numberShownItems);

		delete xmlReader;
	}

	// Clean up functions
	void DestroyEnemyPrototypes()
	{
		for(unsigned int i = 0; i < enemyPrototypes.size(); ++i)
			delete enemyPrototypes[i];
		enemyPrototypes.clear();
	}
	void CleanUp()
	{
		for(unsigned int i = 0; i < shipPrototypes.size(); ++i)
			delete shipPrototypes[i];
		shipPrototypes.clear();
		for(unsigned int i = 0; i < enemyPrototypes.size(); ++i)
			delete enemyPrototypes[i];
		enemyPrototypes.clear();
		for(unsigned int i = 0; i < animationPrototypes.size(); ++i)
			delete animationPrototypes[i];
		animationPrototypes.clear();
		weaponFlyweights.clear();
		shieldFlyweights.clear();
		propulsionFlyweights.clear();
		generatorFlyweights.clear();
		shipNames.clear();
	}

}