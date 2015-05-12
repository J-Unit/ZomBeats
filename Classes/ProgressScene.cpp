#include "ProgressScene.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "View.h"
USING_NS_CC;

ProgressScene *ProgressScene::layer;

ProgressScene::ProgressScene()
{
}


ProgressScene ::~ProgressScene()
{
}

Scene* ProgressScene::createScene()
{
	auto scene = Scene::create();
	//auto layer = ProgressScene::create();
	scene->addChild(layer);
	return scene;

}

void ProgressScene::createLayer(int l)
{
	layer = ProgressScene::create();
	layer->setLevel(l);
}

bool ProgressScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto loadingSprite = Sprite::create("textures/loading.png");
	loadingSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	loadingSprite->setScale(LEVEL_BACKGROUND_SCALE);

	this->addChild(loadingSprite);
	this->scheduleUpdate();

	return true;
}

void ProgressScene::update(float dt){
	if (levelSet){
		GameController* gc = GameController::create();
		gc->setCurrentLevel(level);
		gc->initEnvironment();
		Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, gc->view->scene));
		this->unscheduleUpdate();
	}
}
