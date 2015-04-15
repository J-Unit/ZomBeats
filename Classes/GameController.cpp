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
#include "Zombie.h"
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
#define MAX_LEVELS 2

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

		/*
		//reset weapon sprites durability for new weapon in Ricky
		int durabil = state->ship->currentWeapon->durability;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 2; ++j){
				if (durabil < 1){
					break;
				}
				//create the new sprite
				state->ship->weaponDurabilityDisplay[i][j] = state->ship->currentWeapon->sprite;
				state->ship->weaponDurabilityDisplay[i][j]->setAnchorPoint(Vec2(0.0f, 0.0f));
				state->ship->weaponDurabilityDisplay[i][j]->setPosition(j*20.0f, i*20.0f);
				view->durabilitySpriteContainer->addChild(state->ship->weaponDurabilityDisplay[i][j]);
				durabil--;
			}
		}
		*/
		return;
	}

	//trigger environment weapon activation
	if ((b1->type == EnvironmentWeaponType && b2->type == ShipType) || (b1->type == ShipType && b2->type == EnvironmentWeaponType)){
		currentEnvironment = (b1->type == EnvironmentWeaponType) ? b1->getEnvironmentWeapon() : b2->getEnvironmentWeapon();
		//ew->isUsed = true; //right now, no activation sequence
		
		state->ship->isActivatingEnvironment = true; //right now, no activation sequence
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
			return;
		}
		return;
	}

	//fucking up zombies
	Zombie *z;
	if ((b1->type == ZombieType && b2->type == ShipType) || (b1->type == ShipType && b2->type == ZombieType)){
			state->ship->isDestroyed = true;
	}

}

void GameController::EndContact(b2Contact* contact){

}
/*
void GameController::createZombies(){
	Zombie *z1 = new Zombie(PLANET1_POS.x, PLANET1_POS.y, state->world);
	Zombie *z2 = new Zombie(PLANET2_POS.x, PLANET2_POS.y, state->world);
	state->zombies.AddTail(z1);
	state->zombies.AddTail(z2);
	view->enviornment->addChild(z1->sprite);
	view->enviornment->addChild(z2->sprite);
}

void GameController::createWalls(){
	state->level->walls = new Wall[30];
	state->level->nWalls = 30;
	int j = 0;
	// set up the walls here
	//we can move the below code to level editor later so it looks clean
	//-------------------------------f------------------------------------------------------
	// a vertical wall here
	for (int i = 0; i < 20; i++, j++) {
		state->level->walls[j].init(state->world, SPACE_TILE*5.5f, SPACE_TILE*(5.125f + i*0.25f));
		view->enviornment->addChild(state->level->walls[j].sprite);
		//new_wall->setSprite(view->walls[i]);
	}

	// a horizontal wall here
	for (int i = 20; i < 30; i++, j++) {
		state->level->walls[j].init(state->world, SPACE_TILE*(5.5f + (i - 19)*0.25f), SPACE_TILE*5.125f);
		view->enviornment->addChild(state->level->walls[j].sprite);
		//new_wall->setSprite(view->walls[i]);
	}
	state->level->markWallTiles();
}

void GameController::createWeapons(){
	//Sword *s1 = new Sword(state->world, SPACE_TILE*5.9f, SPACE_TILE*(5.5f + 1*0.25f)); //this makes it corner inside of wall
	Sword *s1 = new Sword(state->world, SPACE_TILE*9.0f, SPACE_TILE*(5.5f + 1 * 0.25f));
	state->weapons.AddTail(s1);
	view->enviornment->addChild(s1->sprite);
}*/

//purpose of this function is to set a level when choosing from start menu
void GameController::setCurrentLevel(int level){
	currentLevel = level;
}

