#include "pch.h"
#include "EntryState.h"
#include "View/ViewManager.h"
#include "Model/BaseObject.h"

unsigned int EntryState::Heartbeat(float _delta)
{
	unsigned int transition = NOCHANGE;

	// update reticle (mouse cursor)
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(FindWindow(WINDOW_TITLE, WINDOW_TITLE), &mouse);
	cursor->SetXPosition(float(mouse.x));
	cursor->SetYPosition(float(mouse.y));

	if( (GetAsyncKeyState(VK_RETURN) & 1) || (GetAsyncKeyState(VK_LBUTTON) & 1) )
		transition = MENU;
	return transition;
}

void EntryState::Init()
{
	SaiphApp::LoadConfigData("Resources/config.xml");
	SaiphApp::LoadAnimations("Resources/anims.xml");
	SaiphApp::LoadFlyweights("Resources/flyweights.xml");
	SaiphApp::LoadShips("Resources/shipPrototypes.xml");

	background = new BaseObject();
	ViewManager::GetInstance().AddObject(background, 0);
	background->SetTexture("Resources/images/intro.png");
	background->SetDimensions(1024,1024);
	background->SetPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
	
	title = new BaseObject();
	ViewManager::GetInstance().AddObject(title, 0);
	title->SetTexture("Resources/images/saiph.png");
	title->SetDimensions(512, 64);
	title->SetPosition(WINDOW_WIDTH / 2.0f, 100);

	cursor = new BaseObject();
	ViewManager::GetInstance().AddObject(cursor, 5);
	cursor->SetTexture("Resources/images/reticle.png");
	cursor->SetDimensions(32,32);
	cursor->SetColor(0.74f,1,1,1);
}

void EntryState::RenderFrame() const
{
	ViewManager::GetInstance().BeginScene();
	ViewManager::GetInstance().RenderObjects();
	ViewManager::GetInstance().RenderText(100.0f, 500.0f, "Please press Enter or Left Mouse Button to continue...");
	ViewManager::GetInstance().EndScene();
}

void EntryState::Shutdown()
{
	ViewManager::GetInstance().RemoveObject(background);
	delete background;
	background = NULL;

	ViewManager::GetInstance().RemoveObject(title);
	delete title;
	title = NULL;

	ViewManager::GetInstance().RemoveObject(cursor);
	delete cursor;
	cursor = NULL;
}