#include "LevelSelectScene3.h"
#include "LevelSelectScene4.h"
#include "GameController.h"
#include "MainMenuScene.h"
#include "ProgressScene.h"
#include "SaveSerializer.h"
#include "View.h"
USING_NS_CC;
using namespace cocos2d;

LevelSelectScene4::LevelSelectScene4()
{
}


LevelSelectScene4 ::~LevelSelectScene4()
{
}

Scene* LevelSelectScene4::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelSelectScene4::create();
	scene->addChild(layer);

	return scene;

}

bool LevelSelectScene4::init()
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

	MenuItemImage* startButton24;
	if (24 <= completedLevel) {
		startButton24 = MenuItemImage::create("textures/level24_completed.png", "textures/level24_completed_clicked.png");
	}
	//next level we need to complete
	else if (24 == completedLevel + 1) {
		startButton24 = MenuItemImage::create("textures/level24.png", "textures/level24_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton24 = MenuItemImage::create("textures/level24.png", "textures/level24.png");
	}
	startButton24->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton24->setScale(LEVEL_BUTTON_SCALE);


	MenuItemImage* startButton25;
	if (25 <= completedLevel) {
		startButton25 = MenuItemImage::create("textures/level25_completed.png", "textures/level25_completed_clicked.png");
	}
	//next level we need to complete
	else if (25 == completedLevel + 1) {
		startButton25 = MenuItemImage::create("textures/level25.png", "textures/level25_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton25 = MenuItemImage::create("textures/level25.png", "textures/level25.png");
	}
	startButton25->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton25->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton26;
	if (26 <= completedLevel) {
		startButton26 = MenuItemImage::create("textures/level26_completed.png", "textures/level26_completed_clicked.png");
	}
	//next level we need to complete
	else if (26 == completedLevel + 1) {
		startButton26 = MenuItemImage::create("textures/level26.png", "textures/level26_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton26 = MenuItemImage::create("textures/level26.png", "textures/level26.png");
	}

	startButton26->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton26->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton27;
	if (27 <= completedLevel) {
		startButton27 = MenuItemImage::create("textures/level27_completed.png", "textures/level27_completed_clicked.png");
	}
	//next level we need to complete
	else if (27 == completedLevel + 1) {
		startButton27 = MenuItemImage::create("textures/level27.png", "textures/level27_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton27 = MenuItemImage::create("textures/level27.png", "textures/level27.png");
	}

	startButton27->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET / 2));
	startButton27->setScale(LEVEL_BUTTON_SCALE);



	MenuItemImage* startButton28;
	if (28 <= completedLevel) {
		startButton28 = MenuItemImage::create("textures/level28_completed.png", "textures/level28_completed_clicked.png");
	}
	//next level we need to complete
	else if (28 == completedLevel + 1) {
		startButton28 = MenuItemImage::create("textures/level28.png", "textures/level28_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton28 = MenuItemImage::create("textures/level28.png", "textures/level28.png");
	}

	startButton28->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton28->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton29;
	if (29 <= completedLevel) {
		startButton29 = MenuItemImage::create("textures/level29_completed.png", "textures/level29_completed_clicked.png");
	}
	//next level we need to complete
	else if (29 == completedLevel + 1) {
		startButton29 = MenuItemImage::create("textures/level29.png", "textures/level29_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton29 = MenuItemImage::create("textures/level29.png", "textures/level29.png");
	}

	startButton29->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton29->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton30;
	if (30 <= completedLevel) {
		startButton30 = MenuItemImage::create("textures/level30_completed.png", "textures/level30_completed_clicked.png");
	}
	//next level we need to complete
	else if (30 == completedLevel + 1) {
		startButton30 = MenuItemImage::create("textures/level30.png", "textures/level30_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton30 = MenuItemImage::create("textures/level30.png", "textures/level30.png");
	}

	startButton30->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton30->setScale(LEVEL_BUTTON_SCALE);

	MenuItemImage* startButton31;
	if (31 <= completedLevel) {
		startButton31 = MenuItemImage::create("textures/level31_completed.png", "textures/level31_completed_clicked.png");
	}
	//next level we need to complete
	else if (31 == completedLevel + 1) {
		startButton31 = MenuItemImage::create("textures/level31.png", "textures/level31_clicked.png");
	}
	//could not unlock this level yet
	else {
		startButton31 = MenuItemImage::create("textures/level31.png", "textures/level31.png");
	}


	startButton31->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.4, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET / 4));
	startButton31->setScale(LEVEL_BUTTON_SCALE);


	auto backDirButton = MenuItemImage::create("textures/back_dir.png", "textures/back_dir.png", CC_CALLBACK_1(LevelSelectScene4::GoToPrevLevelScene, this));
	backDirButton->setPosition(Point(visibleSize.width / 2 + origin.x - LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	backDirButton->setScale(DIRECTION_BUTTON_SCALE);

	auto forwardDirButton = MenuItemImage::create("textures/forward_dir.png", "textures/forward_dir.png");
	forwardDirButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*0.5, visibleSize.height / 2 + origin.y - LEVEL_BUTTON_VERTICAL_OFFSET));
	forwardDirButton->setScale(DIRECTION_BUTTON_SCALE);


	auto backButton = MenuItemImage::create("textures/back_button.png", "textures/back_button_clicked.png", CC_CALLBACK_1(LevelSelectScene4::GoToMainMenuScene, this));
	backButton->setPosition(Point(visibleSize.width / 2 + origin.x + LEVEL_BUTTON_HORIZONTAL_OFFSET*1.7, visibleSize.height / 2 + origin.y + LEVEL_BUTTON_VERTICAL_OFFSET*1.25));
	backButton->setScale(BACK_BUTTON_SCALE);

	auto menu = Menu::create(startButton24, startButton25, startButton26, startButton27,
		startButton28, startButton29, startButton30, startButton31, backDirButton, forwardDirButton, backButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	return true;
}

/*
auto progressScene = ProgressScene::createScene();
//Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, progressScene));
Director::getInstance()->replaceScene(progressScene);
*/

void LevelSelectScene4::GoToGameScene(cocos2d::Ref *sender, int level)
{
	ProgressScene::createLayer(level);
	auto progressScene = ProgressScene::createScene();
	Director::getInstance()->replaceScene(progressScene);

}

void LevelSelectScene4::GoToPrevLevelScene(cocos2d::Ref *sender)
{
	auto scene = LevelSelectScene3::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));

}
void LevelSelectScene4::GoToMainMenuScene(cocos2d::Ref *sender)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
}
