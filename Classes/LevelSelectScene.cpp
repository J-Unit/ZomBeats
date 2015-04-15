#include "LevelSelectScene.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "View.h"
USING_NS_CC;

LevelSelectScene::LevelSelectScene()
{
}


LevelSelectScene ::~LevelSelectScene()
{
}

Scene* LevelSelectScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelSelectScene::create();
	scene->addChild(layer);

	return scene;

}

bool LevelSelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->setScale(visibleSize.height / VIEW_HEIGHT);
	auto backgroundSprite = Sprite::create("textures/level_selection_bg.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	backgroundSprite->setScale(LEVEL_BACKGROUND_SCALE);
	this->addChild(backgroundSprite);

	auto startButton0 = MenuItemImage::create("textures/calibrate.png", "textures/calibrate_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 0));
	startButton0->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET/2, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET/2));
	startButton0->setScale(LEVEL_BUTTON_SCALE);

	auto startButton1 = MenuItemImage::create("textures/level_1.png", "textures/level_1_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 1));
	startButton1->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET / 2, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton1->setScale(LEVEL_BUTTON_SCALE);

	auto startButton2 = MenuItemImage::create("textures/level_2.png", "textures/level_2_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 2));
	startButton2->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET/2, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	startButton2->setScale(LEVEL_BUTTON_SCALE);

	auto startButton3 = MenuItemImage::create("textures/level_3.png", "textures/level_3_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 3));
	startButton3->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET/2, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET/2));
	startButton3->setScale(LEVEL_BUTTON_SCALE);

	auto startButton4 = MenuItemImage::create("textures/level_4.png", "textures/level_4_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 4));
	startButton4->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET/2, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton4->setScale(LEVEL_BUTTON_SCALE);

	auto backButton = MenuItemImage::create("textures/back_button.png", "textures/back_button_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToMainMenuScene, this));
	backButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.7, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET*1.25));
	backButton->setScale(BACK_BUTTON_SCALE);

	auto menu = Menu::create(startButton0, startButton1, startButton2, startButton3, startButton4, backButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

void LevelSelectScene::GoToGameScene(cocos2d::Ref *sender, int level)
{
	GameController* gc = GameController::create();
	gc->setCurrentLevel(level);
	gc->initEnvironment();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, gc->view->scene));
}

void LevelSelectScene::GoToMainMenuScene(cocos2d::Ref *sender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
}