#include "ProgressScene.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "View.h"
#include "ResourceLoader.h"

#define BAR_WIDTH 335
#define BAR_HEIGHT 10
#define BAR_OFFSET_X 29
#define BAR_OFFSET_Y 160

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
	loading = false;
	levelSet = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto loadingSprite = Sprite::create("textures/loading.png");
	loadingSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	loadingSprite->setScale(LEVEL_BACKGROUND_SCALE);

	bar = DrawNode::create();
	loadingSprite->addChild(bar, -1);


	this->addChild(loadingSprite);
	this->scheduleUpdate();

	return true;
}

void ProgressScene::update(float dt){
	if (loading){
		ResourceLoader *loader = ResourceLoader::getInstance();
		bar->clear();
		bar->drawSolidRect(Vec2(BAR_OFFSET_X, BAR_OFFSET_Y), Vec2(BAR_OFFSET_X + BAR_WIDTH * loader->getPercentage(), BAR_OFFSET_Y + BAR_HEIGHT), Color4F(0.2f, 0.3f, 0.8f, 1.0f));
		if (loader->isComplete()){
			gc = GameController::create();
			gc->setCurrentLevel(level);
			gc->initEnvironment();
			Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, gc->view->scene));
			this->unscheduleUpdate();
		}
	}
	else if (levelSet){
		ResourceLoader::loadContent();
		loading = true;
	}
}
