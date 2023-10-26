#include "..\\SaiphApp.h"
#include "SubShopState.h"
#include "../View/ViewManager.h"

#define MENU_ITEM_WIDTH				(425.0f)
#define MENU_ITEM_HEIGHT			(50.0f)

extern int alteredScore;
void SubShopState::Init()
{
	cursor = NULL;
	currentMenu = &menu;
	alteredScore = SaiphApp::GetScore();

	// menus
	SaiphApp::LoadVerticalMenuList("Resources/shop.xml", menu);
	submenu = "";
	numShownItems = menu.GetNumberShownItems();
	GenerateShopMenus();
	ships.SetPosition(menu.GetPosition());
	weapons.SetPosition(menu.GetPosition());
	shields.SetPosition(menu.GetPosition());
	generators.SetPosition(menu.GetPosition());
	props.SetPosition(menu.GetPosition());
	menu.SelectItem(0);
	ships.SelectItem(0);
	weapons.SelectItem(0);
	shields.SelectItem(0);
	generators.SelectItem(0);
	props.SelectItem(0);
	scroll.SelectItem(0);

	// mouse
	cursor = CreateMouseCursor();

	// hud
	hud = CreateHud();

	// player
	playerShip = 0;
	ResetPlayer();
	shieldTimer = 0;

	// prevent escape from previous state
	GetAsyncKeyState(VK_ESCAPE);
}

