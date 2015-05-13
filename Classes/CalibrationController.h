#pragma once
#include "cocos2d.h"

#define VIDEO_BEATS 8
#define AUDIO_BEATS 16

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

	cocos2d::Sprite *zombies[VIDEO_BEATS];
	float zombieTimes[VIDEO_BEATS];
	float phaseDelay;
};

