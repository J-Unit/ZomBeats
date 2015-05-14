//
//  GameController.cpp
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#include "GameController.h"
#include "SongDecomposition.h"
#include "ResourceLoader.h"
#include "View.h"
#include "InputController.h"
#include "FilmStrip.h"
#include "GameState.h"
#include "LevelMap.h"
#include "Wall.h"
#include "GrooveMeter.h"
#include "Sword.h"
#include "Ship.h"
#include "Lawnmower.h"
#include "Trashcan.h"
#include "Trash.h"
#include "Zombie.h"
#include "GoalObject.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include "CalibrationController.h"
#include "AIController.h"
#include "AudioController.h"
#include "Box2d/Box2d.h"
#include <ctime>
#include <stdlib.h>
#include "MainMenuScene.h"

#define STARTING_LEVEL 0
#define MAX_LEVELS 7

GameController *GameController::globalGC = NULL;

/**
* Initialize the game state.
*
* This is an overridden method inherited from the super(super) class Node.
* The Director calls this soon after start up.
*/

void GameController::BeginContact(b2Contact* contact){
	Type *b1, *b2;
	b1 = (Type *)contact->GetFixtureA()->GetBody()->GetUserData();
	b2 = (Type *)contact->GetFixtureB()->GetBody()->GetUserData();

	//trigger environment weapon activation
	EnvironmentWeapon *ewep;
	if ((b1->type == EnvironmentWeaponType && b2->type == WallType) || (b1->type == WallType && b2->type == EnvironmentWeaponType)){
		ewep = (b1->type == EnvironmentWeaponType) ? b1->getEnvironmentWeapon() : b2->getEnvironmentWeapon();
		ewep->hitWall = true;
		if (ewep == currentMower) {
			currentMower = NULL;
		}
		currentEnvironment = NULL;
		return;
	}

	//wall contact
	if (b1->type == WallType || b2->type == WallType) return;

	//weapon pickup
	Weapon *w;
	if ((b1->type == WeaponType && b2->type == ShipType) || (b1->type == ShipType && b2->type == WeaponType)){
		w = (b1->type == WeaponType) ? b1->getWeapon() : b2->getWeapon();
		w->isDesroyed = true;
		state->ship->hasWeapon = true;
		state->ship->currentWeapon = w;
		//set the sprite frame immediately to correspond to the change of weapon state
		int currentFrame = state->ship->sprite->getFrame();
		currentFrame += 3;
		state->ship->sprite->setFrame(currentFrame);
		view->redrawDurability(w->durability);

		return;
	}

	//trigger environment weapon activation
	if ((b1->type == EnvironmentWeaponType && b2->type == ShipType) || (b1->type == ShipType && b2->type == EnvironmentWeaponType)){
		EnvironmentWeapon *cew = (b1->type == EnvironmentWeaponType) ? b1->getEnvironmentWeapon() : b2->getEnvironmentWeapon();
		//ew->isUsed = true; //right now, no activation sequence
		if (!cew->onCooldown){
			currentEnvironment = (b1->type == EnvironmentWeaponType) ? b1->getEnvironmentWeapon() : b2->getEnvironmentWeapon();
			if (currentEnvironment->e_weapon_type == 1){
				audio->playEffect("sound_effects/RummagingThroughTrash.mp3",2.0f);
			}
			state->ship->isActivatingEnvironment = true; //right now, no activation sequence
			state->ship->hasEnvironmentWeapon = true;
		}
		//state->ship->isActivatingEnvironment = false; 
		return;
	}

	Zombie *zb;
	EnvironmentWeapon *envwep;
	//trigger environment weapon activation
	if ((b1->type == EnvironmentWeaponType && b2->type == ZombieType) || (b1->type == ZombieType && b2->type == EnvironmentWeaponType)){
		envwep = (b1->type == EnvironmentWeaponType) ? b1->getEnvironmentWeapon() : b2->getEnvironmentWeapon();
		if (envwep->hasMoved){
			zb = (b1->type == ZombieType) ? b1->getZombie() : b2->getZombie();
			zb->isDestroyed = true;
			if (envwep->e_weapon_type == 2){
				//if its trash then it dies too
				envwep->hitWall = true;
			}
			return;
		}
		return;
	}

	//collecting goal item
	GoalObject *go;
	if ((b1->type == GoalType && b2->type == ShipType) || (b1->type == ShipType && b2->type == GoalType)){
		go = (b1->type == GoalType) ? b1->getGoalObject() : b2->getGoalObject();
		go->isCollected = true; //signal to remove goal object
		hasCollectedGoal = true; //signal win condition
	}

	//fucking up zombies
	Zombie *z;
	if ((b1->type == ZombieType && b2->type == ShipType) || (b1->type == ShipType && b2->type == ZombieType)){
		state->ship->isDestroyed = true;
	}

}

void GameController::EndContact(b2Contact* contact){

}


//purpose of this function is to set a level when choosing from start menu
void GameController::setCurrentLevel(int level){
	currentLevel = level;
}

void GameController::createFog() {
	//add the fog of war here
	fogSp = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("fog"));
	
	if(currentLevel == CALIBRATION_LEVEL) fogSp->setVisible(false);
	//fogSpOuter = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("fog_outer"));
	fogSp->setScale(FOG_SCALE, FOG_SCALE);
	//fogSpOuter->setScale(OUTER_FOG_SCALE, OUTER_FOG_SCALE);
	fogSp->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
	//fogSpOuter->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
	view->enviornment->addChild(fogSp, 3); //the z-order of fog should be the largest 
	//view->enviornment->addChild(fogSpOuter);
}

void GameController::createGameMenu() {

	/*pauseMenu = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("pause_menu"));
	pauseMenu->setScale(0.15f);
	pauseMenu->setPosition(Vec2(HUD_OFFSET.x * 50, HUD_OFFSET.y * 35));
	this->addChild(pauseMenu);*/
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	pauseMenuBackground = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("blank_pause_menu"));
	pauseMenuBackground->setScale(GAME_MENU_SCALE);
	pauseMenuBackground->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(pauseMenuBackground);

	auto resumeButton = MenuItemImage::create("textures/resume_button.png", "textures/resume_button_clicked.png", CC_CALLBACK_0(GameController::resumeGame, this));
	resumeButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + GAME_MENU_BUTTON_OFFSET));
	resumeButton->setScale(GAME_MENU_BUTTON_SCALE);

	auto restartButton = MenuItemImage::create("textures/restart_button.png", "textures/restart_button_clicked.png", CC_CALLBACK_0(GameController::restartGame, this));
	restartButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	restartButton->setScale(GAME_MENU_BUTTON_SCALE);

	auto homeButton = MenuItemImage::create("textures/home_button.png", "textures/home_button_clicked.png", CC_CALLBACK_0(GameController::goBackToMainMenu, this));
	homeButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - GAME_MENU_BUTTON_OFFSET));
	homeButton->setScale(GAME_MENU_BUTTON_SCALE);

	menu = Menu::create(resumeButton, restartButton, homeButton, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

}

void GameController::removeGameMenu() {
	this->removeChild(pauseMenuBackground);
	this->removeChild(menu);
}

void GameController::updateFog() {
	if (fogSp != NULL) {
		//if (INITIAL_DETECTION_RADIUS / detectionRadius > 0.65f) {
		fogSp->setScale(FOG_SCALE + (meter->detectionRadius - MIN_DETECTION_RADIUS) / 120.0f, FOG_SCALE + (meter->detectionRadius - MIN_DETECTION_RADIUS) / 120.0f);

		//}

		fogSp->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
		//fogSpOuter->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
	}
}

void GameController::createCountDown() {
	countDown = FilmStrip::create(ResourceLoader::getInstance()->getTexture("count_down"), 1, 3, 3);
	countDown->setScale(COUNT_DOWN_SCALE, COUNT_DOWN_SCALE);
	countDown->setPosition(Vec2(HUD_OFFSET.x*50.0f, HUD_OFFSET.y * 35.5)); //change it later
	view->resIndepScreen->addChild(countDown, 5);
	countDown->setFrame(0);
	countDownCounter = 0;
	panning();
}

