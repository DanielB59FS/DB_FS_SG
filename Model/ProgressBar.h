// ProgressBar.h
// Progress bar used to display information to the player
// Code by Derek Bliss
// Created October 7, 2008
// Last updated October 7, 2008
//
#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "BaseObject.h"

#define HUD_BAR_MIN_SIZE			2.0f

class ProgressBar
{
	bool		horizontal;
	float		percent;
	Vec2f		barMaxDimensions;
	Vec2f		labelPosition;
	BaseObject	bar;
	BaseObject	background;
	std::string labelText;
public:

	ProgressBar();
	ProgressBar(const ProgressBar &_progressBar);
	virtual ~ProgressBar();

	// mutators
	void SetHorizontal();
	void SetVertical();
	void SetPercent(float _percent);
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
	void SetTexture(const std::string &_texture);
	void SetBarMaxWidth(float _width);
	void SetBarMaxHeight(float _height);
	void SetBarMaxDimensions(float _width, float _height);
	void SetBarMaxDimensions(const Vec2f &_dimensions);
	void SetBarAlpha(float _alpha);
	void SetBarRed(float _red);
	void SetBarGreen(float _green);
	void SetBarBlue(float _blue);
	void SetBarColor(float _alpha, float _red, float _green, float _blue);
	void SetBarColor(const Color4f &_color);
	void SetBarTexture(const std::string &_texture);
	void SetLabelXPosition(float _x);
	void SetLabelYPosition(float _y);
	void SetLabelPosition(float _x, float _y);
	void SetLabelPosition(const Vec2f &_position);
	void SetLabelText(const std::string &_labelText);

	// accessors
	bool GetHorizontal() const;
	bool GetVertical() const;
	float GetPercent() const;
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
	float GetBarMaxWidth() const;
	float GetBarMaxHeight() const;
	const Vec2f &GetBarMaxDimensions() const;
	float GetBarAlpha() const;
	float GetBarRed() const;
	float GetBarGreen() const;
	float GetBarBlue() const;
	const Color4f &GetBarColor() const;
	const std::string &GetBarTexture() const;
	float GetLabelXPosition() const;
	float GetLabelYPosition() const;
	const Vec2f &GetLabelPosition() const;
	const std::string &GetLabelText() const;

	// interface methods
	// Sets foreground width/height of bar to greater of 
	// [percent * max width/height] or [HUD_BAR_MIN_SIZE]
	void Update();
	// Removes this bar from render lists
	void Hide();
	// Adds this bar to render lists
	void Show();
};

#endif