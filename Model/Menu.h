#ifndef MENU_H
#define MENU_H

#include "BaseObject.h"

class MenuItem
{
	bool highlight;
	unsigned int actionType;
	Color4f textNormal;
	Color4f textHighlight;
	BaseObject background;
	std::string text;
	std::string actionValue;

public:
	enum ACTIONTYPES
	{
		NONE, SUBMENU, STATECHANGE, NUMACTIONS
	};

	MenuItem();
	~MenuItem();

	// mutators
	void SetHighlight(bool _highlight);
	void SetActionType(unsigned int _actionType);
	void SetActionValue(const std::string &_actionValue);
	void SetXPosition(float _x);
	void SetYPosition(float _y);
	void SetPosition(float _x, float _y);
	void SetPosition(const Vec2f &_position);
	void SetWidth(float _width);
	void SetHeight(float _height);
	void SetDimensions(float _width, float _height);
	void SetDimensions(const Vec2f &_dimensions);
	void SetTextNormalAlpha(float _alpha);
	void SetTextNormalRed(float _red);
	void SetTextNormalGreen(float _green);
	void SetTextNormalBlue(float _blue);
	void SetTextNormalColor(float _alpha, float _red, float _green, float _blue);
	void SetTextNormalColor(const Color4f &_color);
	void SetTextHighlightAlpha(float _alpha);
	void SetTextHighlightRed(float _red);
	void SetTextHighlightGreen(float _green);
	void SetTextHighlightBlue(float _blue);
	void SetTextHighlightColor(float _alpha, float _red, float _green, float _blue);
	void SetTextHighlightColor(const Color4f &_color);
	void SetText(const std::string &_text);
	void SetTexture(const std::string &_texture);

	// accessors
	bool GetHighlight() const;
	unsigned int GetActionType() const;
	const std::string &GetActionValue() const;
	float GetXPosition() const;
	float GetYPosition() const;
	const Vec2f &GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	const Vec2f &GetDimensions() const;
	float GetTextAlpha() const;
	float GetTextRed() const;
	float GetTextGreen() const;
	float GetTextBlue() const;
	const Color4f &GetTextColor() const;
	const std::string &GetText() const;
	const std::string &GetTexture() const;

	// interface methods
	void Hide();
	void Show();
	void Render() const;
};

// Abstract class for basic menu interface standard
class Menu
{
protected:

	std::string name;
	std::vector<MenuItem *> items;

public:
	virtual ~Menu();

	// mutators
	void SetName(const std::string &_name);

	// accessors
	const std::string &GetName() const;
	virtual unsigned int GetSelectedIndex() const = 0;
	virtual unsigned int GetNumberItems() const = 0;

	// interface methods
	virtual void Hide();
	virtual void Show();
	virtual void AddItem(MenuItem *_item) = 0;
	virtual void ClearItems() = 0;
	virtual void Update() = 0;
	virtual const MenuItem *GetSelectedItem() const = 0;
};

// This menu will display items virtically with no
// overlap or spacing between items
class VerticalMenuList : public Menu
{
	const BaseObject *cursor;
	Vec2f position;
	unsigned int selectedIndex;
	unsigned int numShownItems;
	unsigned int startShowIndex;

public:

	VerticalMenuList();
	virtual ~VerticalMenuList();

	// mutators
	void SetXPosition(float _x);
	void SetYPosition(float _y);
	void SetPosition(float _x, float _y);
	void SetPosition(const Vec2f &_position);
	void SetNumberShownItems(unsigned int _num);
	void SetStartShowIndex(unsigned int _index);
	
	// accessors
	unsigned int GetSelectedIndex() const;			// this will return -1 if nothing is highlighted
	unsigned int GetNumberItems() const;
	float GetXPosition() const;
	float GetYPosition() const;
	const Vec2f &GetPosition() const;
	unsigned int GetNumberShownItems() const;
	unsigned int GetStartShowIndex() const;

	// interface methods
	void LinkCursor(const BaseObject *_cursor);
	void RenderItemText() const;
	void AddItem(MenuItem *_item);
	void ClearItems();
	void Update();
	// Sets the items at the given index (if valid) to be highlighted.
	// This method will change what items are viewable if necessary.
	void SelectItem(unsigned int _index);
	const MenuItem *GetSelectedItem() const;		// this will return NULL if nothing is highlighted
	bool SelectWithCursor();						// this will select the item the mouse is over if any

};

#endif