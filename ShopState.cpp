#include "pch.h"

#ifdef BUGS
#define _CRT_SECURE_NO_DEPRECATE	// 2k5 complains about 2k3 string stuff

#include "ShopState.h"
#include "Model/HUD.h"
#include "Model/BaseObject.h"
#include "Model/Ship.h"
#include "Model/Bullet.h"
#include "Model/Effect.h"
#include "View/ViewManager.h"

int alteredScore;

// Game Variables
#define MAX_SHIP_ARMOR				200.0f
#define MAX_WEAPON_UPGRADE			5
#define HUD_BAR_WIDTH				100.0f
#define SHIELD_INTERVAL				0.25f
#define MENU_ITEM_WIDTH				(425.0f)
#define MENU_ITEM_HEIGHT			(50.0f)

// create functions
MenuItem *ShopState::CreateMenuItem(const Color4f &_backgroundColor, const std::string &_texture)
{
	MenuItem *menuItem = new MenuItem();
	menuItem->SetTexture(_texture);
	menuItem->SetDimensions(10,10);
	menuItem->SetTextHighlightColor(_backgroundColor);
	return menuItem;
}

HUD *ShopState::CreateHud()
{
	HUD* hud = new HUD( );
	return hud;
}
//TODO Bug #3: Find and fix the bug in this function.	- Fixed
BaseObject *ShopState::CreateMouseCursor()
{
	BaseObject *cur = new BaseObject();
	ViewManager::GetInstance().AddObject(cur, 5);
	cur->SetTexture("Resources/images/reticle.png");
	cur->SetDimensions(32,32);
	menu.LinkCursor(cur);
	ships.LinkCursor(cur);
	weapons.LinkCursor(cur);
	shields.LinkCursor(cur);
	generators.LinkCursor(cur);
	props.LinkCursor(cur);
	scroll.LinkCursor(cur);
	cur->SetColor(0.5f,1,0,0);
	return cur;
}

// helper functions

