#include "GameController.h"
#include "View.h"
#include "cocos2d.h"
#pragma once

#define LEVEL_MENU_TRANSITION_TIME 0.3f
#define LEVEL_BACKGROUND_SCALE 1.25f

class ProgressScene : public cocos2d::Layer
{
public:
	ProgressScene();
	~ProgressScene();

	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(ProgressScene);
};

