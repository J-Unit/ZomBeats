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
	void GoToGameScene0(cocos2d::Ref* sender);
	void GoToGameScene1(cocos2d::Ref* sender);
	void GoToGameScene2(cocos2d::Ref* sender);
	void GoToGameScene3(cocos2d::Ref* sender);
};