void SubShopState::GenerateShopMenus()
{
	char buffer[256];

	memset(buffer, 0, 256);
	MenuItem *item;
	// Ships
	for (unsigned int i = 0; i < SaiphApp::GetNumberOfShipPrototypes(); ++i)
	{
		shiptypes[i] = SaiphApp::CloneShipFromPrototype(i);
		ViewManager::GetInstance().RemoveObject(shiptypes[i]);
		if (shiptypes[i]->GetCost() >= 0)
		{
			sprintf(buffer, "%s | Armor Value: %.f, Cost: %d", SaiphApp::GetShipName(i).c_str()
				, shiptypes[i]->GetArmor(), shiptypes[i]->GetCost());
			item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
			item->SetText(buffer);
			item->SetActionType(MenuItem::NONE);
			sprintf(buffer, "%d", i);
			item->SetActionValue(buffer);
			item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
			item->SetTextNormalColor(1, 0, 1, 0);
			ships.AddItem(item);
		}
	}
	item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
	item->SetText("Exit");
	item->SetActionType(MenuItem::SUBMENU);
	item->SetActionValue("");
	item->SetTextNormalColor(1, 0, 1, 0);
	item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
	ships.AddItem(item);
	ships.SetNumberShownItems(numShownItems);
	// Weapons
	for (unsigned int i = 0; i < SaiphApp::GetNumberOfWeaponFlyweights(); ++i)
	{
		if (SaiphApp::GetWeaponFlyweight(i).levels[0].cost >= 0)
		{
			for (unsigned int j = 0; j < SaiphApp::GetWeaponFlyweight(i).levels.size(); ++j)
			{
				item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));

				const WeaponFlyweight& wf = SaiphApp::GetWeaponFlyweight(i);
				sprintf(buffer, "%s Level:%d Energy Use: %.f Cost:%d", wf.name.c_str(), j + 1,
					wf.levels[j].energy, wf.levels[j].cost);

				item->SetText(buffer);
				item->SetActionType(MenuItem::NONE);
				sprintf(buffer, "%d", i * 100 + j);
				item->SetActionValue(buffer);
				item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
				item->SetTextNormalColor(1, 1, 0, 1);
				weapons.AddItem(item);
			}
		}
	}
	item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
	item->SetText("Exit");
	item->SetActionType(MenuItem::SUBMENU);
	item->SetActionValue("");
	item->SetTextNormalColor(1, 1, 0, 1);
	item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
	weapons.AddItem(item);
	weapons.SetNumberShownItems(numShownItems);
	// Shields
	for (unsigned int i = 0; i < SaiphApp::GetNumberOfShieldFlyweights(); ++i)
	{
		if (SaiphApp::GetShieldFlyweight(i).cost >= 0)
		{
			item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
			sprintf(buffer, "Strength: %.f, Recharge: %.f/s, Energy Use: %.f/s Cost: %d"
				, SaiphApp::GetShieldFlyweight(i).strength
				, SaiphApp::GetShieldFlyweight(i).recharge
				, SaiphApp::GetShieldFlyweight(i).energy
				, SaiphApp::GetShieldFlyweight(i).cost);
			item->SetText(buffer);
			item->SetActionType(MenuItem::NONE);
			sprintf(buffer, "%d", i);
			item->SetActionValue(buffer);
			item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
			item->SetTextNormalColor(1, 0, 1, 1);
			shields.AddItem(item);
		}
	}
	item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
	item->SetText("Exit");
	item->SetActionType(MenuItem::SUBMENU);
	item->SetActionValue("");
	item->SetTextNormalColor(1, 0, 1, 1);
	item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
	shields.AddItem(item);
	shields.SetNumberShownItems(numShownItems);
	// Generators
	for (unsigned int i = 0; i < SaiphApp::GetNumberOfGeneratorFlyweights(); ++i)
	{
		if (SaiphApp::GetGeneratorFlyweight(i).cost >= 0)
		{
			item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
			sprintf(buffer, "Generates: %.f energy/s, Battery: %.f, Cost: %d"
				, SaiphApp::GetGeneratorFlyweight(i).energy
				, SaiphApp::GetGeneratorFlyweight(i).battery
				, SaiphApp::GetGeneratorFlyweight(i).cost);
			item->SetText(buffer);
			item->SetActionType(MenuItem::NONE);
			sprintf(buffer, "%d", i);
			item->SetActionValue(buffer);
			item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
			item->SetTextNormalColor(1, 1, 1, 0);
			generators.AddItem(item);
		}
	}
	item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
	item->SetText("Exit");
	item->SetActionType(MenuItem::SUBMENU);
	item->SetActionValue("");
	item->SetTextNormalColor(1, 1, 1, 0);
	item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
	generators.AddItem(item);
	generators.SetNumberShownItems(numShownItems);
	// Props
	for (unsigned int i = 0; i < SaiphApp::GetNumberOfPropulsionFlyweights(); ++i)
	{
		if (SaiphApp::GetPropulsionFlyweight(i).cost >= 0)
		{
			item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
			sprintf(buffer, "Speed: %.f/s, Afterburner: %.f, Energy Use: %.f/s Cost: %d"
				, SaiphApp::GetPropulsionFlyweight(i).force
				, SaiphApp::GetPropulsionFlyweight(i).afterburnerEnergy
				, SaiphApp::GetPropulsionFlyweight(i).energy
				, SaiphApp::GetPropulsionFlyweight(i).cost);
			item->SetText(buffer);
			item->SetActionType(MenuItem::NONE);
			sprintf(buffer, "%d", i);
			item->SetActionValue(buffer);
			item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
			item->SetTextNormalColor(1, 1, 0, 0);
			props.AddItem(item);
		}
	}
	item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
	item->SetText("Exit");
	item->SetActionType(MenuItem::SUBMENU);
	item->SetActionValue("");
	item->SetTextNormalColor(1, 1, 0, 0);
	item->SetDimensions(MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
	props.AddItem(item);
	props.SetNumberShownItems(numShownItems);
	// Scroll menu
	item = CreateMenuItem(Color4f(), "Resources/images/arrow.png");
	item->SetDimensions(60, 150);
	item->SetText("  Up");
	item->SetTextNormalColor(1, 0, 1, 1);
	item->SetTextHighlightColor(1, 1, 0, 0);
	scroll.AddItem(item);
	item = CreateMenuItem(Color4f(), "Resources/images/arrow.png");
	item->SetDimensions(60, -150);
	item->SetText("Down");
	item->SetTextNormalColor(1, 0, 1, 1);
	item->SetTextHighlightColor(1, 1, 0, 0);
	scroll.AddItem(item);
	scroll.SetPosition(menu.GetPosition().x - 60, menu.GetPosition().y);
	scroll.SetNumberShownItems(2);
}
