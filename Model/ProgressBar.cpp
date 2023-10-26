#include "pch.h"
#include "ProgressBar.h"
#include "../View/ViewManager.h"

#define HUD_BAR_WIDTH				100.0f
#define HUD_BAR_HEIGHT				16.0f

ProgressBar::ProgressBar()
{
	SetHorizontal();
	SetDimensions(128.0f, 32.0f);
	SetBarMaxDimensions(HUD_BAR_WIDTH, HUD_BAR_HEIGHT);
	SetPercent(1);
	SetTexture("Resources/images/progressBar1.png");
	SetBarTexture("Resources/images/whitebox.png");
	Update();
	Show();
}
ProgressBar::ProgressBar(const ProgressBar &_progressBar)
{
	*this = _progressBar;
	Update();
	Show();
}

ProgressBar::~ProgressBar()
{
	Hide();
}

// mutators
void ProgressBar::SetHorizontal() {horizontal = true;}
void ProgressBar::SetVertical() {horizontal = false;}
void ProgressBar::SetPercent(float _percent)
{
	if(_percent < 0)
		_percent = 0;
	else if(_percent > 1.0f)
		_percent = 1.0f;
	percent = _percent;
}
void ProgressBar::SetXPosition(float _x) {background.SetXPosition(_x);}
void ProgressBar::SetYPosition(float _y) {background.SetYPosition(_y);}
void ProgressBar::SetPosition(float _x, float _y) {background.SetPosition(_x, _y);}
void ProgressBar::SetPosition(const Vec2f &_position) {background.SetPosition(_position);}
void ProgressBar::SetWidth(float _width)
{
	background.SetWidth(_width);
	if(_width < GetBarMaxWidth())
		SetBarMaxWidth(_width);
}
void ProgressBar::SetHeight(float _height)
{
	background.SetHeight(_height);
	if(_height < GetBarMaxHeight())
		SetBarMaxHeight(_height);
}
void ProgressBar::SetDimensions(float _width, float _height)
{
	SetWidth(_width);
	SetHeight(_height);
}
void ProgressBar::SetDimensions(const Vec2f &_dimensions)
{
	SetWidth(_dimensions.x);
	SetHeight(_dimensions.y);
}
void ProgressBar::SetAlpha(float _alpha) {background.SetAlpha(_alpha);}
void ProgressBar::SetRed(float _red) {background.SetRed(_red);}
void ProgressBar::SetGreen(float _green) {background.SetGreen(_green);}
void ProgressBar::SetBlue(float _blue) {background.SetBlue(_blue);}
void ProgressBar::SetColor(float _alpha, float _red, float _green, float _blue) {background.SetColor(_alpha, _red, _green, _blue);}
void ProgressBar::SetColor(const Color4f &_color) {background.SetColor(_color);}
void ProgressBar::SetTexture(const std::string &_texture) {background.SetTexture(_texture);}
void ProgressBar::SetBarMaxWidth(float _width)
{
	if(_width > 0 && _width <= GetWidth())
		barMaxDimensions.x = _width;
}
void ProgressBar::SetBarMaxHeight(float _height)
{
	if(_height > 0 && _height <= GetHeight())
		barMaxDimensions.y = _height;
}
void ProgressBar::SetBarMaxDimensions(float _width, float _height)
{
	SetBarMaxWidth(_width);
	SetBarMaxHeight(_height);
}
void ProgressBar::SetBarMaxDimensions(const Vec2f &_dimensions)
{
	SetBarMaxWidth(_dimensions.x);
	SetBarMaxHeight(_dimensions.y);
}
void ProgressBar::SetBarAlpha(float _alpha) {bar.SetAlpha(_alpha);}
void ProgressBar::SetBarRed(float _red) {bar.SetRed(_red);}
void ProgressBar::SetBarGreen(float _green) {bar.SetGreen(_green);}
void ProgressBar::SetBarBlue(float _blue) {bar.SetBlue(_blue);}
void ProgressBar::SetBarColor(float _alpha, float _red, float _green, float _blue) {bar.SetColor(_alpha, _red, _green, _blue);}
void ProgressBar::SetBarColor(const Color4f &_color) {bar.SetColor(_color);}
void ProgressBar::SetBarTexture(const std::string &_texture) {bar.SetTexture(_texture);}
void ProgressBar::SetLabelXPosition(float _x) {labelPosition.x = _x;}
void ProgressBar::SetLabelYPosition(float _y) {labelPosition.y = _y;}
void ProgressBar::SetLabelPosition(float _x, float _y) {SetLabelXPosition(_x); SetLabelYPosition(_y);}
void ProgressBar::SetLabelPosition(const Vec2f &_position) {labelPosition = _position;}
void ProgressBar::SetLabelText(const std::string &_labelText) {labelText = _labelText;}

