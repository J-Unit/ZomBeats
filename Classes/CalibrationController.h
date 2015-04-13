#pragma once
#include "cocos2d.h"

class CalibrationController
{
public:
	CalibrationController();
	~CalibrationController();

	void init();
	void update(float deltaTime);
	int clicks;
	float totalOffset;
	bool audioCalibration;
	float audioDelay();
	float videoDelay();
	bool acceptClicks;

	cocos2d::Sprite *zombies[16];
	float zombieTimes[16];
};

