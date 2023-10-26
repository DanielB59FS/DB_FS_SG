// HUD.h
// Interface for managing heads up display used by many states
// Code by Derek Bliss
// Created September 18, 2008
// Last updated October 7, 2008
//
#ifndef HUD_H
#define HUD_H

#include "ProgressBar.h"
#include "BaseObject.h"


class HUD
{
	float						numHits,				// Information displayed upon
								numMisses,				// completion of a level
								totalDamageReceived;
	BaseObject					background;				// Background model for HUD
	std::vector<ProgressBar>	progressBars;			// ProgressBars are used for information 
														// display, indexed by using BARS enum

public:
	HUD();
	virtual ~HUD();

	enum BARS {ENERGY, SHIELD, ARMOR, AFTERBURNER};

	// mutators
	void SetNumHits(float _numHits);
	void SetNumMisses(float _numMisses);
	void SetTotalDamageReceived(float _totalDamageReceived);
	void SetBarPercent(unsigned int _progressBar, float _percent);

	// accessors
	float GetNumHits() const;
	float GetNumMisses() const;
	float GetAccuracy() const;
	float GetTotalDamageReceived() const;
	float GetBarPercent(unsigned int _progressBar);

	// interface methods
	void HideBar(unsigned int _progressBar);			// Removes bar from render list
	void ShowBar(unsigned int _progressBar);			// Adds bar to render list
	void Update();										// Update all HUD Models
	void Render();										// Render all HUD Models
};
#endif