void GameController::createFog() {
	//add the fog of war here
	fogSp = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("fog"));
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
			fogSp->setScale(FOG_SCALE + (meter->detectionRadius - MIN_DETECTION_RADIUS)/120.0f, FOG_SCALE + (meter->detectionRadius - MIN_DETECTION_RADIUS)/120.0f);

		//}

		fogSp->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
		//fogSpOuter->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
	}
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
	meter = new GrooveMeter();
	Director* director = Director::getInstance();
	Size winsize = director->getWinSizeInPixels();
	view = new View(winsize.width, winsize.height);
	view->scene->addChild(this);
	state = NULL;
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
	audio = new AudioController();
	loadLevel(currentLevel);

	ai = new AIController();
	environmentalTimer = 0.0f;
	processDirection = false;
	currentEnvironment = NULL;
	currentMower = NULL;

	//add the fog of war here
	musicNoteCounter = 0; //initialize the sequence of music note path
	// Tell the director we are ready for animation.
	this->scheduleUpdate();
	isPaused = false;

	//create the pause button on the upper right corner
	//---remove these later, these don't really do anything, just to make sure yall have all the textures -----
	Sprite* resumeButtonTexture = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("resume_button"));
	Sprite* resumeButtonClickedTexture = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("resume_button_clicked"));
	Sprite* restartButtonTexture = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("restart_button"));
	Sprite* restartButtonClickedTexture = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("restart_button_clicked"));
	Sprite* pause = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("pause_button"));
	Sprite* pauseClicked = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("pause_button_clicked"));
	//---remove above later----- 

	createPauseButton();
}

//helper method, create pauseButton on the upper right corner
void GameController::createPauseButton() {
	auto pauseButton = MenuItemImage::create("textures/pause_button.png", "textures/pause_button_clicked.png", CC_CALLBACK_0(GameController::pauseGame, this));
	Size visibleSize2 = Director::getInstance()->getVisibleSize();
	Vec2 origin2 = Director::getInstance()->getVisibleOrigin();
	pauseButton->setPosition(Point(visibleSize2.width*0.94 + origin2.x, visibleSize2.height*0.92 + origin2.y));
	pauseButton->setScale(0.2f);

	auto pauseButtonMenu = Menu::create(pauseButton, NULL);
	pauseButtonMenu->setPosition(Point::ZERO);
	this->addChild(pauseButtonMenu);
}

Vec2 GameController::mouseToWorld(Vec2 click){
	b2Vec2 pos = state->ship->body->GetPosition();
	return Vec2((input->lastClick.x - view->screen_size_x / 2.0) / view->resIndepScreen->getScale() + pos.x, 
		-(input->lastClick.y - view->screen_size_y / 2.0) / view->resIndepScreen->getScale() + pos.y);
}

bool GameController::hasWonLevel(){
	if (currentLevel != CALIBRATION_LEVEL) return state->zombies.GetCount() == 0;
	else {
		if (calibration->audioCalibration) return audio->songIsOver() && calibration->phaseDelay > 5.0f;
		else return elapsedTime - calibration->zombieTimes[15] > 5.0f;
	}
}

void GameController::loadLevel(int i){
	currentLevel = i;
	destination = 0;
	isPaused = false;
	input->clickProcessed = true;
	dRickyTap = new Vec2(1.0f,0.0f);
	dRickyTap->normalize();
	elapsedTime = 0.0;
	stringstream ss;
	ss << "levels/level" << currentLevel << ".zbl";
	CC_SAFE_DELETE(state);
	state = ls.parseLevel(ss.str());
	state->world->SetContactListener(this);

	// Build the scene graph and create the ship model.

	ls.addObjects(state);
	this->removeAllChildren();
	//view->releaseScene();
	view->buildScene(state->level, this, i);
	createPauseButton();
	view->allSpace->addChild(state->ship->getSprite());
	createFog();
	for (int i = 0; i<state->level->nWalls; i++) view->enviornment->addChild(state->level->walls[i].sprite);
	for (CTypedPtrDblElement<Zombie> *cz = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(cz); cz = cz->Next()) view->zombies->addChild(cz->Data()->sprite, 2);
	for (CTypedPtrDblElement<Weapon> *cw = state->weapons.GetHeadPtr(); !state->weapons.IsSentinel(cw); cw = cw->Next()) view->enviornment->addChild(cw->Data()->sprite, 2);
	for (CTypedPtrDblElement<EnvironmentWeapon> *ew = state->environment_weapons.GetHeadPtr(); !state->environment_weapons.IsSentinel(ew); ew = ew->Next()) view->enviornment->addChild(ew->Data()->sprite, 2);
	//initial detection radius
	meter->detectionRadius = INITIAL_DETECTION_RADIUS;
	currAwareness = 0.0f;
	currentFingerPos = Vec2(0.0f, 0.0f);
	if (currentLevel == CALIBRATION_LEVEL){
		calibration->init();
		view->objective->setString("Audio Calibration: Tap anywhere to the beat after the first four, don't miss any!");
	}
	audio->playTrack(ls.getLevelTrack(), currentLevel != CALIBRATION_LEVEL);
}

