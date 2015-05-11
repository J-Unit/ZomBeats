#include "ProgressScene.h"
#include "GameController.h"
#include "View.h"
USING_NS_CC;

ProgressScene::ProgressScene()
{
}


ProgressScene ::~ProgressScene()
{
}

Scene* ProgressScene::createScene()
{
	auto scene = Scene::create();
	auto layer = ProgressScene::create();
	scene->addChild(layer);

	return scene;

}

bool ProgressScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

void ProgressScene::GoToGameScene(cocos2d::Ref *sender, int level)
{
	GameController* gc = GameController::create();
	gc->setCurrentLevel(level);
	gc->initEnvironment();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, gc->view->scene));
}
