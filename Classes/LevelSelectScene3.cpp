#include "LevelSelectScene2.h"
#include "LevelSelectScene3.h"
#include "LevelSelectScene4.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "ProgressScene.h"
#include "SaveSerializer.h"
#include "View.h"
USING_NS_CC;
using namespace cocos2d;

LevelSelectScene3::LevelSelectScene3()
{
}


LevelSelectScene3 ::~LevelSelectScene3()
{
}

Scene* LevelSelectScene3::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelSelectScene3::create();
	scene->addChild(layer);

	return scene;

}

bool LevelSelectScene3::init()
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

	MenuItemImage* startButton16;
	if (16 <= completedLevel) {
		startButton16 = MenuItemImage::create("textures/level16_completed.png", "textures/level16_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 16));
	}
	//next level we need to complete
	else if (16 == completedLevel + 1) {
		startButton16 = MenuItemImage::create("textures/level16.png", "textures/level16_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 16));
	}
	//could not unlock this level yet
	else {
		startButton16 = MenuItemImage::create("textures/level16.png", "textures/level16.png");
	}
	startButton16->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton16->setScale(LEVEL_BUTTON_SCALE);


	MenuItemImage* startButton17;
	if (17 <= completedLevel) {
		startButton17 = MenuItemImage::create("textures/level17_completed.png", "textures/level17_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 17));
	}
	//next level we need to complete
	else if (17 == completedLevel + 1) {
		startButton17 = MenuItemImage::create("textures/level17.png", "textures/level17_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 17));
	}
	//could not unlock this level yet
	else {
		startButton17 = MenuItemImage::create("textures/level17.png", "textures/level17.png");
	}
	startButton17->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton17->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton18;
	if (18 <= completedLevel) {
		startButton18 = MenuItemImage::create("textures/level18_completed.png", "textures/level18_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 18));
	}
	//next level we need to complete
	else if (18 == completedLevel + 1) {
		startButton18 = MenuItemImage::create("textures/level18.png", "textures/level18_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 18));
	}
	//could not unlock this level yet
	else {
		startButton18 = MenuItemImage::create("textures/level18.png", "textures/level18.png");
	}

	startButton18->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton18->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton19;
	if (19 <= completedLevel) {
		startButton19 = MenuItemImage::create("textures/level19_completed.png", "textures/level19_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 19));
	}
	//next level we need to complete
	else if (19 == completedLevel + 1) {
		startButton19 = MenuItemImage::create("textures/level19.png", "textures/level19_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 19));
	}
	//could not unlock this level yet
	else {
		startButton19 = MenuItemImage::create("textures/level19.png", "textures/level19.png");
	}

	startButton19->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton19->setScale(LEVEL_BUTTON_SCALE);



	MenuItemImage* startButton20;
	if (20 <= completedLevel) {
		startButton20 = MenuItemImage::create("textures/level20_completed.png", "textures/level20_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 20));
	}
	//next level we need to complete
	else if (20 == completedLevel + 1) {
		startButton20 = MenuItemImage::create("textures/level20.png", "textures/level20_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 20));
	}
	//could not unlock this level yet
	else {
		startButton20 = MenuItemImage::create("textures/level20.png", "textures/level20.png");
	}

	startButton20->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton20->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton21;
	if (21 <= completedLevel) {
		startButton21 = MenuItemImage::create("textures/level21_completed.png", "textures/level21_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 21));
	}
	//next level we need to complete
	else if (21 == completedLevel + 1) {
		startButton21 = MenuItemImage::create("textures/level21.png", "textures/level21_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToGameScene, this, 21));
	}
	//could not unlock this level yet
	else {
		startButton21 = MenuItemImage::create("textures/level21.png", "textures/level21.png");
	}

	startButton21->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton21->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton22;
	if (22 <= completedLevel) {
		startButton22 = MenuItemImage::create("textures/level22_completed.png", "textures/level22_completed_clicked.png");
	}
	//next level we need to complete
	else if (22 == completedLevel + 1) {
		startButton22 = MenuItemImage::create("textures/level22.png", "textures/level22_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton22 = MenuItemImage::create("textures/level22.png", "textures/level22.png");
	}

	startButton22->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton22->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton23;
	if (23 <= completedLevel) {
		startButton23 = MenuItemImage::create("textures/level23_completed.png", "textures/level23_completed_clicked.png");
	}
	//next level we need to complete
	else if (23 == completedLevel + 1) {
		startButton23 = MenuItemImage::create("textures/level23.png", "textures/level23_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton23 = MenuItemImage::create("textures/level23.png", "textures/level23.png");
	}


	startButton23->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton23->setScale(LEVEL_BUTTON_SCALE);






	auto backDirButton = MenuItemImage::create("textures/back_dir.png", "textures/back_dir.png", CC_CALLBACK_1(LevelSelectScene3::GoToPrevLevelScene, this));
	backDirButton->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	backDirButton->setScale(DIRECTION_BUTTON_SCALE);

	auto forwardDirButton = MenuItemImage::create("textures/forward_dir.png", "textures/forward_dir.png", CC_CALLBACK_1(LevelSelectScene3::GoToNextLevelScene, this));
	forwardDirButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	forwardDirButton->setScale(DIRECTION_BUTTON_SCALE);


	auto backButton = MenuItemImage::create("textures/back_button.png", "textures/back_button_clicked.png", CC_CALLBACK_1(LevelSelectScene3::GoToMainMenuScene, this));
	backButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.7, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET*1.25));
	backButton->setScale(BACK_BUTTON_SCALE);

	auto menu = Menu::create(startButton16, startButton17, startButton18, startButton19, startButton20,
		startButton21, startButton22, startButton23, backDirButton, forwardDirButton, backButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

/*
auto progressScene = ProgressScene::createScene();
//Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, progressScene));
Director::getInstance()->replaceScene(progressScene);
*/

void LevelSelectScene3::GoToGameScene(cocos2d::Ref *sender, int level)
{
	ProgressScene::createLayer(level);
	auto progressScene = ProgressScene::createScene();
	Director::getInstance()->replaceScene(progressScene);

}

void LevelSelectScene3::GoToPrevLevelScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelectScene2::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));

}

void LevelSelectScene3::GoToNextLevelScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelectScene4::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));

}



void LevelSelectScene3::GoToMainMenuScene(cocos2d::Ref *sender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
}
