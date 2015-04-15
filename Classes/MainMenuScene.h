#include "GameController.h"
#include "View.h"
#include "LevelSelectScene.h"
#include "cocos2d.h"
#pragma once

#define MAIN_MENU_SCALE 1.5f
#define MAIN_MENU_BUTTON_SCALE 0.8f
#define MAIN_MENU_BUTTON_OFFSET 200
#define MAIN_MENU_TRANSITION_TIME 0.3f

class MainMenuScene : public cocos2d::Layer
{
public:
	MainMenuScene();
	~MainMenuScene();

	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(MainMenuScene);
	void GoToLevelSelectScene(cocos2d::Ref* sender);
};

