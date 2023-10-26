#include "pch.h"
#include "Menu.h"
#include "../View/ViewManager.h"
#include <math.h>

namespace
{
	float absf(float _value)
	{
		if(_value < 0)
			return -_value;
		return _value;
	}
}

MenuItem::MenuItem()
{
	SetTexture("Resources/images/menubox.png");
	SetTextNormalColor(1, 1, 1, 1);
	SetTextHighlightColor(1, 0.1f, 0.1f, 0.1f);
	SetHighlight(false);
	SetActionType(NONE);
}

MenuItem::~MenuItem()
{
	Hide();
}

// mutators
void MenuItem::SetHighlight(bool _highlight) 
{
	highlight = _highlight;
	if(highlight)
		background.SetColor(textNormal);
	else
		background.SetColor(textHighlight);
}
void MenuItem::SetActionType(unsigned int _actionType)
{
	if(_actionType >= NUMACTIONS)
		_actionType = 0;
	actionType = _actionType;
}
void MenuItem::SetActionValue(const std::string &_actionValue) {actionValue = _actionValue;}
void MenuItem::SetXPosition(float _x) {background.SetXPosition(_x);}
void MenuItem::SetYPosition(float _y) {background.SetYPosition(_y);}
void MenuItem::SetPosition(float _x, float _y) {background.SetPosition(_x, _y);}
void MenuItem::SetPosition(const Vec2f &_position) {background.SetPosition(_position);}
void MenuItem::SetWidth(float _width) {background.SetWidth(_width);}
void MenuItem::SetHeight(float _height) {background.SetHeight(_height);}
void MenuItem::SetDimensions(float _width, float _height) {background.SetDimensions(_width, _height);}
void MenuItem::SetDimensions(const Vec2f &_dimensions) {background.SetDimensions(_dimensions);}
void MenuItem::SetTextNormalAlpha(float _alpha) {textNormal.a = _alpha;}
void MenuItem::SetTextNormalRed(float _red) {textNormal.r = _red;}
void MenuItem::SetTextNormalGreen(float _green) {textNormal.g = _green;}
void MenuItem::SetTextNormalBlue(float _blue) {textNormal.b = _blue;}
void MenuItem::SetTextNormalColor(float _alpha, float _red, float _green, float _blue)
{
	textNormal.a = _alpha;
	textNormal.r = _red;
	textNormal.g = _green;
	textNormal.b = _blue;
}
void MenuItem::SetTextNormalColor(const Color4f &_color) {textNormal = _color;}
void MenuItem::SetTextHighlightAlpha(float _alpha) {textHighlight.a = _alpha;}
void MenuItem::SetTextHighlightRed(float _red) {textHighlight.r = _red;}
void MenuItem::SetTextHighlightGreen(float _green) {textHighlight.g = _green;}
void MenuItem::SetTextHighlightBlue(float _blue) {textHighlight.b = _blue;}
void MenuItem::SetTextHighlightColor(float _alpha, float _red, float _green, float _blue)
{
	textHighlight.a = _alpha;
	textHighlight.r = _red;
	textHighlight.g = _green;
	textHighlight.b = _blue;
}
void MenuItem::SetTextHighlightColor(const Color4f &_color) {textHighlight = _color;}
void MenuItem::SetText(const std::string &_text) {text = _text;}
void MenuItem::SetTexture(const std::string &_texture) {background.SetTexture(_texture);}

// accessors
bool MenuItem::GetHighlight() const {return highlight;}
unsigned int MenuItem::GetActionType() const {return actionType;}
const std::string &MenuItem::GetActionValue() const {return actionValue;}
float MenuItem::GetXPosition() const {return background.GetXPosition();}
float MenuItem::GetYPosition() const {return background.GetYPosition();}
const Vec2f &MenuItem::GetPosition() const {return background.GetPosition();}
float MenuItem::GetWidth() const {return background.GetWidth();}
float MenuItem::GetHeight() const {return background.GetHeight();}
const Vec2f &MenuItem::GetDimensions() const {return background.GetDimensions();}
float MenuItem::GetTextAlpha() const
{
	float a = 0;
	if(!highlight)
		a = textNormal.a;
	else
		a = textHighlight.a;
	return a;
}
float MenuItem::GetTextRed() const
{
	float r = 0;
	if(!highlight)
		r = textNormal.r;
	else
		r = textHighlight.r;
	return r;
}
float MenuItem::GetTextGreen() const
{
	float g = 0;
	if(!highlight)
		g = textNormal.g;
	else
		g = textHighlight.g;
	return g;
}
float MenuItem::GetTextBlue() const
{
	float b = 0;
	if(!highlight)
		b = textNormal.b;
	else
		b = textHighlight.b;
	return b;
}
const Color4f &MenuItem::GetTextColor() const
{
	if(!highlight)
		return textNormal;
	return textHighlight;
}
const std::string &MenuItem::GetText() const {return text;}
const std::string &MenuItem::GetTexture() const {return background.GetTexture();}