void GameController::panning() {
	view->allSpace->setScale(PANNING_SCALE);
	Vec2 anchor(0.4f, 0.6f);
	view->allSpace->setAnchorPoint(anchor);
	view->durability->setVisible(false);
	view->durabilityBox->setVisible(false);
}

void GameController::unPanning() {
	createFog();
	view->allSpace->setScale(GAME_VIEW_SCALE);
	Vec2 anchor(0.5f, 0.5f);
	view->allSpace->setAnchorPoint(anchor);
	view->durability->setVisible(true);
	view->durabilityBox->setVisible(true);
	view->allSpace->addChild(state->ship->getSprite()); //add ricky sprite back to canvas
	createPauseButton();
}


/**
* Initialize the game state.
*
* This is an overridden method inherited from the super(super) class Node.
* The Director calls this soon after start up.
*/
bool GameController::init() {
	if (!Layer::init()) {
		return false;
	}
	globalGC = this;
	meter = new GrooveMeter();
	currentEnvironmentMeter = 0.3f;
	activationDelay = true;
	doneActivating = false;
	hasCollectedGoal = false;
	Director* director = Director::getInstance();
	Size winsize = director->getWinSizeInPixels();
	view = new View(winsize.width, winsize.height);
	view->scene->addChild(this);
	state = NULL;
	currentEnvironment = currentMower = NULL;
	currentTrash = NULL;
	audio = new AudioController();
	save.parseSave(this, SAVE_FILE);
	return true;
}
/**
* Intialize the environment of the game, this is the real method that populates the game
* state and level
*/
void GameController::initEnvironment() {
	// Start listening to input
	input = new InputController(_eventDispatcher);
	input->startInput();
	calibration = new CalibrationController();
	loadLevel(currentLevel);

	ai = new AIController();

	//add the fog of war here
	musicNoteCounter = 0; //initialize the sequence of music note path
	// Tell the director we are ready for animation.
	this->scheduleUpdate();
	isPaused = false;
	//for calibration level we need to initially pause the game to let
	//kids read the instruction
	if (currentLevel == CALIBRATION_LEVEL){
		pauseGameOnly();
		calibration->acceptClicks = false;
		beginCountDown = false; //no need to do count down in calibration level
		createPauseButton();
	}
	//for normal levels, we only need to pause the games 
	else {
		pauseGameOnly();
		beginCountDown = true;
		createCountDown();
	}

	//ricky texture not initially removed
	removed = false;


}

//helper method, create pauseButton on the upper right corner
void GameController::createPauseButton() {
	auto pauseButton = MenuItemImage::create("textures/pause_button.png", "textures/pause_button_clicked.png", CC_CALLBACK_0(GameController::pauseGame, this));
	Size visibleSize2 = Director::getInstance()->getVisibleSize();
	Vec2 origin2 = Director::getInstance()->getVisibleOrigin();
	pauseButton->setPosition(Point(visibleSize2.width*0.97 + origin2.x, visibleSize2.height*0.96 + origin2.y));
	pauseButton->setScale(0.2f);

	auto pauseButtonMenu = Menu::create(pauseButton, NULL);
	pauseButtonMenu->setPosition(Point::ZERO);
	this->addChild(pauseButtonMenu);
}

//create a okay button for the calibration level
void GameController::createOkButton(int index) {
	if (index == 0) {
		auto okButton = MenuItemImage::create("textures/ok_button.png", "textures/ok_clicked.png", CC_CALLBACK_0(GameController::resumeGameOnly, this, 0));
		okButton->setPosition(Vec2(HUD_OFFSET.x*70.5f, HUD_OFFSET.y * 5));
		okButton->setScale(0.2f);

		okButtonMenu = Menu::create(okButton, NULL);
		okButtonMenu->setPosition(Point::ZERO);
		view->resIndepScreen->addChild(okButtonMenu, 5);
	}
	else {
		auto okButton = MenuItemImage::create("textures/ok_button.png", "textures/ok_clicked.png", CC_CALLBACK_0(GameController::resumeGameOnly, this, 1));
		okButton->setPosition(Vec2(HUD_OFFSET.x*70.5f, HUD_OFFSET.y * 5));
		okButton->setScale(0.2f);

		okButtonMenu = Menu::create(okButton, NULL);
		okButtonMenu->setPosition(Point::ZERO);
		view->resIndepScreen->addChild(okButtonMenu, 5);
	}
}


Vec2 GameController::mouseToWorld(Vec2 click){
	b2Vec2 pos = state->ship->body->GetPosition();
	return Vec2((input->lastClick.x - view->screen_size_x / 2.0) / view->resIndepScreen->getScale() / view->allSpace->getScale() + pos.x,
		-(input->lastClick.y - view->screen_size_y / 2.0) / view->resIndepScreen->getScale() / view->allSpace->getScale() + pos.y);
}

bool GameController::hasWonLevel(){
	//add 2 fields to game state or game controller
	//target num zombies 
	//pointer to collectable in level...can be null if level doesnt have collectable
	if (currentLevel != CALIBRATION_LEVEL) {
		//logic for multiple goals
		if (state->instrument != NULL){
			//the level has a collectable item
			//check if hasCollectedGoal and numzombies is below threshold
			if (hasCollectedGoal && state->zombies.GetCount() <= state->zomGoal){ //TODO: add zombie threshold here
				hasCollectedGoal = false;
				return true;
			}
			else return false;

		}
		else{
			//no collectable item on level
			//just check if numzombies is below threshold
			//they start out equal on backwards compatible levels
			int i = state->zombies.GetCount();
			int j = state->zomGoal;
			return state->zombies.GetCount() <= state->zomGoal;
		}

	}
	else {
		if (calibration->audioCalibration) return audio->songIsOver() && calibration->phaseDelay > 2.5f;
		else return elapsedTime - calibration->zombieTimes[calibration->clicks] > 3.0f;
	}
}


void GameController::loadLevel(int i){
	currentLevel = i;
	destination = 0;
	isPaused = false;
	input->clickProcessed = true;
	dRickyTap = new Vec2(1.0f, 0.0f);
	dRickyTap->normalize();
	environmentalTimer = 0.0f;
	trashTimer = 0.0f;
	processDirection = false;
	if (currentEnvironment != NULL) currentEnvironment = NULL;//CC_SAFE_DELETE(currentEnvironment);
	if (currentMower != NULL) currentMower = NULL;//CC_SAFE_DELETE(currentMower);
	if (currentTrash != NULL) currentTrash = NULL;//CC_SAFE_DELETE(currentTrash);
	elapsedTime = 0.0;
	stringstream ss;
	ss << "levels/level" << currentLevel << ".zbl";
	if (state != NULL) CC_SAFE_DELETE(state);
	state = ls.parseLevel(ss.str());
	state->world->SetContactListener(this);

	// Build the scene graph and create the ship model.

	ls.addObjects(state);
	//view->releaseScene();
	this->removeAllChildrenWithCleanup(true);
	view->buildScene(state->level, this, i);
	view->createMusicNotePath(musicNotes);

	if (currentLevel == CALIBRATION_LEVEL) {
		view->allSpace->addChild(state->ship->getSprite());
	}
	//createFog();
	for (int i = 0; i<state->level->nWalls; i++) view->enviornment->addChild(state->level->walls[i].sprite);
	for (CTypedPtrDblElement<Zombie> *cz = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(cz); cz = cz->Next()) view->zombies->addChild(cz->Data()->sprite, 2);
	for (CTypedPtrDblElement<Weapon> *cw = state->weapons.GetHeadPtr(); !state->weapons.IsSentinel(cw); cw = cw->Next()) view->enviornment->addChild(cw->Data()->sprite, 2);
	for (CTypedPtrDblElement<EnvironmentWeapon> *ew = state->environment_weapons.GetHeadPtr(); !state->environment_weapons.IsSentinel(ew); ew = ew->Next()) {
		view->enviornment->addChild(ew->Data()->sprite, 2);
		ew->Data()->beingUsed = false;
	}
	if (state->instrument != NULL){
		view->enviornment->addChild(state->instrument->sprite, 2);
	}
	//initial detection radius
	meter->detectionRadius = INITIAL_DETECTION_RADIUS;
	currAwareness = 0.0f;
	currentFingerPos = Vec2(0.0f, 0.0f);
	if (currentLevel == CALIBRATION_LEVEL){
		audio->paused = true;
		view->durabilityBox->setVisible(false);
		view->durability->setVisible(false);
		calibration->init();
		audio->audioDelay = audio->videoDelay = 0.0f;
		Size visibleSizeDialogue = Director::getInstance()->getVisibleSize();
		Vec2 originDialogue = Director::getInstance()->getVisibleOrigin();

		popup = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("dialogue_popup"));
		popup->setScale(DIALOGUE_POPUP_SCALE);
		popup->setPosition(Vec2(HUD_OFFSET.x*44.5, HUD_OFFSET.y*10));
		//popup->setPosition(Point(visibleSizeDialogue.width / 2, visibleSizeDialogue.height / 7));
		view->resIndepScreen->addChild(popup, 4);
		createOkButton(0);
		view->objective->setString("Audio Calibration: Listen to the four warm-up beats,\nthen start tapping the screen to the beat, and\ndon't miss any!  Click ok to start.");

	}
	//for normal levels, may need to do sth here later
	else{
		
	}
}

