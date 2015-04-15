#include "MainMenuScene.h"
#include "GameController.h"
#include "View.h"
USING_NS_CC;

MainMenuScene::MainMenuScene()
{
}


MainMenuScene::~MainMenuScene()
{
}

Scene* MainMenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenuScene::create();
	scene->addChild(layer);

	return scene;

}

bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("textures/startBackground.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	backgroundSprite->setScale(0.5f);
	this->addChild(backgroundSprite);

	auto startButton0 = MenuItemImage::create("textures/levelButton0.png", "textures/levelButton0.png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this, 0));
	startButton0->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y +LEVEL_BUTTON_OFFSET));
	startButton0->setScale(LEVEL_BUTTON_SCALE);

	auto startButton1 = MenuItemImage::create("textures/levelButton1.png", "textures/levelButton1.png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this, 1));
	startButton1->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y+ LEVEL_BUTTON_OFFSET/2));
	startButton1->setScale(LEVEL_BUTTON_SCALE);

	auto startButton2 = MenuItemImage::create("textures/levelButton2.png", "textures/levelButton2.png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this, 2));
	startButton2->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	startButton2->setScale(LEVEL_BUTTON_SCALE);

	auto startButton3 = MenuItemImage::create("textures/levelButton3.png", "textures/levelButton3.png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this, 3));
	startButton3->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_OFFSET / 2));
	startButton3->setScale(LEVEL_BUTTON_SCALE);

	auto menu = Menu::create(startButton0, startButton1, startButton2 , startButton3, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

void MainMenuScene::GoToGameScene(cocos2d::Ref *sender, int level)
{
	GameController* gc = GameController::create();
	gc->setCurrentLevel(level);
	gc->initEnvironment();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gc->view->scene));
}