void ShopState::GenerateShopMenus()
{
	char buffer[256];

	memset(buffer, 0, 256);
	MenuItem *item;
	// Ships
	for(unsigned int i = 0; i < SaiphApp::GetNumberOfShipPrototypes(); ++i)
	{
		shiptypes[i] = SaiphApp::CloneShipFromPrototype(i);
		ViewManager::GetInstance().RemoveObject(shiptypes[i]);
		if(shiptypes[i]->GetCost() >= 0)
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
	for(unsigned int i = 0; i < SaiphApp::GetNumberOfWeaponFlyweights(); ++i)
	{
		if(SaiphApp::GetWeaponFlyweight(i).levels[0].cost >=0)
		{
			for(unsigned int j = 0; j < SaiphApp::GetWeaponFlyweight(i).levels.size(); ++j)
			{
				item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));

				const WeaponFlyweight& wf = SaiphApp::GetWeaponFlyweight( i );
				sprintf(buffer, "%s Level:%d Energy Use: %.f Cost:%d", wf.name.c_str(), j+1,
					wf.levels[j].energy, wf.levels[j].cost);

				item->SetText(buffer);
				item->SetActionType(MenuItem::NONE);
				sprintf(buffer, "%d", i*100 + j);
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
	for(unsigned int i = 0; i < SaiphApp::GetNumberOfShieldFlyweights(); ++i)
	{
		if(SaiphApp::GetShieldFlyweight(i).cost >= 0)
		{
			item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
			sprintf(buffer, "Strength: %.f, Recharge: %.f/s, Energy Use: %.f/s Cost: %d"
				,SaiphApp::GetShieldFlyweight(i).strength
				,SaiphApp::GetShieldFlyweight(i).recharge
				,SaiphApp::GetShieldFlyweight(i).energy
				,SaiphApp::GetShieldFlyweight(i).cost);
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
	for(unsigned int i = 0; i < SaiphApp::GetNumberOfGeneratorFlyweights(); ++i)
	{
		if(SaiphApp::GetGeneratorFlyweight(i).cost >= 0)
		{
			item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
			sprintf(buffer, "Generates: %.f energy/s, Battery: %.f, Cost: %d"
				,SaiphApp::GetGeneratorFlyweight(i).energy
				,SaiphApp::GetGeneratorFlyweight(i).battery
				,SaiphApp::GetGeneratorFlyweight(i).cost);
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
	for(unsigned int i = 0; i < SaiphApp::GetNumberOfPropulsionFlyweights(); ++i)
	{
		if(SaiphApp::GetPropulsionFlyweight(i).cost >= 0)
		{
			item = CreateMenuItem(Color4f(1, 0.1f, 0.1f, 0.1f));
			sprintf(buffer, "Speed: %.f/s, Afterburner: %.f, Energy Use: %.f/s Cost: %d"
				,SaiphApp::GetPropulsionFlyweight(i).force
				,SaiphApp::GetPropulsionFlyweight(i).afterburnerEnergy
				,SaiphApp::GetPropulsionFlyweight(i).energy
				,SaiphApp::GetPropulsionFlyweight(i).cost);
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
	scroll.SetPosition(menu.GetPosition().x-60,menu.GetPosition().y);
	scroll.SetNumberShownItems(2);
}

//TODO Bug #4: Find and fix the bug in this function.	- Fixed
void ShopState::ComputeHudInformation()
{
	if(playerShip)
	{
		hud->SetBarPercent(HUD::ENERGY, playerShip->GetBattery() / SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
		hud->SetBarPercent(HUD::SHIELD, playerShip->GetShield() / SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength);
		hud->SetBarPercent(HUD::ARMOR, playerShip->GetArmor() / MAX_SHIP_ARMOR);
		hud->SetBarPercent(HUD::AFTERBURNER, playerShip->GetAfterburner() / SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
	}
	hud->Update();
}

//TODO Bug #5: Find and fix bug in this function.	- Fixed
void ShopState::ResetPlayer()
{
	if(playerShip)
	{
		ViewManager::GetInstance().RemoveObject(playerShip);
		delete playerShip;
		playerShip = nullptr;
	}
	playerShip = SaiphApp::CloneShipFromPrototype(SaiphApp::GetShipID());
	playerShip->SetXPosition(50.0f);
	playerShip->SetYPosition( WINDOW_HEIGHT / 2.0f);
	playerShip->SetHeading(-PI /2.0f);
	playerShip->SetShield(SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength);
	playerShip->SetBattery(SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
	playerShip->SetAfterburner(SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
}

//TODO Bug #6: Find and fix the bug in this function.	- Fixed
void ShopState::PlayerShieldEffect(const Vec2f &_impactorPosition)
{
	Effect *se = SaiphApp::CloneEffectFromPrototype(SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).animationID);
	se->SetDimensions(playerShip->GetDimensions());
	se->SetTarget(playerShip);
	Vec2f direction = _impactorPosition - playerShip->GetPosition();
	direction.Normalize();
	se->SetHeading(SaiphApp::GetHeadingFromUnitVector(direction));
	effects.push_back(se);
}
//TODO Bug #7: Find and fix the bug in this function.	- Fixed
void ShopState::GarbageCollect()
{
	unsigned int loop;

	//	Clean up Bullet objects
	for(loop=0;loop<bullets.size();)
	{
		if(!bullets[loop]->GetActive())
		{
			ViewManager::GetInstance().RemoveObject(bullets[loop]);
			delete bullets[loop];
			bullets.erase(bullets.begin() + loop);

		}
		else ++loop;

	}

	// Clean up effects
	for(loop=0;loop<effects.size();)
	{
		if(!effects[loop]->GetActive())
		{
			ViewManager::GetInstance().RemoveObject(effects[loop]);
			delete effects[loop];
			effects.erase(effects.begin() + loop);
		}
		else ++loop;
	}
}
//TODO Bug #8: Find and fix the bug in this function.	- Fixed
void ShopState::FireWeapon()
{
	Bullet *projectile;
	const WeaponLevelFlyweight &wlf = SaiphApp::GetWeaponFlyweight(SaiphApp::GetWeaponID()).levels[SaiphApp::GetWeaponLevel()];
	if((playerShip->GetBattery() > wlf.energy) && (playerShip->GetLastFired() >= wlf.reloadTime))
	{
		playerShip->SetBattery(playerShip->GetBattery() - wlf.energy);
		playerShip->SetLastFired(0);
		for(unsigned int i = 0; i < wlf.bullets.size(); ++i)
		{
			projectile = SaiphApp::CloneBulletFromPrototype(wlf.bullets[i]);
			projectile->SetPosition(playerShip->GetPosition());
			projectile->SetHeading(projectile->GetHeading() + playerShip->GetHeading());
			bullets.push_back(projectile);
		}
	}
}
//TODO Bug #9: Find and fix the bug in this function.	- Fixed
unsigned int ShopState::PerformSelection(bool _menu)
{
	unsigned int ret = NOCHANGE, temp;
	if(_menu)
	{
		selected = currentMenu->GetSelectedItem();
		if(selected)
		{

			if(submenu == "Ship Type")
			{
				if(alteredScore > 0 && selected->GetActionType() == MenuItem::NONE)
				{
					temp = atoi(selected->GetActionValue().c_str());
					SaiphApp::SetShipID(temp);
					ResetPlayer();
					SaiphApp::SetScore((unsigned int)alteredScore);
				}
			}
			else if(submenu == "Weapon")
			{
				if(alteredScore > 0 && selected->GetActionType() == MenuItem::NONE)
				{
					temp = atoi(selected->GetActionValue().c_str());
					SaiphApp::SetWeaponID(temp / 100);
					SaiphApp::SetWeaponLevel(temp % 100);
					SaiphApp::SetScore((unsigned int)alteredScore);
				}
			}
			else if(submenu == "Shield")
			{
				if(alteredScore > 0 && selected->GetActionType() == MenuItem::NONE)
				{
					temp = atoi(selected->GetActionValue().c_str());
					SaiphApp::SetShieldID(temp);
					SaiphApp::SetScore((unsigned int)alteredScore);
				}
				playerShip->SetShield(0);
			}
			else if(submenu == "Generator")
			{
				if(alteredScore > 0 && selected->GetActionType() == MenuItem::NONE)
				{
					temp = atoi(selected->GetActionValue().c_str());
					SaiphApp::SetGeneratorID(temp);
					SaiphApp::SetScore((unsigned int)alteredScore);
				}
				playerShip->SetBattery(0);
			}
			else if(submenu == "Propulsion")
			{
				if(alteredScore > 0 && selected->GetActionType() == MenuItem::NONE)
				{
					temp = atoi(selected->GetActionValue().c_str());
					SaiphApp::SetPropulsionID(temp);
					SaiphApp::SetScore((unsigned int)alteredScore);
				}
				playerShip->SetAfterburner(0);
			}

			if(selected->GetActionType() == MenuItem::SUBMENU)
				submenu = selected->GetActionValue();
			else if(selected->GetActionType() == MenuItem::STATECHANGE &&
				selected->GetActionValue() == "exit")
				ret = MENU;
		}
	}
	else
	{
		if(scroll.GetSelectedIndex())
			currentMenu->SelectItem(currentMenu->GetSelectedIndex() + 1);
		else
			currentMenu->SelectItem(currentMenu->GetSelectedIndex() - 1);
	}
	return ret;
}


// interface methods
unsigned int ShopState::Heartbeat(float _delta)
{
	unsigned int stateChange = NOCHANGE, temp;

	// change positions of menus based on which is current
	menu.Hide();
	ships.Hide();
	weapons.Hide();
	shields.Hide();
	generators.Hide();
	props.Hide();
	if(submenu == "Ship Type")
	{
		currentMenu = &ships;
		if(currentMenu->GetSelectedItem()->GetActionType() != MenuItem::SUBMENU)
		{
			alteredScore = SaiphApp::GetScore() - shiptypes[
				atoi(currentMenu->GetSelectedItem()->GetActionValue().c_str())]->GetCost() + playerShip->GetCost();
		}
	}
	else if(submenu == "Weapon")
	{
		currentMenu = &weapons;
		FireWeapon();
		if(currentMenu->GetSelectedItem()->GetActionType() != MenuItem::SUBMENU)
		{
			temp = atoi(currentMenu->GetSelectedItem()->GetActionValue().c_str());
			alteredScore = SaiphApp::GetScore() - SaiphApp::GetWeaponFlyweight(temp/100)
				.levels[temp - (temp / 100 * 100)].cost +
				SaiphApp::GetWeaponFlyweight(SaiphApp::GetWeaponID()).levels[SaiphApp::GetWeaponLevel()].cost;
		}
	}
	else if(submenu == "Shield")
	{
		if(SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength)
		{
			shieldTimer += _delta;
			if(shieldTimer >= SHIELD_INTERVAL)
			{
				shieldTimer = 0;
				PlayerShieldEffect(Vec2f(RandomFloat(0, 100).GenerateValue(),
					RandomFloat(WINDOW_HEIGHT / 2.0f - 50, WINDOW_HEIGHT / 2.0f + 50).GenerateValue()));
			}
		}
		currentMenu = &shields;
		if(currentMenu->GetSelectedItem()->GetActionType() != MenuItem::SUBMENU)
		{
			alteredScore = SaiphApp::GetScore() - SaiphApp::GetShieldFlyweight(
				atoi(currentMenu->GetSelectedItem()->GetActionValue().c_str())).cost +
				SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).cost;
		}
	}
	else if(submenu == "Generator")
	{
		currentMenu = &generators;
		if(currentMenu->GetSelectedItem()->GetActionType() != MenuItem::SUBMENU)
		{
			alteredScore = SaiphApp::GetScore() - SaiphApp::GetGeneratorFlyweight(
				atoi(currentMenu->GetSelectedItem()->GetActionValue().c_str())).cost +
				SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).cost;
		}
	}
	else if(submenu == "Propulsion")
	{
		currentMenu = &props;
		if(currentMenu->GetSelectedItem()->GetActionType() != MenuItem::SUBMENU)
		{
			alteredScore = SaiphApp::GetScore() - SaiphApp::GetPropulsionFlyweight(
				atoi(currentMenu->GetSelectedItem()->GetActionValue().c_str())).cost +
				SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).cost;
		}
	}
	else
	{
		currentMenu = &menu;
		alteredScore = SaiphApp::GetScore();
	}

	// Escape key
	if(GetAsyncKeyState(VK_ESCAPE) & 0x1)
	{
		currentMenu->SelectItem(currentMenu->GetNumberItems()-1);
		return PerformSelection(true);
	}

	// keyboard movement
	if(GetAsyncKeyState(VK_UP) & 0x1)
		currentMenu->SelectItem(currentMenu->GetSelectedIndex() - 1);
	else if(GetAsyncKeyState(VK_DOWN) & 0x1)
		currentMenu->SelectItem(currentMenu->GetSelectedIndex() + 1);

	// update hud
	ComputeHudInformation();

	// mouse movement
	static POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(FindWindow(WINDOW_TITLE, WINDOW_TITLE), &mouse);
	cursor->SetXPosition(float(mouse.x));
	cursor->SetYPosition(float(mouse.y));

	// Update bullets
	for(unsigned int i = 0; i < bullets.size(); ++i)
		if(bullets[i]->GetActive())
			bullets[i]->Heartbeat(_delta);

	// Update effects
	for(unsigned int i = 0; i < effects.size(); ++i)
		if(effects[i]->GetActive())
			effects[i]->Heartbeat(_delta);

	// Update ship
	if(playerShip)
	{
		playerShip->SetLastFired(playerShip->GetLastFired() + _delta);
		if(playerShip->GetBattery() > SaiphApp::GetShieldFlyweight(SaiphApp::GetPropulsionID()).energy * _delta)
			playerShip->SetBattery(playerShip->GetBattery() - SaiphApp::GetShieldFlyweight(SaiphApp::GetPropulsionID()).energy * _delta);
		playerShip->Heartbeat(_delta);
		// update shielding
		if(playerShip->GetShield() != SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength)
		{
			if(playerShip->GetBattery() > SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).energy * _delta)
			{
				playerShip->SetBattery(playerShip->GetBattery() - SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).energy * _delta);
				playerShip->SetShield(playerShip->GetShield() + SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).recharge * _delta);
				if(playerShip->GetShield() > SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength)
					playerShip->SetShield(SaiphApp::GetShieldFlyweight(SaiphApp::GetShieldID()).strength);
			}
		}
		// update battery
		playerShip->SetBattery(playerShip->GetBattery() + SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).energy * _delta);
		if(playerShip->GetBattery() > SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery)
		{
			playerShip->SetAfterburner(playerShip->GetAfterburner() + playerShip->GetBattery() - SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
			if(playerShip->GetAfterburner() > SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy)
				playerShip->SetAfterburner(SaiphApp::GetPropulsionFlyweight(SaiphApp::GetPropulsionID()).afterburnerEnergy);
			playerShip->SetBattery(SaiphApp::GetGeneratorFlyweight(SaiphApp::GetGeneratorID()).battery);
		}
	}

	// update menu
	currentMenu->SelectWithCursor();
	scroll.SelectWithCursor();
	currentMenu->Update();
	scroll.Update();

	// confirm keys/buttons
	if(GetAsyncKeyState(VK_RETURN) & 0x1)
		stateChange = PerformSelection(true);
	if(GetAsyncKeyState(VK_LBUTTON) & 0x1)
	{
		if(scroll.SelectWithCursor())
			stateChange = PerformSelection(false);
		else if(currentMenu->SelectWithCursor())
			stateChange = PerformSelection(true);
	}

	GarbageCollect();

	return stateChange;
}
//TODO Bug #10: Find and fix the bug in this function.	- Fixed
void ShopState::Init()
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
//TODO Bug #11: Find and fix the bug in this function.	- Fixed
void ShopState::RenderFrame() const
{
	char buffer[256];

	ViewManager::GetInstance().BeginScene();
	ViewManager::GetInstance().RenderText(10,20,"Upgrade Ship Menu");
	ViewManager::GetInstance().RenderObjects();
	currentMenu->RenderItemText();
	scroll.RenderItemText();
	sprintf(buffer, "Current Score: %.8d", SaiphApp::GetScore());
	ViewManager::GetInstance().RenderText(50, 57, buffer);
	sprintf(buffer, "With Selected: %.8d", alteredScore);
	if((int)SaiphApp::GetScore() == alteredScore)
		ViewManager::GetInstance().RenderText(50, 72, buffer);
	else if(alteredScore < (int)SaiphApp::GetScore())
		ViewManager::GetInstance().RenderText(50, 72, buffer, Color4f(1, 1, 0, 0));
	else
		ViewManager::GetInstance().RenderText(50, 72, buffer, Color4f(1, 0, 1, 0));

	// render hud text on top of everything else
	hud->Render();
	sprintf(buffer, "%.2f", playerShip->GetArmor());
	ViewManager::GetInstance().RenderText(725, 54, buffer, Color4f(1,0,1,0));
	sprintf(buffer, "%.2f", playerShip->GetShield());
	ViewManager::GetInstance().RenderText(725, 107, buffer, Color4f(1,0,1,1));
	sprintf(buffer, "%.2f", playerShip->GetAfterburner());
	ViewManager::GetInstance().RenderText(725, 160, buffer, Color4f(1,1,0,0));
	sprintf(buffer, "%.2f", playerShip->GetBattery());
	ViewManager::GetInstance().RenderText(725, 213, buffer, Color4f(1,1,1,0));

	ViewManager::GetInstance().EndScene();
}

//TODO Bug #12: Find and fix the bug in this function.	- Fixed
void ShopState::Shutdown()
{
	// Clean menus
	menu.ClearItems();
	menu.LinkCursor(NULL);
	ships.ClearItems();
	ships.LinkCursor(NULL);
	weapons.ClearItems();
	weapons.LinkCursor(NULL);
	shields.ClearItems();
	shields.LinkCursor(NULL);
	generators.ClearItems();
	generators.LinkCursor(NULL);
	props.ClearItems();
	props.LinkCursor(NULL);
	scroll.ClearItems();
	scroll.LinkCursor(NULL);

	// Clean up Model data
	for(unsigned int i = 0; i < bullets.size(); i++)
	{
		ViewManager::GetInstance().RemoveObject(bullets[i]);
		delete bullets[i];
	}
	bullets.clear();

	for(unsigned int i = 0; i < effects.size(); ++i)
	{
		ViewManager::GetInstance().RemoveObject(effects[i]);
		delete effects[i];
	}
	effects.clear();

	// player ship data
	ViewManager::GetInstance().RemoveObject(playerShip);
	if (playerShip) delete playerShip;
	playerShip = NULL;

	// reticle
	ViewManager::GetInstance().RemoveObject(cursor);
	if (cursor) delete cursor;
	cursor = NULL;

	// hud data
	if (hud) delete hud;
	hud = NULL;

	// shiptype data
	for(std::map<int, Ship *>::iterator iter = shiptypes.begin(); iter != shiptypes.end();)
	{
		ViewManager::GetInstance().RemoveObject(iter->second);
		delete iter->second;
		iter = shiptypes.erase(iter);
	}
	
}
#endif BUGS