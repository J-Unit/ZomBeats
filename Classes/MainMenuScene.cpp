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

	auto startButton = MenuItemImage::create("textures/startButton.png", "textures/startButton/png", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
	startButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	startButton->setScale(0.5f);
	auto menu = Menu::create(startButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

void MainMenuScene::GoToGameScene(cocos2d::Ref *sender)
{
	GameController* gc = GameController::create();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gc->view->scene));

}