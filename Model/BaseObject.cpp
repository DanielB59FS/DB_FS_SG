#include "pch.h"
#include "../Model/BaseObject.h"

BaseObject::BaseObject()
{
	SetActive(true);
	SetHeading(0);
}

BaseObject::~BaseObject()
{
}

// accessors
bool BaseObject::GetActive() const {return active;}
float BaseObject::GetHeading() const {return heading;}
float BaseObject::GetXPosition() const {return position.x;}
float BaseObject::GetYPosition() const {return position.y;}
const Vec2f &BaseObject::GetPosition() const {return position;}
float BaseObject::GetWidth() const {return dimensions.x;}
float BaseObject::GetHeight() const {return dimensions.y;}
const Vec2f &BaseObject::GetDimensions() const {return dimensions;}
float BaseObject::GetAlpha() const {return color.a;}
float BaseObject::GetRed() const {return color.r;}
float BaseObject::GetGreen() const {return color.g;}
float BaseObject::GetBlue() const {return color.b;}
const Color4f &BaseObject::GetColor() const {return color;}
const std::string &BaseObject::GetTexture() const {return texture;}

// mutators
void BaseObject::SetActive(bool _active) {active = _active;}

void BaseObject::SetHeading(float _heading)
{
	if(_heading > 4 * PI)
		_heading = 0;
	if(_heading < -4 * PI)
		_heading = 0;
	while(_heading < 0)
		_heading += 2 * PI;
	while(_heading >=  2 * PI)
		_heading -= 2 * PI;
	heading = _heading;
}
void BaseObject::SetXPosition(float _x) {position.x = _x;}
void BaseObject::SetYPosition(float _y) {position.y = _y;}
void BaseObject::SetPosition(float _x, float _y) {position.x = _x; position.y = _y;}
void BaseObject::SetPosition(const Vec2f &_position) {position = _position;}
void BaseObject::SetWidth(float _width) {dimensions.x = _width;}
void BaseObject::SetHeight(float _height) {dimensions.y = _height;}
void BaseObject::SetDimensions(float _width, float _height)
{
	dimensions.x = _width;
	dimensions.y = _height;
}
void BaseObject::SetDimensions(const Vec2f &_dimensions) {dimensions = _dimensions;}
void BaseObject::SetAlpha(float _alpha) {color.a = _alpha;}
void BaseObject::SetRed(float _red) {color.r = _red;}
void BaseObject::SetGreen(float _green) {color.g = _green;}
void BaseObject::SetBlue(float _blue) {color.b = _blue;}
void BaseObject::SetColor(float _alpha, float _red, float _green, float _blue)
{
	color.a = _alpha;
	color.r = _red;
	color.g = _green;
	color.b = _blue;
}
void BaseObject::SetColor(const Color4f &_color) {color = _color;}
void BaseObject::SetTexture(const std::string &_filename) {texture = _filename;}

// interface methods
bool BaseObject::Collide(const BaseObject &_in) const
{
	Vec2f vectorToWaypoint = _in.GetPosition() - GetPosition();
	float myradius = GetWidth() *.5f;
	float itsradius = _in.GetWidth() * .5f;
	float radii = myradius + itsradius;

	// circle to circle collision
	if(vectorToWaypoint * vectorToWaypoint < radii * radii)
		return true;
	else
		return false;
}
void BaseObject::CollisionResponse()
{
	SetActive(false);
}
void BaseObject::TurnToLocation(const Vec2f &_location)
{
	Vec2f forward = _location - GetPosition();
	forward.Normalize();
	SetHeading(SaiphApp::GetHeadingFromUnitVector(forward));
}