//restart the game upon death or reset
void GameController::restartGame() {
	loadLevel(currentLevel);
	removeGameMenu();
}

void GameController::goBackToMainMenu() {
	audio->stop();
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(LEVEL_MENU_TRANSITION_TIME, scene));
}

void GameController::pauseGame() {
	isPaused = true;
	createGameMenu();
}

void GameController::resumeGame() {
	isPaused = false;
	removeGameMenu();
}

void GameController::createWeaponRanges(float weapWidth, float weapRange, b2Vec2 dir){
	weaponRectangle[0] = b2Vec2(-weapRange / 2.0f, weapWidth / 2.0f); //top left
	weaponRectangle[1] = b2Vec2(weapRange / 2.0f,weapWidth / 2.0f); //top right
	weaponRectangle[2] = b2Vec2(-weapRange / 2.0f ,- weapWidth / 2.0f); //bottom left
	weaponRectangle[3] = b2Vec2(weapRange / 2.0f,-weapWidth / 2.0f); //bottom right

	float theta = atan2(dir.y, dir.x);
	b2Rot rotationM = b2Rot(theta);

	//add ricky pos to each point and translate box in front by mult dir facing * half range weap
	for (int i = 0; i < sizeof(weaponRectangle)/sizeof(b2Vec2); i++){
		b2Vec2 rickPos = state->ship->body->GetPosition();
		weaponRectangle[i] = b2Vec2(weaponRectangle[i].y, weaponRectangle[i].x);
		weaponRectangle[i] = b2MulT(rotationM,weaponRectangle[i]);
		weaponRectangle[i] = b2Vec2(weaponRectangle[i].y, weaponRectangle[i].x);
		weaponRectangle[i] = weaponRectangle[i] + rickPos + ((weapRange / 2.0f + SHIP_HEIGHT)*dir);

	}
	
}

bool GameController::isZombieHit(b2Vec2 az, b2Vec2 bz, b2Vec2 ab, b2Vec2 bc){
	//0 <= dot(AB, AM) <= dot(AB, AB) &&
	//	0 <= dot(BC, BM) <= dot(BC, BC)
	if ((0 <= b2Dot(ab, az) && b2Dot(ab, az) <= b2Dot(ab, ab)) && (0 <= b2Dot(bc, bz) && b2Dot(bc, bz) <= b2Dot(bc, bc))){
		return true;
	}
	else return false;
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
		if (eweap->isUsed || eweap->hitWall){
			eweap->isUsed = false;
			state->world->DestroyBody(eweap->body);
			toDelete = e_weapon;
			view->enviornment->removeChild(eweap->sprite);
		}
	}
	if (toDelete != NULL){
		state->environment_weapons.Remove(toDelete);
	}
}

void GameController::removeDeadZombies(){
	CTypedPtrDblList<CTypedPtrDblElement<Zombie>> zombsToDel;
	for (CTypedPtrDblElement<Zombie> *zombie = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(zombie);  zombie = zombie->Next())
	{
		Zombie *zomb = zombie->Data();
		if (zomb->isDestroyed){
			zomb->isDestroyed = false;
			zombsToDel.AddTail(zombie);
			view->zombies->removeChild(zomb->sprite);
		}
		
	}
	for (CTypedPtrDblElement<CTypedPtrDblElement<Zombie>> *z = zombsToDel.GetHeadPtr(); !zombsToDel.IsSentinel(z);  z = z->Next()){
		delete z->Data()->Data();
		state->zombies.Remove(z->Data());
	}
}

