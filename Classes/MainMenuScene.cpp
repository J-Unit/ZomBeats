#include "MainMenuScene.h"
#include "LevelSelectScene.h"
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

	auto backgroundSprite = Sprite::create("textures/start_screen_bg.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	backgroundSprite->setScale(MAIN_MENU_SCALE);
	this->addChild(backgroundSprite);

	auto startButton = MenuItemImage::create("textures/start_button.png", "textures/start_button_clicked.png", CC_CALLBACK_1(MainMenuScene::GoToLevelSelectScene, this));
	startButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - MAIN_MENU_BUTTON_OFFSET));
	startButton->setScale(MAIN_MENU_BUTTON_SCALE);

	auto menu = Menu::create(startButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

void MainMenuScene::GoToLevelSelectScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelectScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(MAIN_MENU_TRANSITION_TIME, scene));
}