//restart the game upon death or reset
void GameController::restartGame() {
	audio->stop(); //stop all songs upon restart
	loadLevel(currentLevel);
	if (currentLevel == CALIBRATION_LEVEL){
		isPaused = true;
		createPauseButton();
	}
	//normal level
	else {
		beginCountDown = true;
		createCountDown();
	}

	removeGameMenu();
	removed = false;
}

void GameController::goBackToMainMenu() {
	audio->stop();
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
	globalGC = NULL;
}

//pause the game and also create a pause menu
void GameController::pauseGame() {
	if (isPaused) return;
	isPaused = true;
	if (currentLevel == CALIBRATION_LEVEL){
		audio->pauseSounds();
	}
	createGameMenu();
	
}

//only pause the game without creating the game menu,
//used ONLY in calibration level
void GameController::pauseGameOnly() {
	isPaused = true;
}



void GameController::resumeGame() {
	input->clickProcessed = true;
	isPaused = false;
	if (currentLevel == CALIBRATION_LEVEL){
		audio->resumeSounds();
	}
	removeGameMenu();
}

//used ONLY in calibration level
void GameController::resumeGameOnly(int index) {
	input->clickProcessed = true;
	isPaused = false;
	view->resIndepScreen->removeChild(okButtonMenu);
	if (index == 0) {
		view->objective->setString("Audio Calibration: After the four warm-up beats, tap\nthe screen to the beat, and don't miss any!");
		audio->playTrack(ls.getLevelTrack(), currentLevel != CALIBRATION_LEVEL);
		audio->paused = false;
	}
	else if (index == 1) {
		for (int i = 0; i < VIDEO_BEATS; i++){
			calibration->zombieTimes[i] = elapsedTime * 2 + 1000000;
		}
		view->objective->setString("Video: Tap when center of each orange triangle\nlines up with the blue triangle, don't miss any!");
	}
	calibration->acceptClicks = true;
}

void GameController::createEnvironmentActivationMeter(float meterWidth, float meterLength, float percentFull, b2Vec2 dir){
	outterEnvironmentActivation[0] = b2Vec2(-meterLength / 2.0f, meterWidth / 2.0f); //top left
	outterEnvironmentActivation[1] = b2Vec2(meterLength / 2.0f, meterWidth / 2.0f); //top right
	outterEnvironmentActivation[2] = b2Vec2(meterLength / 2.0f, -meterWidth / 2.0f); //bottom right 
	outterEnvironmentActivation[3] = b2Vec2(-meterLength / 2.0f, -meterWidth / 2.0f); //bottom left

	innerEnvironmentActivation[0] = b2Vec2(-meterLength / 2.0f, meterWidth / 2.0f); //top left
	innerEnvironmentActivation[1] = b2Vec2(-meterLength /2.0f + (meterLength * percentFull), meterWidth / 2.0f); //top right
	innerEnvironmentActivation[2] = b2Vec2(-meterLength / 2.0f + (meterLength * percentFull), -meterWidth / 2.0f); //bottom right
	innerEnvironmentActivation[3] = b2Vec2(-meterLength / 2.0f, -meterWidth / 2.0f); //bottom left

	float theta = atan2(dir.y, dir.x);
	b2Rot rotationM = b2Rot(theta);

	//add ricky pos to each point and translate box in front by mult dir facing * half range weap
	for (int i = 0; i < sizeof(outterEnvironmentActivation) / sizeof(b2Vec2); i++){
		b2Vec2 rickPos = state->ship->body->GetPosition();
		outterEnvironmentActivation[i] = b2Vec2(outterEnvironmentActivation[i].y, outterEnvironmentActivation[i].x);
		outterEnvironmentActivation[i] = b2MulT(rotationM, outterEnvironmentActivation[i]);
		outterEnvironmentActivation[i] = b2Vec2(outterEnvironmentActivation[i].y, outterEnvironmentActivation[i].x);
		outterEnvironmentActivation[i] = outterEnvironmentActivation[i] + rickPos + ((meterLength / 2.0f + SHIP_HEIGHT)*dir);

		transformedOuterEnvironmnetActivation[i] = Vec2(outterEnvironmentActivation[i].x, outterEnvironmentActivation[i].y);

		innerEnvironmentActivation[i] = b2Vec2(innerEnvironmentActivation[i].y, innerEnvironmentActivation[i].x);
		innerEnvironmentActivation[i] = b2MulT(rotationM, innerEnvironmentActivation[i]);
		innerEnvironmentActivation[i] = b2Vec2(innerEnvironmentActivation[i].y, innerEnvironmentActivation[i].x);
		innerEnvironmentActivation[i] = innerEnvironmentActivation[i] + rickPos + ((meterLength / 2.0f + SHIP_HEIGHT)*dir);

		transformedInnerEnvironmentActivation[i] = Vec2(innerEnvironmentActivation[i].x, innerEnvironmentActivation[i].y);

	}
	view->weaponBox->clear();
	view->weaponBox->drawPoly(transformedOuterEnvironmnetActivation,4, true, ccColor4F(0.0f, 0.0f, 0.0f, 1.0f));
	view->weaponBox->drawSolidPoly(transformedInnerEnvironmentActivation, 4, ccColor4F(1.0f, 0.0f, 0.0f, 0.6f));
	//view->weaponBox->drawRect(Vec2(outterEnvironmentActivation[0].x, outterEnvironmentActivation[0].y), Vec2(outterEnvironmentActivation[1].x, outterEnvironmentActivation[1].y), Vec2(outterEnvironmentActivation[3].x, outterEnvironmentActivation[3].y), Vec2(outterEnvironmentActivation[2].x, outterEnvironmentActivation[2].y), ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));
	//view->weaponBox->drawRect(Vec2(innerEnvironmentActivation[0].x, innerEnvironmentActivation[0].y), Vec2(innerEnvironmentActivation[1].x, innerEnvironmentActivation[1].y), Vec2(innerEnvironmentActivation[3].x, innerEnvironmentActivation[3].y), Vec2(innerEnvironmentActivation[2].x, innerEnvironmentActivation[2].y), ccColor4F(1.0f, 0.0f, 0.0f, 1.0f));
}

