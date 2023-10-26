// BaseObject.h
// Base object class for managing common data and functionality across all objects
// Code by Nick Penney and Derek Bliss
// Created April 10, 2007
// Last updated October 7, 2008
//
#ifndef BASEOBJECT_H
#define BASEOBJECT_H

class BaseObject
{
	bool active;			// Set to false to remove object
	float heading;			// Radial heading in radians (0 is the positive x-axis direction)
	Vec2f position;			// Position of object in world coordinates
	Vec2f dimensions;		// Height and Width of object
	Color4f color;			// Color4f of object
	std::string texture;	// Filename of texture to use for rendering

public:
	BaseObject();
	virtual ~BaseObject();

	// mutators
	void SetActive(bool _active);
	void SetHeading(float _heading);
	void SetXPosition(float _x);
	void SetYPosition(float _y);
	void SetPosition(float _x, float _y);
	void SetPosition(const Vec2f &_position);
	void SetWidth(float _width);
	void SetHeight(float _height);
	void SetDimensions(float _width, float _height);
	void SetDimensions(const Vec2f &_dimensions);
	void SetAlpha(float _alpha);
	void SetRed(float _red);
	void SetGreen(float _green);
	void SetBlue(float _blue);
	void SetColor(float _alpha, float _red, float _green, float _blue);
	void SetColor(const Color4f &_color);
	void SetTexture(const std::string &_filename);

	// accessors
	bool GetActive() const;
	float GetHeading() const;
	float GetXPosition() const;
	float GetYPosition() const;
	const Vec2f &GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	const Vec2f &GetDimensions() const;
	float GetAlpha() const;
	float GetRed() const;
	float GetGreen() const;
	float GetBlue() const;
	const Color4f &GetColor() const;
	const std::string &GetTexture() const;

	// interface methods
	virtual void CollisionResponse();
	bool Collide(const BaseObject &_in) const;
	virtual void TurnToLocation(const Vec2f &_location);	// Set heading to face given location
};

#endif