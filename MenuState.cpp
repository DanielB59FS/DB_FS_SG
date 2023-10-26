#include "pch.h"
#include "MenuState.h"
#include "Model/Menu.h"
#include "Model/BaseObject.h"
#include "View/ViewManager.h"

namespace
{
	VerticalMenuList menu, options;
	VerticalMenuList *current;
	BaseObject *cursor;
	std::string submenu = "";
	char buffer[64];

	BaseObject *CreateMouseCursor()
	{
		BaseObject *cur = new BaseObject();
		ViewManager::GetInstance().AddObject(cur, 5);
		menu.LinkCursor(cur);
		options.LinkCursor(cur);
		cur->SetTexture("Resources/images/reticle.png");
		cur->SetDimensions(32,32);
		cur->SetColor(0.5f,1,1,1);
		return cur;
	}
	MenuItem *CreateMenuItem()
	{
		MenuItem *menuItem = new MenuItem();
		menuItem->SetDimensions(400,50);
		return menuItem;
	}
}

unsigned int MenuState::PerformSelection()
{
	unsigned int ret = NOCHANGE;
	const MenuItem *selected;
	selected = current->GetSelectedItem();
	if(selected)
	{
		switch(selected->GetActionType())
		{
		case MenuItem::NONE:
			if(selected->GetActionValue() == "render")
			{
				if(SaiphApp::GetRenderer())
				{
					const_cast<MenuItem *>(selected)->SetText("Renderer: OpenGL");
					SaiphApp::UseOpenGL();
				}
				else
				{
					const_cast<MenuItem *>(selected)->SetText("Renderer: DirectX");
					SaiphApp::UseDirectX();
				}
			}
			else if(selected->GetActionValue() == "window")
			{
				SaiphApp::SetFullscreen(!SaiphApp::GetFullscreen());
				if(SaiphApp::GetFullscreen())
					const_cast<MenuItem *>(selected)->SetText("FullScreen: Yes");
				else
					const_cast<MenuItem *>(selected)->SetText("FullScreen: No");
			}
			else if(selected->GetActionValue() == "control")
			{
				SaiphApp::SetControl(!SaiphApp::GetControl());
				if(SaiphApp::GetControl())
					const_cast<MenuItem *>(selected)->SetText("Using Cartesian Control Style");
				else
					const_cast<MenuItem *>(selected)->SetText("Using Polar Control Style");
			}
			break;
		case MenuItem::SUBMENU:
			submenu = selected->GetActionValue();
			if(submenu == "options")
			{
				current = &options;
				menu.Hide();
				options.SelectItem(0);
			}
			else
			{
				current = &menu;
				options.Hide();
				menu.SelectItem(0);
			}
			break;
		case MenuItem::STATECHANGE:
			if(selected->GetActionValue() == "shop")
				ret = SHOP;
			else if(selected->GetActionValue() == "level")
				ret = LEVEL;
			else if(selected->GetActionValue() == "exit")
				ret = EXIT;
			break;
		default:
			break;
		}
	}
	return ret;
}


void MenuState::Init()
{
	cursor = CreateMouseCursor();
	SaiphApp::LoadVerticalMenuList("Resources/entry.xml", menu);
	menu.SelectItem(0);

	// build the options submenu by hand
	MenuItem *item = CreateMenuItem();
	if(SaiphApp::GetRenderer())
		item->SetText("Renderer: DirectX");
	else
		item->SetText("Renderer: OpenGL");
	item->SetActionValue("render");
	item->SetTextNormalColor(1, 1, 1, 0);
	item->Show();
	options.AddItem(item);
	item = CreateMenuItem();
	if(SaiphApp::GetFullscreen())
		item->SetText("FullScreen: Yes");
	else
		item->SetText("FullScreen: No");
	item->SetActionValue("window");
	item->SetTextNormalColor(1, 1, 1, 0);
	item->Show();
	options.AddItem(item);
	item = CreateMenuItem();
	if(SaiphApp::GetControl())
		item->SetText("Using Cartesian Control Style");
	else
		item->SetText("Using Polar Control Style");
	item->SetActionValue("control");
	item->SetTextNormalColor(1, 1, 1, 0);
	options.AddItem(item);
	item = CreateMenuItem();
	item->SetText("Exit");
	item->SetActionType(MenuItem::SUBMENU);
	item->SetActionValue("");
	item->SetTextNormalColor(1, 1, 1, 0);
	options.AddItem(item);
	options.SetPosition(menu.GetPosition());
	options.SetNumberShownItems(menu.GetNumberShownItems());
	options.SelectItem(0);
	options.Hide();
	current = &menu;

	// prevent escape from previous state
	GetAsyncKeyState(VK_ESCAPE);
}

unsigned int MenuState::Heartbeat(float _delta)
{
	unsigned int stateChange = NOCHANGE;

	// Escape key
	if(GetAsyncKeyState(VK_ESCAPE) & 0x1)
	{
		current->SelectItem(menu.GetNumberItems()-1);
		return PerformSelection();
	}

	// keyboard movement
	if(GetAsyncKeyState(VK_UP) & 0x1)
		current->SelectItem(current->GetSelectedIndex() - 1);
	else if(GetAsyncKeyState(VK_DOWN) & 0x1)
		current->SelectItem(current->GetSelectedIndex() + 1);

	// mouse movement
	static POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(FindWindow(WINDOW_TITLE, WINDOW_TITLE), &mouse);
	if(cursor->GetXPosition() != float(mouse.x) || cursor->GetYPosition() != float(mouse.y))
	{
		cursor->SetXPosition(float(mouse.x));
		cursor->SetYPosition(float(mouse.y));
		current->SelectWithCursor();
	}

	// update menus
	current->Update();

	// confirm keys/buttons
	if(((GetAsyncKeyState(VK_LBUTTON) & 0x1) && current->SelectWithCursor()) || (GetAsyncKeyState(VK_RETURN) & 0x1))
		stateChange = PerformSelection();

	return stateChange;
}

void MenuState::RenderFrame() const
{
	ViewManager::GetInstance().BeginScene();
	sprintf(buffer, "Main Menu | Next Level : %d", SaiphApp::GetLevel());
	ViewManager::GetInstance().RenderText(10,20, buffer);
	ViewManager::GetInstance().RenderObjects();
	current->RenderItemText();
	ViewManager::GetInstance().EndScene();
}

void MenuState::Shutdown()
{
	menu.ClearItems();
	menu.LinkCursor(0);
	options.ClearItems();
	options.LinkCursor(0);
	ViewManager::GetInstance().RemoveObject(cursor);
	delete cursor;
	cursor = NULL;
}