// accessors
bool ProgressBar::GetHorizontal() const {return horizontal;}
bool ProgressBar::GetVertical() const {return !horizontal;}
float ProgressBar::GetPercent() const {return percent;}
float ProgressBar::GetXPosition() const {return background.GetXPosition();}
float ProgressBar::GetYPosition() const {return background.GetYPosition();}
const Vec2f &ProgressBar::GetPosition() const {return background.GetPosition();}
float ProgressBar::GetWidth() const {return background.GetWidth();}
float ProgressBar::GetHeight() const {return background.GetHeight();}
const Vec2f &ProgressBar::GetDimensions() const {return background.GetDimensions();}
float ProgressBar::GetAlpha() const {return background.GetAlpha();}
float ProgressBar::GetRed() const {return background.GetRed();}
float ProgressBar::GetGreen() const {return background.GetGreen();}
float ProgressBar::GetBlue() const {return background.GetBlue();}
const Color4f &ProgressBar::GetColor() const {return background.GetColor();}
const std::string &ProgressBar::GetTexture() const {return background.GetTexture();}
float ProgressBar::GetBarMaxWidth() const {return barMaxDimensions.x;}
float ProgressBar::GetBarMaxHeight() const {return barMaxDimensions.y;}
const Vec2f &ProgressBar::GetBarMaxDimensions() const {return barMaxDimensions;}
float ProgressBar::GetBarAlpha() const {return bar.GetAlpha();}
float ProgressBar::GetBarRed() const {return bar.GetRed();}
float ProgressBar::GetBarGreen() const {return bar.GetGreen();}
float ProgressBar::GetBarBlue() const {return bar.GetBlue();}
const Color4f &ProgressBar::GetBarColor() const {return bar.GetColor();}
const std::string &ProgressBar::GetBarTexture() const {return bar.GetTexture();}
float ProgressBar::GetLabelXPosition() const {return labelPosition.x;}
float ProgressBar::GetLabelYPosition() const {return labelPosition.y;}
const Vec2f &ProgressBar::GetLabelPosition() const {return labelPosition;}
const std::string &ProgressBar::GetLabelText() const {return labelText;}

// interface methods
void ProgressBar::Update()
{
	float size = percent;
	if(horizontal)
	{
		size *= GetBarMaxWidth();
		if(size < HUD_BAR_MIN_SIZE)
			size = HUD_BAR_MIN_SIZE;
		bar.SetHeight(GetBarMaxHeight());
		bar.SetWidth(size);
		bar.SetPosition(GetXPosition() - GetBarMaxWidth()/2 + bar.GetWidth()/2,
			GetYPosition());
	}
	else
	{
		size *= GetBarMaxHeight();
		if(size < HUD_BAR_MIN_SIZE)
			size = HUD_BAR_MIN_SIZE;
		bar.SetWidth(GetBarMaxWidth());
		bar.SetHeight(size);
		bar.SetPosition(GetXPosition(), GetYPosition() + GetBarMaxHeight()/2 -
			bar.GetHeight()/2);
	}
}
void ProgressBar::Hide()
{
	ViewManager::GetInstance().RemoveObject(&background);
	ViewManager::GetInstance().RemoveObject(&bar);
}
void ProgressBar::Show()
{
	ViewManager::GetInstance().AddObject(&background, 5);
	ViewManager::GetInstance().AddObject(&bar, 5);
}