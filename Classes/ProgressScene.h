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

	static ProgressScene* layer;
	static Scene* createScene();
	static void createLayer(int l);
	virtual bool init();
	void update(float dt);

	void setLevel(int i){
		levelSet = true;
		level = i;
	}

	CREATE_FUNC(ProgressScene);

private:
	bool levelSet;
	int level;
};

