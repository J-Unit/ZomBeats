#include "GameController.h"
#include "View.h"
#include "cocos2d.h"
#pragma once

#define LEVEL_BUTTON_VERTICAL_OFFSET 200
#define LEVEL_BUTTON_HORIZONTAL_OFFSET 200
#define LEVEL_BACKGROUND_SCALE 1.25f
#define LEVEL_BUTTON_SCALE 1.2f
#define BACK_BUTTON_SCALE 2.2f
#define DIRECTION_BUTTON_SCALE 0.5f
#define LEVEL_MENU_TRANSITION_TIME 0.3f

class LevelSelectScene4 : public cocos2d::Layer
{
public:
	LevelSelectScene4();
	~LevelSelectScene4();

	static Scene* createScene();
	virtual bool init();

	void GoToGameScene(cocos2d::Ref* sender, int level);
	void GoToMainMenuScene(cocos2d::Ref* sender);
	void GoToPrevLevelScene(cocos2d::Ref* sender);
	CREATE_FUNC(LevelSelectScene4);
};

