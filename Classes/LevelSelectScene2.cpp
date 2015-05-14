#include "LevelSelectScene.h"
#include "LevelSelectScene2.h"
#include "LevelSelectScene3.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "ProgressScene.h"
#include "SaveSerializer.h"
#include "View.h"
USING_NS_CC;
using namespace cocos2d;

LevelSelectScene2::LevelSelectScene2()
{
}


LevelSelectScene2 ::~LevelSelectScene2()
{
}

Scene* LevelSelectScene2::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelSelectScene2::create();
	scene->addChild(layer);

	return scene;

}

bool LevelSelectScene2::init()
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

	int completedLevel = save.parseLevel(SAVE_LEVEL_FILE);

	MenuItemImage* startButton8;
	if (8 <= completedLevel) {
		startButton8 = MenuItemImage::create("textures/level8_completed.png", "textures/level8_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 8));
	}
	//next level we need to complete
	else if (8 == completedLevel + 1) {
		startButton8 = MenuItemImage::create("textures/level8.png", "textures/level8_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 8));
	}
	//could not unlock this level yet
	else {
		startButton8 = MenuItemImage::create("textures/level8.png", "textures/level8.png");
	}
	startButton8->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton8->setScale(LEVEL_BUTTON_SCALE);


	MenuItemImage* startButton9;
	if (9 <= completedLevel) {
		startButton9 = MenuItemImage::create("textures/level9_completed.png", "textures/level9_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 9));
	}
	//next level we need to complete
	else if (9 == completedLevel + 1) {
		startButton9 = MenuItemImage::create("textures/level9.png", "textures/level9_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 9));
	}
	//could not unlock this level yet
	else {
		startButton9 = MenuItemImage::create("textures/level9.png", "textures/level9.png");
	}
	startButton9->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton9->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton10;
	if (10 <= completedLevel) {
		startButton10 = MenuItemImage::create("textures/level10_completed.png", "textures/level10_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 10));
	}
	//next level we need to complete
	else if (10 == completedLevel + 1) {
		startButton10 = MenuItemImage::create("textures/level10.png", "textures/level10_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 10));
	}
	//could not unlock this level yet
	else {
		startButton10 = MenuItemImage::create("textures/level10.png", "textures/level10.png");
	}

	startButton10->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton10->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton11;
	if (11 <= completedLevel) {
		startButton11 = MenuItemImage::create("textures/level11_completed.png", "textures/level11_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 11));
	}
	//next level we need to complete
	else if (11 == completedLevel + 1) {
		startButton11 = MenuItemImage::create("textures/level11.png", "textures/level11_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 11));
	}
	//could not unlock this level yet
	else {
		startButton11 = MenuItemImage::create("textures/level11.png", "textures/level11.png");
	}

	startButton11->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton11->setScale(LEVEL_BUTTON_SCALE);



	MenuItemImage* startButton12;
	if (12 <= completedLevel) {
		startButton12 = MenuItemImage::create("textures/level12_completed.png", "textures/level12_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 12));
	}
	//next level we need to complete
	else if (12 == completedLevel + 1) {
		startButton12 = MenuItemImage::create("textures/level12.png", "textures/level12_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 12));
	}
	//could not unlock this level yet
	else {
		startButton12 = MenuItemImage::create("textures/level12.png", "textures/level12.png");
	}

	startButton12->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton12->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton13;
	if (13 <= completedLevel) {
		startButton13 = MenuItemImage::create("textures/level13_completed.png", "textures/level13_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 13));
	}
	//next level we need to complete
	else if (13 == completedLevel + 1) {
		startButton13 = MenuItemImage::create("textures/level13.png", "textures/level13_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 13));
	}
	//could not unlock this level yet
	else {
		startButton13 = MenuItemImage::create("textures/level13.png", "textures/level13.png");
	}

	startButton13->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton13->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton14;
	if (14 <= completedLevel) {
		startButton14 = MenuItemImage::create("textures/level14_completed.png", "textures/level14_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 14));
	}
	//next level we need to complete
	else if (14 == completedLevel + 1) {
		startButton14 = MenuItemImage::create("textures/level14.png", "textures/level14_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 14));
	}
	//could not unlock this level yet
	else {
		startButton14 = MenuItemImage::create("textures/level14.png", "textures/level14.png");
	}

	startButton14->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton14->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton15;
	if (15 <= completedLevel) {
		startButton15 = MenuItemImage::create("textures/level15_completed.png", "textures/level15_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 15));
	}
	//next level we need to complete
	else if (15 == completedLevel + 1) {
		startButton15 = MenuItemImage::create("textures/level15.png", "textures/level15_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToGameScene, this, 15));
	}
	//could not unlock this level yet
	else {
		startButton15 = MenuItemImage::create("textures/level15.png", "textures/level15.png");
	}


	startButton15->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton15->setScale(LEVEL_BUTTON_SCALE);






	auto backDirButton = MenuItemImage::create("textures/back_dir.png", "textures/back_dir.png", CC_CALLBACK_1(LevelSelectScene2::GoToPrevLevelScene, this));
	backDirButton->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	backDirButton->setScale(DIRECTION_BUTTON_SCALE);

	auto forwardDirButton = MenuItemImage::create("textures/forward_dir.png", "textures/forward_dir.png", CC_CALLBACK_1(LevelSelectScene2::GoToNextLevelScene, this));
	forwardDirButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	forwardDirButton->setScale(DIRECTION_BUTTON_SCALE);


	auto backButton = MenuItemImage::create("textures/back_button.png", "textures/back_button_clicked.png", CC_CALLBACK_1(LevelSelectScene2::GoToMainMenuScene, this));
	backButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.7, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET*1.25));
	backButton->setScale(BACK_BUTTON_SCALE);

	auto menu = Menu::create(startButton8, startButton9, startButton10, startButton11, startButton12, startButton13, startButton14, startButton15, backDirButton, forwardDirButton, backButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

/*
auto progressScene = ProgressScene::createScene();
//Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, progressScene));
Director::getInstance()->replaceScene(progressScene);
*/

void LevelSelectScene2::GoToGameScene(cocos2d::Ref *sender, int level)
{
	ProgressScene::createLayer(level);
	auto progressScene = ProgressScene::createScene();
	Director::getInstance()->replaceScene(progressScene);

}

void LevelSelectScene2::GoToPrevLevelScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelectScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));

}


void LevelSelectScene2::GoToNextLevelScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelectScene3::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));

}


void LevelSelectScene2::GoToMainMenuScene(cocos2d::Ref *sender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
}