#define VIDEO_CALIBRATION_OFFSET 150
void GameController::startVideoCalibration(){
	view->objective->setString("Now tap just as the center of each zombie lines up with Ricky, don't miss any!");
	calibration->totalOffset = 0.0;
	calibration->clicks = 0;
	calibration->acceptClicks = true;
	state->ship->body->SetLinearVelocity(b2Vec2_zero);
	Vec2 anchor = Vec2(0.5f, 0.5f);
	b2Vec2 p = state->ship->body->GetPosition();
	calibration->audioCalibration = false;
	view->zombies->removeAllChildren();
	view->zombies->setPosition(Vec2::ZERO);
	for (int i = 0; i < 16; i++){
		calibration->zombies[i] = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("zombie_single"));
		calibration->zombies[i]->setAnchorPoint(anchor);
		calibration->zombies[i]->setPosition(p.x + VIDEO_CALIBRATION_OFFSET*i + 2 * VIDEO_CALIBRATION_OFFSET, p.y+80);
		calibration->zombieTimes[i] = elapsedTime * 2;
		view->zombies->addChild(calibration->zombies[i]);
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
	if (!isPaused) {
		if (hasWonLevel()){
			if (currentLevel == CALIBRATION_LEVEL){
				if (calibration->audioCalibration){
					if (calibration->clicks < 32){
						restartGame();
					}
					else{
						startVideoCalibration();
					}
				}
				else{
					if (calibration->clicks < 16){
						startVideoCalibration();
					}
					else{
						loadLevel(std::min(currentLevel + 1, MAX_LEVELS));
					}
				}
			}
			else{
				loadLevel(std::min(currentLevel + 1, MAX_LEVELS));
			}
		}
		elapsedTime += deltaTime;
		audio->setFrameOnBeat(deltaTime);
		removeDeadWeapons();
		removeDeadEWeapons();
		removeDeadZombies();

		//if we are currently activating environment and player clicking
		if (state->ship->isActivatingEnvironment){
			environmentalTimer += deltaTime; //just ran into environmental object so start delay count before it registers next click
		}

		//delay is up so now 
		if (input->clicked &&  environmentalTimer >= ENVIRONMENTAL_WEAPON_DELAY){
			processDirection = true;
			currentFingerPos = Vec2((input->lastClick.x - view->screen_size_x / 2.0) + state->ship->body->GetPosition().x, -1.0f*(input->lastClick.y - view->screen_size_y / 2.0) + state->ship->body->GetPosition().y);
			currentFingerPos.subtract(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
			currentFingerPos.normalize();
			currentFingerPos.scale(225.0f);
			currentFingerPos.add(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
		}

		bool activated = false;

		if (environmentalTimer >= ENVIRONMENTAL_WEAPON_DELAY_MAX || (!input->clicked &&  environmentalTimer >= ENVIRONMENTAL_WEAPON_DELAY)){
			environmentalTimer = 0.0f;
			state->ship->isActivatingEnvironment = false;
			processDirection = false;
			activated = true;
			//TODO: Process moving the mower, etc. make mower variable that says in use..might have and make sure it is still active. Iterate thru mower list and point to the correct one, then switch position of ricky to it and it to slightly towards new direction

			currentEnvironment->isUsed = true;

		}

		if (currentEnvironment != NULL && currentEnvironment->isUsed && !currentEnvironment->hasMoved){
			Vec2 mowerDir = currentFingerPos;
			mowerDir.subtract(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
			mowerDir.normalize();
			mowerDir.scale(125.0f);
			mowerDir.add(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));

			b2Vec2 dir = b2Vec2(mowerDir.x, mowerDir.y);
			Lawnmower *lm = new Lawnmower(state->world, mowerDir.x, mowerDir.y, dir);
			state->environment_weapons.AddTail(lm);
			view->enviornment->addChild(lm->sprite, 2); //lawnmower's z-order needs to be changed the same as envrionment weapon 
			currentMower = lm;
			currentEnvironment = lm;

			/*
			b2Vec2 dir = b2Vec2(mowerDir.x,mowerDir.y);
			dir.Normalize();
			dir *= MOWER_IMPULSE;
			lm->body->ApplyLinearImpulse(dir, lm->body->GetPosition(), true);*/
		}
		if (state->ship->isDestroyed){
			restartGame();
		}
		updateFog();
		float x = state->ship->body->GetPosition().x;
		float y = state->ship->body->GetPosition().y;
		MapNode *from = state->level->locateCharacter(x, y);
		if (!input->clickProcessed && !activated){
			if (currentLevel != CALIBRATION_LEVEL || (calibration->audioCalibration && calibration->acceptClicks)){
				//cout << "PBF: " << elapsedTime << "\n";
				stringstream st;
				//char* tempstr = "kgkgk";
				//std::sprintf(tempstr, "%f", elapsedTime);

				//view->beatHUD->setString("Actual time: " + std::to_string(elapsedTime) + " song time: " + std::to_string(AudioEngine::getCurrentTime(audioid)));
				time_t now = time(0);
				bool userOnBeat = audio->wasOnBeat(now - input->clickTime);
				if (!userOnBeat && currentLevel != CALIBRATION_LEVEL) {
					state->ship->body->SetLinearVelocity(b2Vec2_zero);
					destination = 0;

					//if it is not on beat, increase the detection radius slightly
					meter->increaseRadius();
				}
				else{
					state->ship->boostFrames = MAX_BOOST_FRAMES;
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
						/*if ((destination != 0 && destPrev != 0) && (destination == destPrev)){
							dRickyTap = dRickyTap;
							}
							if (state->ship->currentWeapon->durability < 1){
								state->ship->hasWeapon = false;
								free(state->ship->currentWeapon);
								state->ship->currentWeapon = NULL;
							else{
							dRickyTap = new Vec2(destination->x - destPrev->x, destination->y - destPrev->y);
							dRickyTap->normalize();
							}*/
						dRickyTap->set(click.x - state->ship->body->GetPosition().x, click.y - state->ship->body->GetPosition().y);
						float theta = atan2(dRickyTap->y, dRickyTap->x);
						theta = round(theta / (M_PI / 4.0f)) * (M_PI / 4.0f);

						createWeaponRanges(state->ship->currentWeapon->width, state->ship->currentWeapon->range, b2Vec2(cos(theta), sin(theta)));

						//detect if zombies are inside rectangle of weapon
						CTypedPtrDblElement<Zombie> *zambie = state->zombies.GetHeadPtr();
						int num_zombies_killed = 0;
						while (!state->zombies.IsSentinel(zambie))
						{
							Zombie *zombb = zambie->Data();
							//AB is vector AB, with coordinates (Bx-Ax,By-Ay)
							b2Vec2 az = b2Vec2(zombb->body->GetPosition().x - weaponRectangle[0].x, zombb->body->GetPosition().y - weaponRectangle[0].y);
							b2Vec2 bz = b2Vec2(zombb->body->GetPosition().x - weaponRectangle[1].x, zombb->body->GetPosition().y - weaponRectangle[1].y);
							b2Vec2 ab = b2Vec2(weaponRectangle[1].x - weaponRectangle[0].x, weaponRectangle[1].y - weaponRectangle[0].y);
							b2Vec2 bc = b2Vec2(weaponRectangle[3].x - weaponRectangle[1].x, weaponRectangle[3].y - weaponRectangle[1].y);

							if (isZombieHit(az, bz, ab, bc)){
								num_zombies_killed += 1;
								//zombie got hit so delete it
								zombb->isDestroyed = true;
							}
							zambie = zambie->Next();
						}
						if (num_zombies_killed > 0){
							state->ship->currentWeapon->durability -= 1;
						}
						if (state->ship->currentWeapon->durability == 0){
							state->ship->hasWeapon = false;
							free(state->ship->currentWeapon);
							state->ship->currentWeapon = NULL;
						}

					}

				}
				if (currentLevel == CALIBRATION_LEVEL){
					if (calibration->acceptClicks){
						calibration->totalOffset += audio->timeToBeat(4 + calibration->clicks);
						st << "OFFSET: " << audio->timeToBeat(4 + calibration->clicks);
						calibration->clicks++;
						view->beatHUD->setString(st.str());
						if (calibration->clicks >= 32) calibration->acceptClicks = false;
					}
				}
				else{
					if (userOnBeat){
						st << "HIT";
					}
					else{
						st << "MISS BY: " << audio->timeToClosestBeat();
					}
					view->beatHUD->setString(st.str());
				}
				input->clickProcessed = true;
				//check if there is a zombie within the radius
				CTypedPtrDblElement<Zombie> *cur = state->zombies.GetHeadPtr();
				Zombie *curZ;
				b2Vec2 tmp;
				int count = 0;
				while (!state->zombies.IsSentinel(cur)){
					curZ = cur->Data();
					tmp = state->ship->body->GetPosition() - curZ->body->GetPosition();
					//if this zombie is within our detection radius and we messed up the beat
					float dis;
					dis = sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
					if (!audio->frameOnBeat && dis < meter->detectionRadius) {
						audio->playEffect("sound_effects/ZombieHiss.mp3");
						curZ->increaseAwarness();
					}
					if (count == 0) {
						currAwareness = curZ->awareness;
					}

					count++;
					cur = cur->Next();
				}


			}
			 else{
				 if (calibration->acceptClicks){
					 time_t now = time(0);
					 calibration->totalOffset += (((elapsedTime - (now - input->clickTime)) - audio->audioDelay) - calibration->zombieTimes[calibration->clicks]);
					 view->zombies->removeChild(calibration->zombies[calibration->clicks]);
					 calibration->clicks++;
					 input->clickProcessed = true;
					 if (calibration->clicks == 16){
						 calibration->acceptClicks = false;
						 audio->videoDelay = calibration->videoDelay();
						 stringstream ss;
						 ss << "Ok, great.  Video Delay: " << int(audio->videoDelay * 1000) << " ms.";
						 view->objective->setString(ss.str());
					 }
				 }
			 }
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
			if (!calibration->audioCalibration){
				view->zombies->setPosition(view->zombies->getPosition() + Vec2(-2, 0));
				for (int i = calibration->clicks; i < 16; i++){
					if (calibration->zombieTimes[i]>elapsedTime){
						calibration->zombieTimes[i] = elapsedTime + (VIDEO_CALIBRATION_OFFSET*i + 2 * VIDEO_CALIBRATION_OFFSET + view->zombies->getPosition().x) / 2 * deltaTime;
					}
				}
				if (elapsedTime - calibration->zombieTimes[15] > 1.0f){
					calibration->acceptClicks = false;
					if (calibration->clicks != 16) view->objective->setString("You missed some of the zombies, try again.");
				}
			}
			else{
				if (audio->songTime>20.0f || calibration->clicks == 32){
					audio->stop();
					stringstream ss;
					if (calibration->clicks < 32){
						ss << "You missed some of the the beats, try again.";  
					}
					else{
						audio->audioDelay = calibration->audioDelay();
						ss << "Ok, great! Now stop tapping! Audio delay: " << int(audio->audioDelay * 1000) << " ms.";
						calibration->acceptClicks = false;
					}
					view->objective->setString(ss.str());
				}
				if (audio->songIsOver()){
					calibration->phaseDelay += deltaTime;
				}
			}
		}


		activated = false;

		if (state->ship->isActivatingEnvironment){
			state->ship->body->SetLinearVelocity(b2Vec2_zero);
			destination = 0;
		}

		if (currentMower != NULL && currentMower->hasMoved){
			Vec2 mowerDir = currentFingerPos;
			mowerDir.subtract(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y));
			mowerDir.normalize();

			currentMower->update(deltaTime, mowerDir);
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
				z->Data()->sprite->setVisible(true);
				z->Data()->sprite->setOpacity(255);
				z->Data()->sprite->setPosition(pos.x, pos.y);
				z->Data()->advanceFrame();
			}
			else{
				z->Data()->sprite->setVisible(true);
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
			envwe = envwe->Next();
		}
	}

	//game is paused, check if they click on the game menu buttons
	else {
		//do nothing, might gonna need it to do something later
	}
}