void GameController::createWeaponRanges(float weapWidth, float weapRange, float weapDetectionRange, b2Vec2 dir){
	weaponRectangle[0] = b2Vec2(-weapRange / 2.0f, weapWidth / 2.0f); //top left
	weaponRectangle[1] = b2Vec2(weapRange / 2.0f, weapWidth / 2.0f); //top right
	weaponRectangle[2] = b2Vec2(-weapRange / 2.0f, -weapWidth / 2.0f); //bottom left
	weaponRectangle[3] = b2Vec2(weapRange / 2.0f, -weapWidth / 2.0f); //bottom right 

	weaponDetectionRectangle[0] = b2Vec2(-weapRange / 2.0f, weapWidth / 2.0f); //top left
	weaponDetectionRectangle[1] = b2Vec2(weapDetectionRange / 2.0f, weapWidth / 2.0f); //top right
	weaponDetectionRectangle[2] = b2Vec2(-weapRange / 2.0f, -weapWidth / 2.0f); //bottom left
	weaponDetectionRectangle[3] = b2Vec2(weapDetectionRange / 2.0f, -weapWidth / 2.0f); //bottom right

	float theta = atan2(dir.y, dir.x);
	b2Rot rotationM = b2Rot(theta);

	//add ricky pos to each point and translate box in front by mult dir facing * half range weap
	for (int i = 0; i < sizeof(weaponRectangle) / sizeof(b2Vec2); i++){
		b2Vec2 rickPos = state->ship->body->GetPosition();
		weaponRectangle[i] = b2Vec2(weaponRectangle[i].y, weaponRectangle[i].x);
		weaponRectangle[i] = b2MulT(rotationM, weaponRectangle[i]);
		weaponRectangle[i] = b2Vec2(weaponRectangle[i].y, weaponRectangle[i].x);
		weaponRectangle[i] = weaponRectangle[i] + rickPos + ((weapRange / 2.0f + SHIP_HEIGHT)*dir);

		weaponDetectionRectangle[i] = b2Vec2(weaponDetectionRectangle[i].y, weaponDetectionRectangle[i].x);
		weaponDetectionRectangle[i] = b2MulT(rotationM, weaponDetectionRectangle[i]);
		weaponDetectionRectangle[i] = b2Vec2(weaponDetectionRectangle[i].y, weaponDetectionRectangle[i].x);
		weaponDetectionRectangle[i] = weaponDetectionRectangle[i] + rickPos + ((weapRange / 2.0f + SHIP_HEIGHT)*dir);

	}
	view->weaponBox->clear();
	view->weaponBox->drawRect(Vec2(weaponRectangle[0].x, weaponRectangle[0].y), Vec2(weaponRectangle[1].x, weaponRectangle[1].y), Vec2(weaponRectangle[3].x, weaponRectangle[3].y), Vec2(weaponRectangle[2].x, weaponRectangle[2].y), ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));
	view->weaponBox->drawRect(Vec2(weaponDetectionRectangle[0].x, weaponDetectionRectangle[0].y), Vec2(weaponDetectionRectangle[1].x, weaponDetectionRectangle[1].y), Vec2(weaponDetectionRectangle[3].x, weaponDetectionRectangle[3].y), Vec2(weaponDetectionRectangle[2].x, weaponDetectionRectangle[2].y), ccColor4F(1.0f, 0.0f, 0.0f, 1.0f));
}

bool GameController::isZombieHit(b2Vec2 az, b2Vec2 bz, b2Vec2 ab, b2Vec2 bc){
	//0 <= dot(AB, AM) <= dot(AB, AB) &&
	//	0 <= dot(BC, BM) <= dot(BC, BC)
	if ((0 <= b2Dot(ab, az) && b2Dot(ab, az) <= b2Dot(ab, ab)) && (0 <= b2Dot(bc, bz) && b2Dot(bc, bz) <= b2Dot(bc, bc))){
		return true;
	}
	else return false;
}


void GameController::removeCollectedGoals(){
	GoalObject *go = state->instrument;
	if (go != NULL && go->isCollected){
		go->isCollected = false;
		state->world->DestroyBody(go->body);
		view->enviornment->removeChild(go->sprite);
		//delete(go);
	}
}


void GameController::removeDeadWeapons(){

	CTypedPtrDblElement<Weapon> *toDelete = NULL;
	for (CTypedPtrDblElement<Weapon> *weapon = state->weapons.GetHeadPtr(); !state->weapons.IsSentinel(weapon); weapon = weapon->Next())
	{
		Weapon *weap = weapon->Data();
		if (weap->isDesroyed){
			weap->isDesroyed = false;
			state->world->DestroyBody(weap->body);
			toDelete = weapon;
			view->enviornment->removeChild(weap->sprite);
		}
	}
	if (toDelete != NULL){
		state->weapons.Remove(toDelete);
	}
}

void GameController::removeDeadEWeapons(){

	CTypedPtrDblElement<EnvironmentWeapon> *toDelete = NULL;
	EnvironmentWeapon *eweap = NULL;
	for (CTypedPtrDblElement<EnvironmentWeapon> *e_weapon = state->environment_weapons.GetHeadPtr(); !state->environment_weapons.IsSentinel(e_weapon); e_weapon = e_weapon->Next())
	{
		eweap = e_weapon->Data();
		if ((eweap->e_weapon_type == 0) && (eweap->isJammed)){
			eweap->onCooldown = true;
			eweap->isUsed = false;
			eweap->isJammed = false;
		}
		if ((eweap->e_weapon_type == 1) && (eweap->isUsed)){
			eweap->onCooldown = true;
			eweap->update();
			eweap->isUsed = false;
		}
		if ((eweap->e_weapon_type != 1) && (eweap->isUsed || eweap->hitWall)){
			eweap->isUsed = false;
			if (eweap->e_weapon_type == 2){
				audio->playEffect("sound_effects/BottleSmash.mp3",2.0f);
				((Trash *)eweap)->addParticles();
			}
			state->world->DestroyBody(eweap->body);
			toDelete = e_weapon;
			view->enviornment->removeChild(eweap->sprite);
		}
	}
	if (toDelete != NULL){
		//delete toDelete->Data()
		state->environment_weapons.Remove(toDelete);
	}
}

void GameController::removeDeadZombies(){ 
	CTypedPtrDblList<CTypedPtrDblElement<Zombie>> zombsToDel;
	for (CTypedPtrDblElement<Zombie> *zombie = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(zombie); zombie = zombie->Next())
	{
		Zombie *zomb = zombie->Data();
		if (zomb->isDestroyed){
			zomb->body->GetWorld()->DestroyBody(zomb->body);
			zomb->isDestroyed = false;
			zomb->addParticles();
			zomb->sprite->setColor(Color3B(128, 128, 128));
			zombsToDel.AddTail(zombie);
		}

	}

	for (CTypedPtrDblElement<CTypedPtrDblElement<Zombie>> *z = zombsToDel.GetHeadPtr(); !zombsToDel.IsSentinel(z);  z = z->Next()){
		state->dyingZombies.AddTail(z->Data()->Data());
		state->zombies.Remove(z->Data());
	}
}

void GameController::removeDyingZombies(){
	CTypedPtrDblList<CTypedPtrDblElement<Zombie>> zombsToDel;
	for (CTypedPtrDblElement<Zombie> *zombie = state->dyingZombies.GetHeadPtr(); !state->dyingZombies.IsSentinel(zombie); zombie = zombie->Next())
	{
		Zombie *z = zombie->Data();
		if (!z->emitter->isActive()){
			view->zombies->removeChild(z->sprite);
			zombsToDel.AddTail(zombie);
		}
	}
	for (CTypedPtrDblElement<CTypedPtrDblElement<Zombie>> *z = zombsToDel.GetHeadPtr(); !zombsToDel.IsSentinel(z); z = z->Next()){
		delete z->Data()->Data();
		state->dyingZombies.Remove(z->Data());

	}
}

#define VIDEO_CALIBRATION_OFFSET 150
void GameController::startVideoCalibration(){
	//view->objective->setString("Now tap just as the center of each zombie lines up\nwith Ricky, don't miss any!");
	calibration->totalOffset = 0.0;
	calibration->clicks = 0;
	state->ship->body->SetLinearVelocity(b2Vec2_zero);
	Vec2 anchor = Vec2(0.5f, 0.5f);
	b2Vec2 p = state->ship->body->GetPosition();
	Sprite *hat = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("triangle1"));
	hat->setAnchorPoint(anchor);
	hat->setScale(0.6f);
	hat->setPosition(state->ship->sprite->getContentSize().width/2, state->ship->sprite->getContentSize().height/2+ 50);
	state->ship->sprite->addChild(hat);
	calibration->audioCalibration = false;
	view->zombies->removeAllChildren();
	view->zombies->setPosition(Vec2::ZERO);
	for (int i = 0; i < VIDEO_BEATS; i++){
		calibration->zombies[i] = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("triangle2"));
		calibration->zombies[i]->setAnchorPoint(anchor);
		calibration->zombies[i]->setScale(0.8f);
		calibration->zombies[i]->setPosition(p.x + VIDEO_CALIBRATION_OFFSET*i + 2 * VIDEO_CALIBRATION_OFFSET, p.y + 165);
		calibration->zombieTimes[i] = elapsedTime * 2 + 1000000;
		view->zombies->addChild(calibration->zombies[i], 10);
	}
}

