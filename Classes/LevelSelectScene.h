#include "GameController.h"
#include "View.h"
#include "cocos2d.h"
#pragma once

#define LEVEL_BUTTON_VERTICAL_OFFSET 200
#define LEVEL_BUTTON_HORIZONTAL_OFFSET 200
#define LEVEL_BACKGROUND_SCALE 1.5f
#define LEVEL_BUTTON_SCALE 1.2f
#define LEVEL_MENU_TRANSITION_TIME 0.3f

class LevelSelectScene : public cocos2d::Layer
{
public:
	LevelSelectScene();
	~LevelSelectScene();

	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(LevelSelectScene);
	void GoToGameScene(cocos2d::Ref* sender, int level);
};