// interface methods
void MenuItem::Hide()
{
	ViewManager::GetInstance().RemoveObject(&background);
}
void MenuItem::Show()
{
	ViewManager::GetInstance().AddObject(&background, 4);
}
void MenuItem::Render() const
{
	if(ViewManager::GetInstance().HasObject(&background))
	{
		if (SaiphApp::GetRenderer())
		{
			ViewManager::GetInstance().RenderText(GetXPosition(), GetYPosition(), GetText(), GetTextColor(), true);
		}
		else
		{
			RECT client;
			GetClientRect(FindWindow(WINDOW_TITLE, WINDOW_TITLE), &client);
			ViewManager::GetInstance().RenderText(
				GetXPosition() - GetWidth() / 2 + static_cast<int>((client.bottom - client.top) * 0.02f)
				, GetYPosition() + static_cast<int>((client.bottom - client.top) * 0.02f) / 4.0f
				, GetText()
				, GetTextColor());
		}
	}
}

//////////////////////////////////////////////////////

Menu::~Menu(){}
void Menu::SetName(const std::string &_name) {name = _name;}
const std::string &Menu::GetName() const {return name;}
void Menu::Hide()
{
	for(unsigned int i = 0; i < items.size(); ++i)
		items[i]->Hide();
}
void Menu::Show()
{
	for(unsigned int i = 0; i < items.size(); ++i)
		items[i]->Show();
}

//////////////////////////////////////////////////////

VerticalMenuList::VerticalMenuList()
{
	cursor = 0;
	numShownItems = startShowIndex = 0;
	selectedIndex = -1;
}

VerticalMenuList::~VerticalMenuList()
{
	ClearItems();
}

// mutators
void VerticalMenuList::SetXPosition(float _x) {position.x = _x;}
void VerticalMenuList::SetYPosition(float _y) {position.y = _y;}
void VerticalMenuList::SetPosition(float _x, float _y) {position.x = _x; position.y = _y;}
void VerticalMenuList::SetPosition(const Vec2f &_position) {position = _position;}
void VerticalMenuList::SetNumberShownItems(unsigned int _num)
{
	if(_num < items.size())
		numShownItems = _num;
	else
		numShownItems = (unsigned int)items.size();
}
void VerticalMenuList::SetStartShowIndex(unsigned int _index)
{
	if(numShownItems)
	{
		if(_index + numShownItems < items.size())
			startShowIndex = _index;
		else
			startShowIndex = (unsigned int)items.size() - numShownItems;
	}
}

// accessors
unsigned int VerticalMenuList::GetSelectedIndex() const {return selectedIndex;}
unsigned int VerticalMenuList::GetNumberItems() const {return (unsigned int)items.size();}
float VerticalMenuList::GetXPosition() const {return position.x;}
float VerticalMenuList::GetYPosition() const {return position.y;}
const Vec2f &VerticalMenuList::GetPosition() const {return position;}
unsigned int VerticalMenuList::GetNumberShownItems() const {return numShownItems;}
unsigned int VerticalMenuList::GetStartShowIndex() const {return startShowIndex;}

void VerticalMenuList::LinkCursor(const BaseObject *_cursor)
{
	if(_cursor)
		cursor = _cursor;
}

void VerticalMenuList::RenderItemText() const
{
	for(unsigned int i = startShowIndex; i < startShowIndex + numShownItems; ++i)
		items[i]->Render();
}

void VerticalMenuList::AddItem(MenuItem *_item)
{
	if(_item)
		items.push_back(_item);
}

void VerticalMenuList::ClearItems()
{
	for(unsigned int i = 0; i < items.size(); ++i)
		delete items[i];
	items.clear();
	numShownItems = startShowIndex = 0;
	selectedIndex = -1;
}

void VerticalMenuList::Update()
{
	// make everything disappear
	Hide();
	for(unsigned int i = 0; i < items.size(); ++i)
		items[i]->SetHighlight(false);

	// update positions of items based on menu's position
	float y = 0;
	for(unsigned int i = startShowIndex; i < startShowIndex + numShownItems; ++i)
	{
		items[i]->Show();
		items[i]->SetPosition(GetXPosition() + absf(items[i]->GetWidth()) / 2, GetYPosition() + y + absf(items[i]->GetHeight()) / 2);
		y += items[i]->GetHeight();
	}

	if(selectedIndex != -1)
		// set selected highlight
		items[selectedIndex]->SetHighlight(true);
}

void VerticalMenuList::SelectItem(unsigned int _index)
{
	// do nothing if given index is out of bounds
	// also do nothing if menu is set to show no items
	if((_index < items.size()) && numShownItems)
	{
		SetStartShowIndex(_index);

		selectedIndex = _index;
	}
}

const MenuItem *VerticalMenuList::GetSelectedItem() const
{
	const MenuItem *ret = NULL;
	if(selectedIndex != -1)
		ret = items[selectedIndex];
	return ret;
}

bool VerticalMenuList::SelectWithCursor()
{
	bool ret = false;
	unsigned int index = -1;
	// determine if mouse is over any items and highlight as necessary
	if(cursor)
	{
		for(unsigned int i = startShowIndex; i < startShowIndex + numShownItems; ++i)
		{
			if(cursor->GetXPosition() < items[i]->GetXPosition() - absf(items[i]->GetWidth()) / 2
				|| cursor->GetXPosition() > items[i]->GetXPosition() + absf(items[i]->GetWidth()) / 2
				|| cursor->GetYPosition() < items[i]->GetYPosition() - absf(items[i]->GetHeight()) / 2
				|| cursor->GetYPosition() > items[i]->GetYPosition() + absf(items[i]->GetHeight()) / 2)
				continue;
			// if this point is reached the mouse must be over this item
			ret = true;
			index = i;
			break; // this menu only allows one item to be highlighted at once
		}
	}

	if(index < GetNumberItems())
		selectedIndex = index;
	return ret;
}