/**
* Update the game state.
*
* This method is called every animation frame.  There is no draw() or render()
* counterpoint to this method; drawing is done automatically in the scene graph.
* However, this method is responsible for updating any transforms in the scene graph.
*/
void GameController::update(float deltaTime) {
	if (!audio->paused){
		elapsedTime += deltaTime;
		audio->setFrameOnBeat(deltaTime);
	}
	if (!beginCountDown) {
		if (!isPaused) {
			if (hasWonLevel()){
				if (currentLevel == CALIBRATION_LEVEL){
					if (calibration->audioCalibration){
						if (calibration->clicks < AUDIO_BEATS){
							restartGame();
						}
						else{
							view->objective->setString("Now tap just as the center of each orange triangle\nlines up with the blue triangle, don't miss any!\nClick ok to continue");
							createOkButton(1);
							pauseGameOnly();
							startVideoCalibration();
						}
					}
					else{
						if (calibration->clicks < VIDEO_BEATS){
							view->objective->setString("Now tap just as the center of each orange triangle\nlines up with the blue triangle, don't miss any!\nClick ok to continue");
							createOkButton(1);
							pauseGameOnly();
							startVideoCalibration();
						}
						else{
							//indicate that we have completed the level so far
							save.exportLevel(currentLevel);
							loadLevel(std::min(currentLevel + 1, MAX_LEVELS));
							beginCountDown = true;
							createCountDown();
							return;
						}
					}
				}
				else{
					//indicate that we have completed the level so far
					save.exportLevel(currentLevel);
					loadLevel(std::min(currentLevel + 1, MAX_LEVELS));
					beginCountDown = true;
					createCountDown();
					return;
				}
			}
			removeDeadWeapons();
			removeDeadEWeapons();
			removeDeadZombies();
			removeDyingZombies();
			removeCollectedGoals();
			meter->drain();


			//reduce meter by a little bit
			if (state->ship->isActivatingEnvironment && currentLevel != CALIBRATION_LEVEL){
				currentEnvironmentMeter -= ENVIRONMENT_METER_DEC;
			}


			EnvironmentWeapon *eweap = NULL;
			for (CTypedPtrDblElement<EnvironmentWeapon> *e_weapon = state->environment_weapons.GetHeadPtr(); !state->environment_weapons.IsSentinel(e_weapon); e_weapon = e_weapon->Next())
			{
				eweap = e_weapon->Data();
				if (eweap->onCooldown){
					eweap->cdTimer += deltaTime;
				}
				if (eweap->cdTimer >= TRASHCAN_CD){
					eweap->onCooldown = false;
					eweap->cdTimer = 0.0f;
					eweap->update();
				}
				if (!(currentEnvironment != NULL && currentEnvironment->e_weapon_type == 1)){
					if (removed == true) {
						removed = false;
						currentEnvironment->beingUsed = false;
						view->allSpace->addChild(state->ship->getSprite());
					}
				}
				else {
					if (eweap->beingUsed == true) {
						eweap->update2(); //update frame of trashcan when ricky's hiding
					}
					view->allSpace->removeChild(state->ship->getSprite());
					removed = true;
				}

			}
		
			if (currentEnvironment != NULL && currentEnvironment->e_weapon_type == 1  && !state->ship->hasEnvironmentWeapon && doneActivating){
				//eweap->update();
				eweap->sprite->setFrame(0);
				if (!eweap->onCooldown){
					eweap->sprite->setFrame(1);
				}
				view->allSpace->addChild(state->ship->getSprite());
				eweap->beingUsed = false;
				removed = false;
			}

			if (state->ship->isDestroyed){
				restartGame();
				return;
			}
			if (currentLevel != CALIBRATION_LEVEL) {
				updateFog();
			}
			float x = state->ship->body->GetPosition().x;
			float y = state->ship->body->GetPosition().y;
			MapNode *from = state->level->locateCharacter(x, y);
			//if (!input->clickProcessed && !activated)
			if (!input->clickProcessed){
				if (currentLevel != CALIBRATION_LEVEL || (calibration->audioCalibration && calibration->acceptClicks)){
					//cout << "PBF: " << elapsedTime << "\n";
					stringstream st;
					//char* tempstr = "kgkgk";
					//std::sprintf(tempstr, "%f", elapsedTime);

					//view->beatHUD->setString("Actual time: " + std::to_string(elapsedTime) + " song time: " + std::to_string(AudioEngine::getCurrentTime(audioid)));
					time_t now = time(0);
					int userOnBeat = audio->wasOnBeat((now - input->clickTime)/1000.0f);
					if (userOnBeat == 0 && currentLevel != CALIBRATION_LEVEL) {
						b2Vec2 halfVel = state->ship->body->GetLinearVelocity();
						halfVel *= 0.25;
						state->ship->body->SetLinearVelocity(halfVel);
						//destination = 0;

						//if it is not on beat, increase the detection radius slightly
						meter->increaseRadius();

						//TODO: Decrease meter gradually and when missed
						if (state->ship->isActivatingEnvironment){
							currentEnvironmentMeter -= ENVIRONMENT_METER_DEC_OFFBEAT;
						}
					}
					//know we must be on beat 
					//check if activating environment
					else if (state->ship->isActivatingEnvironment && userOnBeat != 0 && currentLevel != CALIBRATION_LEVEL){
						if (currentEnvironment != NULL) {
							currentEnvironment->beingUsed = true;
							//set the trashcan to ricky hiding
							if (currentEnvironment->e_weapon_type == 1) {
								currentEnvironment->sprite->setFrame(2); 
							}
						}

						meter->decreaseRadius();

						if (!activationDelay){
							currentEnvironmentMeter += ENVIRONMENT_METER_INC;
						}
						activationDelay = false;
					
						//since this is percentage based, snap it between 0-100 for drawing purposes
						if (currentEnvironmentMeter < 0.0f){
							currentEnvironmentMeter = 0.0f;
						}
						if (currentEnvironmentMeter > 1.0f){
							currentEnvironmentMeter = 1.0f;
						}
						/*
					
					
						TODO:
						-Display meter full before it gets released
						-make meter gradually drain and drain when off beat and make sure cooldown works
						-make meter different colors to match UI
					
					
						*/

						processDirection = true;// signals to draw meter
						currentFingerPos = Vec2((input->lastClick.x - view->screen_size_x / 2.0) + state->ship->body->GetPosition().x, -1.0f*(input->lastClick.y - view->screen_size_y / 2.0) + state->ship->body->GetPosition().y);
						currentFingerPos.subtract(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
						currentFingerPos.normalize();
						currentFingerPos.scale(TDISTANCE);
						currentFingerPos.add(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));

						//check activation status
						if (currentEnvironmentMeter == 1.0f){
							//successfully activated
							activationDelay = true;
							Vec2 clk = mouseToWorld(input->lastClick);
							dRickyTap->set(clk.x - state->ship->body->GetPosition().x, clk.y - state->ship->body->GetPosition().y);
							float tht = atan2(dRickyTap->y, dRickyTap->x);
							//tht = round(tht / (M_PI / 4.0f)) * (M_PI / 4.0f);

							createEnvironmentActivationMeter(ENVIRONMENT_METER_WIDTH, ENVIRONMENT_METER_LENGTH, 1.0f, b2Vec2(cos(tht), sin(tht)));

							state->ship->isActivatingEnvironment = false;
							processDirection = false;
							state->ship->hasEnvironmentWeapon = false;

							currentEnvironment->isUsed = true;

							Vec2 mowerDir = currentFingerPos;
							mowerDir.subtract(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
							mowerDir.normalize();
							mowerDir.scale(125.0f);
							mowerDir.add(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));

							b2Vec2 dir = b2Vec2(mowerDir.x, mowerDir.y);
							view->weaponBox->clear();
							if (currentEnvironment->e_weapon_type == 0){
								Lawnmower *lm = new Lawnmower(state->world, mowerDir.x, mowerDir.y, dir);
								state->environment_weapons.AddTail(lm);
								view->enviornment->addChild(lm->sprite, 2); //lawnmower's z-order needs to be changed the same as envrionment weapon 
								audio->playEffect("sound_effects/LawnMower.mp3", 1.2f);
								lm->addParticles();
								currentMower = lm;
								currentEnvironment = lm;
							}
							else if (currentEnvironment->e_weapon_type == 1){
								currentEnvironment->beingUsed = false; //not using trashcan anymore
								Trash *tr = new Trash(state->world, mowerDir.x, mowerDir.y);
								state->environment_weapons.AddTail(tr);
								view->enviornment->addChild(tr->sprite, 2);
								currentTrash = tr;
								currentEnvironment = tr;
							}

						}
					}	
					else{
						if (userOnBeat == 2 && currentLevel != CALIBRATION_LEVEL){
							state->ship->boostFrames = MAX_EIGHTH_NOTE_FRAMES;
							state->ship->thrustFactor = EIGHTH_NOTE_THRUST_FACTOR;
							state->ship->body->SetLinearDamping(EIGHTH_NOTE_DAMPENING);
							state->ship->addDustParticles();
						}
						else{
							state->ship->boostFrames = MAX_BOOST_FRAMES;
							state->ship->thrustFactor = 1.0f;
							state->ship->body->SetLinearDamping(NORMAL_DAMPENING);
						}
						view->weaponBox->clear();
						Vec2 click = mouseToWorld(input->lastClick);
						MapNode *dest = state->level->locateCharacter(click.x, click.y);
						state->level->shortestPath(from, dest);
						destination = from->next;
						MapNode *destPrev = from;

						//if it is on beat, decrease the detection radius slightly
						meter->decreaseRadius();

						if (state->ship->hasWeapon){
							//if you have a weapon and click on the beat, check if you kill any zombies in that direction
							//mouseclick to world coords subtract rickies pos from that...normalize it...snap it to nearest rotation: round nearest int(arctan2(normalized) /. pi/2) * pi/4

							dRickyTap->set(click.x - state->ship->body->GetPosition().x, click.y - state->ship->body->GetPosition().y);
							float theta = atan2(dRickyTap->y, dRickyTap->x);
							theta = round(theta / (M_PI / 4.0f)) * (M_PI / 4.0f);

							createWeaponRanges(state->ship->currentWeapon->width, state->ship->currentWeapon->range, state->ship->currentWeapon->detectionRange, b2Vec2(cos(theta), sin(theta)));
							//detect if zombies are inside rectangle of weapon
							CTypedPtrDblElement<Zombie> *zambie = state->zombies.GetHeadPtr();
							int num_zombies_killed = 0;
							while (!state->zombies.IsSentinel(zambie))
							{
								Zombie *zombb = zambie->Data();
								if (!zombb->isDestroyed){
									//AB is vector AB, with coordinates (Bx-Ax,By-Ay)
									b2Vec2 az = b2Vec2(zombb->body->GetPosition().x - weaponRectangle[0].x, zombb->body->GetPosition().y - weaponRectangle[0].y);
									b2Vec2 bz = b2Vec2(zombb->body->GetPosition().x - weaponRectangle[1].x, zombb->body->GetPosition().y - weaponRectangle[1].y);
									b2Vec2 ab = b2Vec2(weaponRectangle[1].x - weaponRectangle[0].x, weaponRectangle[1].y - weaponRectangle[0].y);
									b2Vec2 bc = b2Vec2(weaponRectangle[3].x - weaponRectangle[1].x, weaponRectangle[3].y - weaponRectangle[1].y);

									b2Vec2 az2 = b2Vec2(zombb->body->GetPosition().x - weaponDetectionRectangle[0].x, zombb->body->GetPosition().y - weaponDetectionRectangle[0].y);
									b2Vec2 bz2 = b2Vec2(zombb->body->GetPosition().x - weaponDetectionRectangle[1].x, zombb->body->GetPosition().y - weaponDetectionRectangle[1].y);
									b2Vec2 ab2 = b2Vec2(weaponDetectionRectangle[1].x - weaponDetectionRectangle[0].x, weaponDetectionRectangle[1].y - weaponDetectionRectangle[0].y);
									b2Vec2 bc2 = b2Vec2(weaponDetectionRectangle[3].x - weaponDetectionRectangle[1].x, weaponDetectionRectangle[3].y - weaponDetectionRectangle[1].y);

									if (isZombieHit(az2, bz2, ab2, bc2)){
										if (isZombieHit(az, bz, ab, bc)){
											num_zombies_killed += 1;
											state->numZombiesRemain -= 1;
											if (state->numZombiesRemain < 0){
												state->numZombiesRemain = 0;
											}
											//zombie got hit so delete it
											zombb->isDestroyed = true;
										}
									}
								}
								zambie = zambie->Next();
							}
							if (num_zombies_killed > 0){
								audio->playEffect("sound_effects/shotgun.mp3", 0.4f);
								state->ship->currentWeapon->durability -= 1;
								view->redrawDurability(state->ship->currentWeapon->durability);
								state->ship->addShrapnelParticles(theta * 180 / M_PI);

							}
							if (state->ship->currentWeapon->durability == 0){
								state->ship->hasWeapon = false;
								free(state->ship->currentWeapon);
								state->ship->currentWeapon = NULL;
								view->weaponBox->clear();
							}

						}

					}
					if (currentLevel == CALIBRATION_LEVEL){
						if (calibration->acceptClicks){
							calibration->totalOffset += audio->timeToBeat(4 + calibration->clicks);
							st << "OFFSET: " << formatMs(audio->timeToBeat(4 + calibration->clicks));
							calibration->clicks++;
							view->beatHUD->setString(st.str());
							if (calibration->clicks >= AUDIO_BEATS) calibration->acceptClicks = false;
						}
					}
					else{
						if (userOnBeat){
							st << "HIT";
						}
						else{
							st << "MISS BY: " << formatMs(audio->timeToClosestBeat());
						}
						view->beatHUD->setString(st.str());
					}
					input->clickProcessed = true;
					//check if there is a zombie within the radius
					CTypedPtrDblElement<Zombie> *cur = state->zombies.GetHeadPtr();
					Zombie *curZ;
					b2Vec2 tmp;
					int count = 0;
					bool hiss = false;
					while (!state->zombies.IsSentinel(cur)){
						curZ = cur->Data();
						tmp = state->ship->body->GetPosition() - curZ->body->GetPosition();
						//if this zombie is within our detection radius and we messed up the beat
						float dis;
						dis = sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
						if (!userOnBeat && dis < meter->detectionRadius) {
							hiss = true;
							curZ->increaseAwarness();
						}
						if (count == 0) {
							currAwareness = curZ->awareness[1];
						}

						count++;
						cur = cur->Next();
					}
					if (hiss){
						audio->playEffect("sound_effects/ZombieHiss.mp3", 0.5f);
					}


				}
				else{
					if (calibration->acceptClicks){
						time_t now = time(0);
						float offset = (((elapsedTime - (now - input->clickTime) / 1000.0f) - audio->audioDelay) - calibration->zombieTimes[calibration->clicks]);
						calibration->zombieTimes[calibration->clicks] = elapsedTime;
						calibration->totalOffset += offset;
						stringstream vss;
						vss << "OFFSET: " << formatMs(offset);
						view->beatHUD->setString(vss.str());
						view->zombies->removeChild(calibration->zombies[calibration->clicks]);
						calibration->clicks++;
						input->clickProcessed = true;
						if (calibration->clicks == VIDEO_BEATS){
							calibration->acceptClicks = false;
							audio->videoDelay = calibration->videoDelay();
							stringstream ss;
							ss << "Ok, great.  Video Delay: " << formatMs(audio->videoDelay); //finishing up calibration
							view->objective->setString(ss.str());
							save.exportSave(this);
							//save.exportLevel(0);
						}
					}
				}
			}

			if (destination != 0 && from == destination){
				destination = from->next;
			}
			if (currentEnvironmentMeter < 0.0f){
				currentEnvironmentMeter = 0.0f;
			}
			if (currentEnvironmentMeter > 1.0f){
				currentEnvironmentMeter = 1.0f;
			}

			if (state->ship->isActivatingEnvironment && currentEnvironmentMeter >= 0.0f && currentLevel != CALIBRATION_LEVEL){
				Vec2 clk = mouseToWorld(input->lastClick);
				dRickyTap->set(clk.x - state->ship->body->GetPosition().x, clk.y - state->ship->body->GetPosition().y);
				float tht = atan2(dRickyTap->y, dRickyTap->x);

				createEnvironmentActivationMeter(ENVIRONMENT_METER_WIDTH, ENVIRONMENT_METER_LENGTH, currentEnvironmentMeter, b2Vec2(cos(tht), sin(tht)));

				//createEnvironmentActivationMeter(ENVIRONMENT_METER_WIDTH, ENVIRONMENT_METER_LENGTH, currentEnvironmentMeter, b2Vec2(cos(ang), sin(ang)));
			}

			if (currentEnvironmentMeter <= 0.0f && currentLevel != CALIBRATION_LEVEL){
				activationDelay = true;
				doneActivating = true;
				if (currentEnvironment != NULL) {
					currentEnvironment->beingUsed = false;
				}
				view->weaponBox->clear();
				//failed to activate
				state->ship->isActivatingEnvironment = false;
				processDirection = false;
				state->ship->hasEnvironmentWeapon = false;
				//place the specific weapon on cd
				if (currentEnvironment->e_weapon_type == 0){
					currentEnvironment->isJammed = true;
					view->enviornment->addChild(currentEnvironment->sprite);
				}
				else if(currentEnvironment->e_weapon_type == 1){
					currentEnvironment->isUsed = true;
				}
			
				currentEnvironmentMeter = 0.3f;

			}

			if (currentEnvironmentMeter >= 1.0f && currentLevel != CALIBRATION_LEVEL){
				currentEnvironmentMeter = 0.3f;
				Vec2 clk = mouseToWorld(input->lastClick);
				dRickyTap->set(clk.x - state->ship->body->GetPosition().x, clk.y - state->ship->body->GetPosition().y);
				float tht = atan2(dRickyTap->y, dRickyTap->x);
				//tht = round(tht / (M_PI / 4.0f)) * (M_PI / 4.0f);

				createEnvironmentActivationMeter(ENVIRONMENT_METER_WIDTH, ENVIRONMENT_METER_LENGTH, 1.0f, b2Vec2(cos(tht), sin(tht)));
			}

			if (destination != 0 && from == destination){
				destination = from->next;
			}

			if (destination != 0){
				Vec2 dir = Vec2(state->level->getTileCenterX(destination) - x, state->level->getTileCenterY(destination) - y);
				dir.normalize();
				if (!state->ship->update(deltaTime, dir, meter->getGrooviness())){
					destination = 0;
				}
			}
			if (currentLevel != CALIBRATION_LEVEL){
				ai->update(state);
			}
			else{
				//calibration->phaseDelay += deltaTime;
				if (!calibration->audioCalibration){
					view->zombies->setPosition(view->zombies->getPosition() + Vec2(-2, 0));
					for (int i = calibration->clicks; i < VIDEO_BEATS; i++){
						if (calibration->zombieTimes[i]>elapsedTime){
							calibration->zombieTimes[i] = elapsedTime + (VIDEO_CALIBRATION_OFFSET*i + 2 * VIDEO_CALIBRATION_OFFSET + view->zombies->getPosition().x) / 2 * deltaTime;
						}
					}
					if (elapsedTime - calibration->zombieTimes[calibration->clicks] > 1.0f){
						calibration->acceptClicks = false;
						if (calibration->clicks != VIDEO_BEATS) {
							view->objective->setString("You missed some of the triangles, please try again.");
						}
					}
				}
				else{
					if (audio->songTime > 11.1f || calibration->clicks == AUDIO_BEATS
						|| (audio->songTime>3.5f && calibration->clicks < 1)
						|| (calibration->totalOffset < -0.5f)){
						audio->stop();
						//calibration->phaseDelay = 0.0f;ph
						stringstream ss;
						if (calibration->clicks < AUDIO_BEATS){
							if (calibration->clicks > 0 && calibration->totalOffset < -0.5f){
								ss << "Just listen to the first four beats,\nthen tap to the next 16!";
							}
							else{
								ss << "You missed some of the the beats, try again.";
							}
						}
						else{
							audio->audioDelay = calibration->audioDelay();
							ss << "Ok, great! Now stop tapping! Audio delay: " << formatMs(audio->audioDelay);
							calibration->acceptClicks = false;
						}
						view->objective->setString(ss.str());
					}
					if (audio->songIsOver()){
						calibration->phaseDelay += deltaTime;
					}
				}
			}


			//activated = false;

			if (state->ship->isActivatingEnvironment){
				state->ship->body->SetLinearVelocity(b2Vec2_zero);
				destination = 0;
			}

			if (state->ship->hasEnvironmentWeapon && currentEnvironment != NULL && currentEnvironment->e_weapon_type != 1 && !currentEnvironment->hasMoved && currentEnvironment->sprite != NULL){
				view->enviornment->removeChild(currentEnvironment->sprite);
			}

			if (currentMower != NULL && currentMower->hasMoved){
				Vec2 mowerDir = currentFingerPos;
				mowerDir.subtract(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
				mowerDir.normalize();

				currentMower->update(deltaTime, mowerDir);
			}

			if (currentTrash != NULL && currentTrash->hasMoved){
				Vec2 mowerDir = currentFingerPos;
				mowerDir.subtract(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
				mowerDir.normalize();

				currentTrash->update(deltaTime, mowerDir);
			}

			// Move the ship (MODEL ONLY)
			//shipModel->setForward(thrust.y);
			//shipModel->setTurning(thrust.x);
			//shipModel->update(deltaTime, thrust);
			state->world->Step(deltaTime, 8, 3);
			//world->ClearForces();


			// "Drawing" code.  Move everything BUT the ship
			// Update the HUD
			displayPosition(view->coordHUD, state->ship->body->GetPosition());

			b2Vec2 pos = state->ship->body->GetPosition();
			Vec2 offset = Vec2(pos.x, pos.y);// -enviornment->getPosition();
			Vec2 center(0.5f, 0.5f);

			// Anchor points are in texture coordinates (0 to 1). Scale it.
			offset.x = offset.x / view->allSpace->getContentSize().width;
			offset.y = offset.y / view->allSpace->getContentSize().height;

			// Reanchor the node at the center of the screen and rotate about center.
			//farSpace->setAnchorPoint(offset*PARALLAX_AMT+center);
			//farSpace->setRotation(-shipModel->body->GetAngle());
			//shipImage->setAnchorPoint(-offset);
			view->enviornment->setAnchorPoint(offset);

			// Reanchor the node at the center of the screen and rotate about center.
			//nearSpace->setAnchorPoint(offset+center);
			//nearSpace->setRotation(-shipModel->body->GetAngle());
			//state->ship->getSprite()->setRotation(state->ship->body->GetAngle());
			CTypedPtrDblElement<Zombie> *z = state->zombies.GetHeadPtr();
			while (!state->zombies.IsSentinel(z)){
				pos = z->Data()->body->GetPosition();
				if ((z->Data()->body->GetPosition() - state->ship->body->GetPosition()).Length() <= meter->detectionRadius + 40){
					//z->Data()->sprite->setVisible(true);
					z->Data()->sprite->setOpacity(255);
					z->Data()->sprite->setPosition(pos.x, pos.y);
					z->Data()->advanceFrame();
				}
				else{
					//z->Data()->sprite->setVisible(true);
					z->Data()->sprite->setOpacity(200);
					z->Data()->sprite->setPosition(pos.x, pos.y);
					z->Data()->advanceFrame();
					/*
					z->Data()->sprite->setVisible(false);
					if (!audio->frameOnBeat){
					view->zombiePositions->clear();
					}else if (!audio->prevOnBeat){
					pos = z->Data()->body->GetPosition();
					view->zombiePositions->drawSolidCircle(Vec2(pos.x, pos.y), 8.0f, 0.0f, 20.0f, ccColor4F(0.5f, 0, 0, 1.0f));
					}*/

				}
				z = z->Next();
			}

			CTypedPtrDblElement<EnvironmentWeapon> *envwe = state->environment_weapons.GetHeadPtr();
			while (!state->environment_weapons.IsSentinel(envwe)){
				pos = envwe->Data()->body->GetPosition();
				envwe->Data()->sprite->setPosition(pos.x, pos.y);
				if (envwe->Data()->e_weapon_type == 2){
					float rot = envwe->Data()->sprite->getRotation();
					envwe->Data()->sprite->setRotation(rot + 15);
				}
				envwe = envwe->Next();
			}
		}

		//game is paused, check if they click on the game menu buttons
		else {
			//do nothing, might gonna need it to do something later
		}
	}
	//begin count down
	else {
		countDownCounter += 1;
		if (countDownCounter % 50 == 0) {
			int currentFrame = countDown->getFrame();
			currentFrame++;
			if (currentFrame == 3) {
				countDownCounter = 0;
				beginCountDown = false;
				audio->paused = false;
				isPaused = false;
				view->resIndepScreen->removeChild(countDown);
				unPanning();
				//now we can start playing songs
				audio->playTrack(ls.getLevelTrack(), currentLevel != CALIBRATION_LEVEL);
				return;
			}
			countDown->setFrame(currentFrame);
		}
	}
}


void GameController::drawMusicNotePath() {
	if (destination != 0){
		MapNode* cur = destination;
		for (int i = 0; i < MAX_MUSIC_NOTES; i++){
			if (cur != NULL){
				musicNotes[i]->setPosition(state->level->getTileCenterX(cur), state->level->getTileCenterY(cur));
				musicNotes[i]->setVisible(true);
				cur = cur->next;
			}
			else{
				musicNotes[i]->setVisible(false);
			}
		}
		/*do {
		float x1 = state->level->getTileCenterX(last);
		float y1 = state->level->getTileCenterY(last);
		float x2 = state->level->getTileCenterX(cur);
		float y2 = state->level->getTileCenterY(cur);
		//draw main character's path
		//view->path->drawLine(Vec2(x1, y1), Vec2(x2, y2), ccColor4F(1.0f, 1.0f, 1.0f, 1.0f));
		drawMusicNotePath(Vec2(x1, y1));
		drawMusicNotePath(Vec2(x2, y2));
		last = cur;
		cur = cur->next;
		} while (cur != 0);*/
	}
	else{
		for (int i = 0; i < MAX_MUSIC_NOTES; i++){
			musicNotes[i]->setVisible(false);
		}
	}
}

/**
* Writes the current ship position to the HUD.
*
* @param label The HUD label
* #param coords The current ship coordinates
*/
void GameController::displayPosition(Label* label, const b2Vec2& coords) {
	/*stringstream ss;
	ss << "Coords: (" << (int)coords.x / 10 << "," << (int)coords.y / 10 << ")";
	view->coordHUD->setString(ss.str());
	stringstream s;
	s << "Speed: " << state->ship->body->GetLinearVelocity().Length();
	view->velHUD->setString(s.str());
	stringstream sss;
	sss << "Click: (" << input->lastClick.x << "," << input->lastClick.y << ")";
	view->thrustHUD->setString(sss.str());*/
	//view->path->clear();

	//clear the old music notes path
	view->directionUseEnvironmentWeapon->clear();
	drawMusicNotePath();
	//clear the old detection circle
	//view->detectionRadiusCircle->clear();
	//clear the old hitbox
	//view->hitBox->clear();
	//view->weaponBox->clear();

	/*stringstream d;
	//d << "Detection Radius: " << detectionRadius;
	d << "Ricky Pos: " << state->ship->body->GetPosition().x << "," << state->ship->body->GetPosition().y;
	view->detectionRadiusHUD->setString(d.str());
	stringstream awr;
	//awr << "Zombie 1 Awarness: " << currAwareness;
	awr << "endpt: " << currentFingerPos.x << "," << currentFingerPos.y;
	view->zombieOneAwarenessHUD->setString(awr.str());*/

	//visualize the detection radius
	//view->detectionRadiusCircle->drawCircle(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), meter->detectionRadius, 0.0f, 1000, false, ccColor4F(0, 0, 2.0f, 1.0f));

	//visualize the hitbox for main character
	//view->hitBox->drawRect(Vec2(state->ship->body->GetPosition().x - 29.5f, state->ship->body->GetPosition().y - 55.0f), Vec2(state->ship->body->GetPosition().x + 29.5f, state->ship->body->GetPosition().y + 55.0f), ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));

	//visualize the arrow for activating the environment
	//if (processDirection){
	//	view->directionUseEnvironmentWeapon->drawLine(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), Vec2(currentFingerPos.x, currentFingerPos.y), ccColor4F(0.0f, 0.0f, 0.0f, 0.8f));
	//}

	if (state->ship->hasWeapon){

		//hero array for sprite pointers
		//pickup will clear contents and then for loop that populates it with sprite pointers and sets position by index in the actual sprite position
		/*int dura = state->ship->currentWeapon->durability;
		for (int i = 0; i < 3; ++i){
		for (int j = 0; j < 2; ++j){
		if (dura < 1){
		break;
		}
		//create the new sprite
		view->durabilitySpriteContainer->addChild(state->ship->weaponDurabilityDisplay[i][j]);
		dura--;
		}
		}*/
	}

	if (!state->ship->hasWeapon){
		view->durabilitySpriteContainer->clear();
		//view->weaponBox->drawRect(Vec2(weaponRectangle[0].x, weaponRectangle[0].y), Vec2(weaponRectangle[1].x, weaponRectangle[1].y), Vec2(weaponRectangle[3].x, weaponRectangle[3].y), Vec2(weaponRectangle[2].x, weaponRectangle[2].y), ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));
	}
	//view->directionUseEnvironmentWeapon->drawLine(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), currentFingerPos, ccColor4F(128.0f, 128.0f, 128.0f, 0.5f));
	view->ai->clear();
	for (CTypedPtrDblElement<Zombie> *z = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(z); z = z->Next()){
	Zombie *zom = z->Data();
	b2Vec2 pos = zom->body->GetPosition();
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->seperation.x, pos.y + zom->seperation.y), ccColor4F(1, 0, 0, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->attraction.x, pos.y + zom->attraction.y), ccColor4F(0, 1, 0, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->aidir.x / 28, pos.y + zom->aidir.y / 28), ccColor4F(0, 0, 0, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->alignment.x, pos.y + zom->alignment.y), ccColor4F(1, 0, 1, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->cohesion.x, pos.y + zom->cohesion.y), ccColor4F(0, 0, 1, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->zombiness.x, pos.y + zom->zombiness.y), ccColor4F(1, 1, 0, 1.0f));
	}

	if (currentLevel != CALIBRATION_LEVEL){
		stringstream st;
		if (audio->frameOnBeat){//AudioEngine::getCurrentTime(audioid))){
			view->mainBeatHUD->setString("BEAT!");
			st << "Difference is: " << audio->keepit;
			//view->beatHUD->setString(st.str());
			view->shake(audio->getBeatStart(), audio->songTime, Vec2(1, 0));
		}
		else{
			view->mainBeatHUD->setString("");
		}
		float g = meter->getGrooviness();
		view->grooviness->setString(meter->getGroovinessDisplay(g));
		view->meter->clear();
		view->meter->drawSolidRect(Vec2(-15, 0), Vec2(+15, g * 180), ccColor4F(0.0f, 1.0f, 1.0f, 1.0f));
		view->meter->drawRect(Vec2(-15, 0), Vec2(+15, 180), ccColor4F(0.5f, 0.5f, 0.5f, 1.0f));
		stringstream ss;
		ss << state->numZombiesRemain << " zombies left!";
		view->objective->setString(ss.str());
		
		if (state->instrument != NULL){
			if (!hasCollectedGoal){
				view->collectionGoal->setString("You must collect the instrument");
			}
			else{
				view->collectionGoal->setString("You have found the instrument!");
			}
		}
		else{
			view->collectionGoal->setVisible(false);
		}
	}

	/*if (!input->clickProcessed) {
	mouse->drawDot(input->lastClick, 2.0f, ccColor4F(0.0f, 0.0f, 0.0f, 0.0f));
	input->clickProcessed = true;
	}*/
	/*stringstream st;
	if (onBeat){
	st << "HIT";
	}
	else{
	st << "MISS";
	}
	view->beatHUD->setString(st.str());*/
	//TODO: Add new HUD for BPM
}

