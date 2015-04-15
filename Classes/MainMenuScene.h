#include "GameController.h"
#include "View.h"
#include "cocos2d.h"
#pragma once

#define LEVEL_BUTTON_OFFSET 200
#define LEVEL_BUTTON_SCALE 0.2f

class MainMenuScene : public cocos2d::Layer
{
public:
	MainMenuScene();
	~MainMenuScene();

	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(MainMenuScene);
	void GoToGameScene(cocos2d::Ref* sender, int level);
};