//draw a new music note path
void GameController::drawMusicNotePath(Vec2 origin) {
	Sprite* singleMusicNote;
	if (musicNoteCounter == 0) {
		singleMusicNote = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("music_note"));
	}
	else if (musicNoteCounter == 1) {
		singleMusicNote = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("music_note"));
	}
	else {
		singleMusicNote = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("music_note"));
	}
	singleMusicNote->setScale(MUSIC_NOTE_SCALE);
	singleMusicNote->setPosition(Point(origin.x, origin.y));
	view->enviornment->addChild(singleMusicNote, 1);
	musicNotes.push_back(singleMusicNote);
	if (musicNoteCounter == 2) {
		musicNoteCounter = 0;
	}
	else {
		musicNoteCounter++;
	}
}

//clear the old path for every iteration
void GameController::clearMusicNotePath() {
	for (int i = 0; i < musicNotes.size(); i++) {
		Sprite* aMusicNote = musicNotes[i];
		view->enviornment->removeChild(aMusicNote);
		//aMusicNote->release();
	}
	musicNotes.clear();
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
	clearMusicNotePath();
	view->directionUseEnvironmentWeapon->clear();
	//clear the old detection circle
	view->detectionRadiusCircle->clear();
	//clear the old hitbox
	view->hitBox->clear();
	view->weaponBox->clear();

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
	if (processDirection){
		view->directionUseEnvironmentWeapon->drawLine(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), Vec2(currentFingerPos.x, currentFingerPos.y), ccColor4F(0.0f, 0.0f, 0.0f, 0.8f));
	}

	view->durability->setString("Durability");
	view->durabilityHolder->clear();
	view->durabilityHolder->drawSolidRect(Vec2(0, 0), Vec2(110, -150), ccColor4F(0.0f, 0.0f, 0.0f, 1.0f));
	view->durabilityHolder->drawRect(Vec2(0, 0), Vec2(110, -150), ccColor4F(0.5f, 0.5f, 0.5f, 1.0f));

	if (state->ship->hasWeapon){
		view->hitBox->clear();
		view->weaponBox->drawRect(Vec2(weaponRectangle[0].x, weaponRectangle[0].y), Vec2(weaponRectangle[1].x, weaponRectangle[1].y), Vec2(weaponRectangle[3].x, weaponRectangle[3].y), Vec2(weaponRectangle[2].x, weaponRectangle[2].y),ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));

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
		view->durabilitySpriteContainer->clear();
		int dur = state->ship->currentWeapon->durability;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 2; ++j){
				if (dur < 1){ //fix
					break;
				}
				else{
					//draw the durability circle
					view->durabilitySpriteContainer->drawSolidCircle(Vec2(25.0f+j*50.0f,-(25.0f+i*50.0f)), 8.0f, 0.0f, 20.0f, ccColor4F(0.5f, 0, 0, 1.0f));
					dur--;
				}
			}
		}
	}

	if (!state->ship->hasWeapon){
		view->durabilitySpriteContainer->clear();
		view->weaponBox->drawRect(Vec2(weaponRectangle[0].x, weaponRectangle[0].y), Vec2(weaponRectangle[1].x, weaponRectangle[1].y), Vec2(weaponRectangle[3].x, weaponRectangle[3].y), Vec2(weaponRectangle[2].x, weaponRectangle[2].y), ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));
	}
	//view->directionUseEnvironmentWeapon->drawLine(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), currentFingerPos, ccColor4F(128.0f, 128.0f, 128.0f, 0.5f));

	if (destination != 0){
		MapNode *last = state->level->locateCharacter(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
		MapNode *cur = destination;
		do {
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
		} while (cur != 0);
	}
	/*view->ai->clear();
	for (CTypedPtrDblElement<Zombie> *z = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(z); z = z->Next()){
	Zombie *zom = z->Data();
	b2Vec2 pos = zom->body->GetPosition();
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->seperation.x, pos.y + zom->seperation.y), ccColor4F(1, 0, 0, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->attraction.x, pos.y + zom->attraction.y), ccColor4F(0, 1, 0, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->aidir.x / 28, pos.y + zom->aidir.y / 28), ccColor4F(0, 0, 0, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->alignment.x, pos.y + zom->alignment.y), ccColor4F(1, 0, 1, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->cohesion.x, pos.y + zom->cohesion.y), ccColor4F(0, 0, 1, 1.0f));
	view->ai->drawLine(Vec2(pos.x, pos.y), Vec2(pos.x + zom->zombiness.x, pos.y + zom->zombiness.y), ccColor4F(1, 1, 0, 1.0f));
	}*/

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
		ss << state->zombies.GetCount() << " zombies left!";
		view->objective->setString(ss.str());
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

