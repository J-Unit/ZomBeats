#include "LevelSelectScene.h"
#include "LevelSelectScene2.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "ProgressScene.h"
#include "View.h"
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

	auto startButton0 = MenuItemImage::create("textures/calibrate.png", "textures/calibrate_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 0));
	startButton0->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET/2));
	startButton0->setScale(LEVEL_BUTTON_SCALE);

	auto startButton1 = MenuItemImage::create("textures/level1.png", "textures/level_1_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 1));
	startButton1->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton1->setScale(LEVEL_BUTTON_SCALE);

	auto startButton2 = MenuItemImage::create("textures/level2.png", "textures/level_2_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 2));
	startButton2->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton2->setScale(LEVEL_BUTTON_SCALE);

	auto startButton3 = MenuItemImage::create("textures/level3.png", "textures/level_3_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 3));
	startButton3->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET/2));
	startButton3->setScale(LEVEL_BUTTON_SCALE);

	auto startButton4 = MenuItemImage::create("textures/level4.png", "textures/level_4_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 4));
	startButton4->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton4->setScale(LEVEL_BUTTON_SCALE);

	auto startButton5 = MenuItemImage::create("textures/level5.png", "textures/level_5_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 5));
	startButton5->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton5->setScale(LEVEL_BUTTON_SCALE);

	auto startButton6 = MenuItemImage::create("textures/level6.png", "textures/level_6_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 6));
	startButton6->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton6->setScale(LEVEL_BUTTON_SCALE);

	auto startButton7 = MenuItemImage::create("textures/level7.png", "textures/level_7_clicked.png", CC_CALLBACK_1(LevelSelectScene::GoToGameScene, this, 7));
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
