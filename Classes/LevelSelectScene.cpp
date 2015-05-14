#include "LevelSelectScene.h"
#include "LevelSelectScene2.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "ProgressScene.h"
#include "View.h"
#include "SaveSerializer.h"
USING_NS_CC;
using namespace cocos2d;

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

	int completedLevel = save.parseLevel(SAVE_LEVEL_FILE);

	auto startButton0 = MenuItemImage::create("textures/calibrate.png", "textures/calibrate_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 0));
	startButton0->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET/2));
	startButton0->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton1;
	if (1 <= completedLevel) {
		startButton1 = MenuItemImage::create("textures/level1_completed.png", "textures/level1_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 1));
	}
	//next level we need to complete
	else if (1 == completedLevel + 1) {
		startButton1 = MenuItemImage::create("textures/level1.png", "textures/level1_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 1));
	}
	//could not unlock this level yet
	else {
		startButton1 = MenuItemImage::create("textures/level1.png", "textures/level1.png");
	}
	startButton1->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton1->setScale(LEVEL_BUTTON_SCALE);


	MenuItemImage* startButton2;
	if (2 <= completedLevel) {
		startButton2 = MenuItemImage::create("textures/level2_completed.png", "textures/level2_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 2));
	}
	//next level we need to complete
	else if (2 == completedLevel + 1) {
		startButton2 = MenuItemImage::create("textures/level2.png", "textures/level2_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 2));
	}
	//could not unlock this level yet
	else {
		startButton2 = MenuItemImage::create("textures/level2.png", "textures/level2.png");
	}
	
	startButton2->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton2->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton3;
	if (3 <= completedLevel) {
		startButton3 = MenuItemImage::create("textures/level3_completed.png", "textures/level3_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 3));
	}
	//next level we need to complete
	else if (3 == completedLevel + 1) {
		startButton3 = MenuItemImage::create("textures/level3.png", "textures/level3_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 3));
	}
	//could not unlock this level yet
	else {
		startButton3 = MenuItemImage::create("textures/level3.png", "textures/level3.png");
	}
	
	startButton3->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET/2));
	startButton3->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton4;
	if (4 <= completedLevel) {
		startButton4 = MenuItemImage::create("textures/level4_completed.png", "textures/level4_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 4));
	}
	//next level we need to complete
	else if (4 == completedLevel + 1) {
		startButton4 = MenuItemImage::create("textures/level4.png", "textures/level4_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 4));
	}
	//could not unlock this level yet
	else {
		startButton4 = MenuItemImage::create("textures/level4.png", "textures/level4.png");
	}

	startButton4->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton4->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton5;
	if (5 <= completedLevel) {
		startButton5 = MenuItemImage::create("textures/level5_completed.png", "textures/level5_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 5));
	}
	//next level we need to complete
	else if (5 == completedLevel + 1) {
		startButton5 = MenuItemImage::create("textures/level5.png", "textures/level5_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 5));
	}
	//could not unlock this level yet
	else {
		startButton5 = MenuItemImage::create("textures/level5.png", "textures/level5.png");
	}
	
	
	startButton5->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton5->setScale(LEVEL_BUTTON_SCALE);


	MenuItemImage* startButton6;
	if (6 <= completedLevel) {
		startButton6 = MenuItemImage::create("textures/level6_completed.png", "textures/level6_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 6));
	}
	//next level we need to complete
	else if (6 == completedLevel + 1) {
		startButton6 = MenuItemImage::create("textures/level6.png", "textures/level6_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 6));
	}
	//could not unlock this level yet
	else {
		startButton6 = MenuItemImage::create("textures/level6.png", "textures/level6.png");
	}

	startButton6->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton6->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton7;
	if (7 <= completedLevel) {
		startButton7 = MenuItemImage::create("textures/level7_completed.png", "textures/level7_completed_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 7));
	}
	//next level we need to complete
	else if (7 == completedLevel + 1) {
		startButton7 = MenuItemImage::create("textures/level7.png", "textures/level7_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 7));
	}
	//could not unlock this level yet
	else {
		startButton7 = MenuItemImage::create("textures/level7.png", "textures/level7.png");
	}


	startButton7->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton7->setScale(LEVEL_BUTTON_SCALE);

	auto backDirButton = MenuItemImage::create("textures/back_dir.png", "textures/back_dir.png");
	backDirButton->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	backDirButton->setScale(DIRECTION_BUTTON_SCALE);

	auto forwardDirButton = MenuItemImage::create("textures/forward_dir.png", "textures/forward_dir.png", CC_CALLBACK_1(LevelSelectScene::GoToNextLevelScene, this));
	forwardDirButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	forwardDirButton->setScale(DIRECTION_BUTTON_SCALE);


	auto backButton = MenuItemImage::create("textures/back_button.png", "textures/back_button_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToMainMenuScene, this));
	backButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.7, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET*1.25));
	backButton->setScale(BACK_BUTTON_SCALE);

	auto menu = Menu::create(startButton0, startButton1, startButton2, startButton3, startButton4, startButton5, startButton6, startButton7, backDirButton, forwardDirButton, backButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

/*
	auto progressScene = ProgressScene::createScene();
	//Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, progressScene));
	Director::getInstance()->replaceScene(progressScene);
*/

void LevelSelectScene::GoToGameScene(cocos2d::Ref *sender, int level)
{
	ProgressScene::createLayer(level);
	auto progressScene = ProgressScene::createScene();
	Director::getInstance()->replaceScene(progressScene);

}

void LevelSelectScene::GoToNextLevelScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelectScene2::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
}


void LevelSelectScene::GoToMainMenuScene(cocos2d::Ref *sender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
}
