#include "pch.h"
#include "HUD.h"
#include "..\View\ViewManager.h"

#define HUD_BAR_WIDTH				100.0f
#define HUD_BAR_HEIGHT				16.0f
#define HUD_BAR_X_OFFSET			611.0f
#define HUD_BAR_MIN_WIDTH			2.0f


HUD::HUD()
{
	// set up level completion stats
	numMisses = numHits = totalDamageReceived = 0;

	// set up background
	background.SetTexture("Resources/images/hud.png");
	background.SetDimensions(256.0f, 1024.0f);
	background.SetPosition(728.0f, 512.0f);
	ViewManager::GetInstance().AddObject(&background, 4);

	// set up bars
	ProgressBar progressBar;
	// energy
	progressBar.SetLabelText("Energy");
	float yOffset = 0;

	if (SaiphApp::GetRenderer())
		yOffset = 11;
	else
		yOffset = WINDOW_HEIGHT * 0.02f;
	progressBar.SetLabelPosition(HUD_BAR_X_OFFSET, 206 - yOffset);
	progressBar.SetBarColor(1.0f, 1.0f, 1.0f, 0);
	progressBar.SetPosition(HUD_BAR_X_OFFSET + HUD_BAR_WIDTH / 2.0f, 203 + HUD_BAR_HEIGHT / 2.0f);
	progressBars.push_back(progressBar);
	// shield
	progressBar.SetLabelText("Shield");
	progressBar.SetLabelPosition(HUD_BAR_X_OFFSET, 100 - yOffset);
	progressBar.SetBarColor(1.0f, 0, 0, 1.0f);
	progressBar.SetPosition(HUD_BAR_X_OFFSET + HUD_BAR_WIDTH / 2.0f, 97 + HUD_BAR_HEIGHT / 2.0f);
	progressBars.push_back(progressBar);
	// armor
	progressBar.SetLabelText("Armor");
	progressBar.SetLabelPosition(HUD_BAR_X_OFFSET, 47 - yOffset);
	progressBar.SetBarColor(1.0f, 0, 1.0f, 0);
	progressBar.SetPosition(HUD_BAR_X_OFFSET + HUD_BAR_WIDTH / 2.0f, 44 + HUD_BAR_HEIGHT / 2.0f);
	progressBars.push_back(progressBar);
	// afterburner
	progressBar.SetLabelText("Afterburner");
	progressBar.SetLabelPosition(HUD_BAR_X_OFFSET, 153 - yOffset);
	progressBar.SetBarColor(1.0f, 1.0f, 0, 0);
	progressBar.SetPosition(HUD_BAR_X_OFFSET + HUD_BAR_WIDTH / 2.0f, 150 + HUD_BAR_HEIGHT / 2.0f);
	progressBars.push_back(progressBar);

}

HUD::~HUD()
{
	ViewManager::GetInstance().RemoveObject(&background);
}

// mutators
void HUD::SetNumHits(float _numHits)
{
	if(_numHits >= 0)
		numHits = _numHits;
}
void HUD::SetNumMisses(float _numMisses)
{
	if(_numMisses >= 0)
		numMisses = _numMisses;
}
void HUD::SetTotalDamageReceived(float _totalDamageReceived)
{
	if(_totalDamageReceived >= 0)
		totalDamageReceived = _totalDamageReceived;
}
void HUD::SetBarPercent(unsigned int _progressBar, float _percent)
{
	if(_progressBar < progressBars.size())
		progressBars[_progressBar].SetPercent(_percent);
}

// accessors
float HUD::GetNumHits() const {return numHits;}
float HUD::GetNumMisses() const {return numMisses;}
float HUD::GetAccuracy() const {return numHits / (numHits + numMisses) * 100.0f;}
float HUD::GetTotalDamageReceived() const {return totalDamageReceived;}
float HUD::GetBarPercent(unsigned int _progressBar) {return progressBars[_progressBar].GetPercent();}

// interface methods
void HUD::HideBar(unsigned int _progressBar)
{
	if(_progressBar < progressBars.size())
		progressBars[_progressBar].Hide();
}
void HUD::ShowBar(unsigned int _progressBar)
{
	if(_progressBar < progressBars.size())
		progressBars[_progressBar].Show();
}
void HUD::Update()
{
	for(unsigned int i = 0; i < progressBars.size(); ++i)
		progressBars[i].Update();
}
void HUD::Render()
{
	char displayTextBuffer[80] = {0};
	for(unsigned int i = 0; i < progressBars.size(); ++i)
		ViewManager::GetInstance().RenderText(progressBars[i].GetLabelXPosition(),
			progressBars[i].GetLabelYPosition(), progressBars[i].GetLabelText(),
			progressBars[i].GetBarColor());
	sprintf(displayTextBuffer, "Weapon Level : %d", SaiphApp::GetWeaponLevel() + 1);
	ViewManager::GetInstance().RenderText(608, 247, displayTextBuffer);
	sprintf(displayTextBuffer, "Level : %d", SaiphApp::GetLevel());
	ViewManager::GetInstance().RenderText(608, 300, displayTextBuffer);
	sprintf(displayTextBuffer, "Score : %.8d", SaiphApp::GetScore());
	ViewManager::GetInstance().RenderText(608, 353, displayTextBuffer);
}