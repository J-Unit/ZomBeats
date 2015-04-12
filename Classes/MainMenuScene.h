#include "GameController.h"
#include "View.h"
#include "cocos2d.h"
#pragma once
class MainMenuScene : public cocos2d::Layer
{
public:
	MainMenuScene();
	~MainMenuScene();

	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(MainMenuScene);
	void GoToGameScene(cocos2d::Ref* sender);
};

