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
#include "SimpleAudioEngine.h"
#include "Wall.h"
#include "Sword.h"
#include "Ship.h"
#include "Lawnmower.h"
#include "Zombie.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include "audio/include/AudioEngine.h"
#include "AIController.h"
#include "Box2d/Box2d.h"

#define STARTING_LEVEL 2

using namespace experimental;
float lastbeat = 0;
int audioid = 0;
float keepit = 0;
float total_kept = 0;
float total_beats = 0;
float estimated_song_time = 0;
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

void GameController::createFog() {
	//add the fog of war here
	fogSp = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("fog"));
	//fogSpOuter = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("fog_outer"));
	fogSp->setScale(FOG_SCALE, FOG_SCALE);
	//fogSpOuter->setScale(OUTER_FOG_SCALE, OUTER_FOG_SCALE);
	fogSp->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
	//fogSpOuter->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
	view->enviornment->addChild(fogSp);
	//view->enviornment->addChild(fogSpOuter);
}

void GameController::createGameMenu() {
	pauseMenu = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("pause_menu"));
	pauseMenu->setScale(0.15f);
	pauseMenu->setPosition(Vec2(HUD_OFFSET.x * 50, HUD_OFFSET.y * 35));
	this->addChild(pauseMenu);
}

void GameController::removeGameMenu() {
	this->removeChild(pauseMenu);
}

void GameController::updateFog() {
	if (fogSp != NULL) {
		//if (INITIAL_DETECTION_RADIUS / detectionRadius > 0.65f) {
			fogSp->setScale(FOG_SCALE + (detectionRadius - MIN_DETECTION_RADIUS)/120.0f, FOG_SCALE + (detectionRadius - MIN_DETECTION_RADIUS)/120.0f);

		//}

		fogSp->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
		//fogSpOuter->setPosition(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
	}
}



bool GameController::init() {
	if (!Layer::init()) {
		return false;
	}
	Director* director = Director::getInstance();
	cocos2d::Size winsize = director->getWinSizeInPixels();
	view = new View(winsize.width, winsize.height);
	view->scene->addChild(this);

	curLevel = STARTING_LEVEL;
	loadLevel(curLevel);

	//add the fog of war here
	createFog();


	ai = new AIController();
	environmentalTimer = 0.0f;
	processDirection = false;
	currentEnvironment = NULL;
	currentMower = NULL;
	//createZombies();
	//createWalls();
	//createWeapons();
	// Start listening to input
	input = new InputController(_eventDispatcher);
	input->startInput();

	// Tell the director we are ready for animation.
	this->scheduleUpdate();
	isPaused = false;
	return true;
}

void GameController::loadLevel(int i){
	destination = 0;
	onBeat = false;
	dRickyTap = new Vec2(1.0f,0.0f);
	dRickyTap->normalize();
	elapsedTime = 0.0;
	stringstream ss;
	ss << "levels/level" << i << ".zbl";
	state = ls.parseLevel(ss.str());
	state->world->SetContactListener(this);

	// Build the scene graph and create the ship model.

	ls.addObjects(state);
	this->removeAllChildren();
	view->buildScene(state->level, this);
	view->allSpace->addChild(state->ship->getSprite());
	for (int i = 0; i<state->level->nWalls; i++) view->enviornment->addChild(state->level->walls[i].sprite);
	for (CTypedPtrDblElement<Zombie> *cz = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(cz); cz = cz->Next()) view->enviornment->addChild(cz->Data()->sprite);
	for (CTypedPtrDblElement<Weapon> *cw = state->weapons.GetHeadPtr(); !state->weapons.IsSentinel(cw); cw = cw->Next()) view->enviornment->addChild(cw->Data()->sprite);
	for (CTypedPtrDblElement<EnvironmentWeapon> *ew = state->environment_weapons.GetHeadPtr(); !state->environment_weapons.IsSentinel(ew); ew = ew->Next()) view->enviornment->addChild(ew->Data()->sprite);
	//initial detection radius
	detectionRadius = INITIAL_DETECTION_RADIUS;
	currAwareness = 0.0f;
	AudioEngine::stopAll();
	currentFingerPos = Vec2(0.0f, 0.0f);
	currentSong = new SongDecomposition(128.0, "songs/ChillDeepHouse.mp3", -0.05);
	audioid = AudioEngine::play2d("songs/01 OverDrive.mp3", true, 1);
}

//restart the game upon death or reset
void GameController::restartGame() {
	AudioEngine::stopAll();
	this->removeAllChildren();
	loadLevel(curLevel);
	createFog();
	input->clickProcessed = true;
	destination = 0;
	isPaused = false;
}

void GameController::pauseGame() {
	isPaused = true;
}

void GameController::resumeGame() {
	isPaused = false;
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
		weaponRectangle[i] = b2MulT(rotationM,weaponRectangle[i]);
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

/**
* Update the game state.
*
* This method is called every animation frame.  There is no draw() or render()
* counterpoint to this method; drawing is done automatically in the scene graph.
* However, this method is responsible for updating any transforms in the scene graph.
*/
void GameController::update(float deltaTime) {
	if (!isPaused) {
		if (!input->clickProcessed && input->lastClick.x < 100 && input->lastClick.y < 100) {
			pauseGame();
			createGameMenu();
			return;
		}

		// Read the thrust from the user input
		//input->update();
		Vec2 thrust = input->lastClick;
		elapsedTime += deltaTime; //a float in seconds


		//cout << "Elapsed time: "<< elapsedTime <<endl;
		// Read the thrust from the user input
		//input->update();
		//bool clicked = input->didClick();

		CTypedPtrDblElement<Weapon> *weapon = state->weapons.GetHeadPtr();
		CTypedPtrDblElement<Weapon> *toDelete = NULL;
		while (!state->weapons.IsSentinel(weapon))
		{
			Weapon *weap = weapon->Data();
			if (weap->isDesroyed){
				weap->isDesroyed = false;
				toDelete = weapon;
				state->world->DestroyBody(weap->body);
				view->enviornment->removeChild(weap->sprite);
			}
			weapon = weapon->Next();
		}
		if (toDelete != NULL){
			state->weapons.Remove(toDelete);
		}

		CTypedPtrDblElement<EnvironmentWeapon> *e_weapon = state->environment_weapons.GetHeadPtr();
		CTypedPtrDblElement<EnvironmentWeapon> *toDelete1 = NULL;
		EnvironmentWeapon *eweap = NULL;
		while (!state->environment_weapons.IsSentinel(e_weapon))
		{
			eweap = e_weapon->Data();
			if (eweap->isUsed || eweap->hitWall){
				eweap->isUsed = false;
				toDelete1 = e_weapon;
				state->world->DestroyBody(eweap->body);
				view->enviornment->removeChild(eweap->sprite);
			}
			e_weapon = e_weapon->Next();
		}
		if (toDelete1 != NULL){
			state->environment_weapons.Remove(toDelete1);
		}

		CTypedPtrDblElement<Zombie> *zombie = state->zombies.GetHeadPtr();
		CTypedPtrDblElement<Zombie> *zombToDel = NULL;
		while (!state->zombies.IsSentinel(zombie))
		{
			Zombie *zomb = zombie->Data();
			if (zomb->isDestroyed){
				zomb->isDestroyed = false;
				zombToDel = zombie;
				state->world->DestroyBody(zomb->body);
				view->enviornment->removeChild(zomb->sprite);
			}
			zombie = zombie->Next();
		}
		if (zombToDel != NULL){
			state->zombies.Remove(zombToDel);
		}

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
			view->enviornment->addChild(lm->sprite);
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
			//if on beat then flag it
			//AudioEngine::getCurrentTime(audioid)
			onBeat = currentSong->isOnBeat(elapsedTime - keepit);

			//cout << "PBF: " << elapsedTime << "\n";
			stringstream st;
			//char* tempstr = "kgkgk";
			//std::sprintf(tempstr, "%f", elapsedTime);

			//view->beatHUD->setString("Actual time: " + std::to_string(elapsedTime) + " song time: " + std::to_string(AudioEngine::getCurrentTime(audioid)));
			if (!onBeat) {
				state->ship->body->SetLinearVelocity(b2Vec2_zero);
				destination = 0;

				//if it is not on beat, increase the detection radius slightly
				if (detectionRadius < MAX_DETECTION_RADIUS) {
					detectionRadius += DETECTION_RADIUS_INCREASE;
				}
			}
			else{
				state->ship->boostFrames = MAX_BOOST_FRAMES;
				float xClick = (input->lastClick.x - view->screen_size_x / 2.0) + x;
				float yClick = -(input->lastClick.y - view->screen_size_y / 2.0) + y;
				MapNode *dest = state->level->locateCharacter(xClick,yClick);
				state->level->shortestPath(from, dest);
				destination = from->next;
				MapNode *destPrev = from;

				//if it is on beat, decrease the detection radius slightly
				if (detectionRadius > MIN_DETECTION_RADIUS) {
					detectionRadius -= DETECTION_RADIUS_DECREASE;
				}

				if (state->ship->hasWeapon){
					//if you have a weapon and click on the beat, check if you kill any zombies in that direction
					//mouseclick to world coords subtract rickies pos from that...normalize it...snap it to nearest rotation: round nearest int(arctan2(normalized) /. pi/2) * pi/4
					/*if ((destination != 0 && destPrev != 0) && (destination == destPrev)){
						dRickyTap = dRickyTap;
					}
					else{
						dRickyTap = new Vec2(destination->x - destPrev->x, destination->y - destPrev->y);
						dRickyTap->normalize();
					}*/
					dRickyTap->set(xClick - state->ship->body->GetPosition().x, yClick - state->ship->body->GetPosition().y);
					float theta = atan2(dRickyTap->y, dRickyTap->x);
					theta = round(theta / (M_PI / 4.0f)) * (M_PI / 4.0f);

					createWeaponRanges(state->ship->currentWeapon->width, state->ship->currentWeapon->range, b2Vec2(cos(theta), sin(theta)));
					
					//detect if zombies are inside rectangle of weapon
					CTypedPtrDblElement<Zombie> *zambie = state->zombies.GetHeadPtr();
					while (!state->zombies.IsSentinel(zambie))
					{
						Zombie *zombb = zambie->Data();
						//AB is vector AB, with coordinates (Bx-Ax,By-Ay)
						b2Vec2 az = b2Vec2(zombb->body->GetPosition().x - weaponRectangle[0].x, zombb->body->GetPosition().y - weaponRectangle[0].y);
						b2Vec2 bz = b2Vec2(zombb->body->GetPosition().x - weaponRectangle[1].x, zombb->body->GetPosition().y - weaponRectangle[1].y);
						b2Vec2 ab = b2Vec2(weaponRectangle[1].x - weaponRectangle[0].x, weaponRectangle[1].y - weaponRectangle[0].y);
						b2Vec2 bc = b2Vec2(weaponRectangle[3].x - weaponRectangle[1].x, weaponRectangle[3].y - weaponRectangle[1].y);
						
						if (isZombieHit(az, bz, ab, bc)){
							//zombie got hit so delete it
							zombb->isDestroyed = true;
							state->ship->currentWeapon->durability -= 1;
							if (state->ship->currentWeapon->durability == 0){
								state->ship->hasWeapon = false;
								free(state->ship->currentWeapon);
								state->ship->currentWeapon = NULL;
							}
						}

						zambie = zambie->Next();
					}

				}

			}
			if (onBeat){
				st << "HIT";
			}
			else{
				st << "MISS BY: " << estimated_song_time - (elapsedTime - keepit);
			}
			view->beatHUD->setString(st.str());
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
				if (!onBeat && dis < detectionRadius) {
					AudioEngine::play2d("sound_effects/ZombieHiss.mp3", false, 1);
					curZ->increaseAwarness();
				}
				if (count == 0) {
					currAwareness = curZ->awareness;
				}

				count++;
				cur = cur->Next();
			}


		}

		if (destination != 0 && from == destination){
			destination = from->next;
		}
		if (destination != 0){
			Vec2 dir = Vec2(state->level->getTileCenterX(destination) - x, state->level->getTileCenterY(destination) - y);
			dir.normalize();
			if (!state->ship->update(deltaTime, dir)){
				destination = 0;
			}
		}
		ai->update(state);

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
			if ((z->Data()->body->GetPosition() - state->ship->body->GetPosition()).Length() <= detectionRadius + 40){
				pos = z->Data()->body->GetPosition();
				z->Data()->sprite->setVisible(true);
				z->Data()->sprite->setPosition(pos.x, pos.y);
				z->Data()->advanceFrame();
			}
			else{
				z->Data()->sprite->setVisible(false);
				if (!frameOnBeat){
					view->zombiePositions->clear();
				}else if (!prevOnBeat){
					pos = z->Data()->body->GetPosition();
					view->zombiePositions->drawSolidCircle(Vec2(pos.x, pos.y), 8.0f, 0.0f, 20.0f, ccColor4F(0.5f, 0, 0, 1.0f));
				}

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
		//if resume is clicked
		if (input->lastClick.x < 602 && input->lastClick.x > 389 && input->lastClick.y > 116 && input->lastClick.y < 176) {
			resumeGame();
			removeGameMenu();
		}
		else if (input->lastClick.x < 602 && input->lastClick.x > 389 && input->lastClick.y > 351 && input->lastClick.y < 415) {
			removeGameMenu();
			restartGame();
		}
		//do nothing
		else {

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
	stringstream ss;
	ss << "Coords: (" << (int)coords.x / 10 << "," << (int)coords.y / 10 << ")";
	view->coordHUD->setString(ss.str());
	stringstream s;
	s << "Speed: " << state->ship->body->GetLinearVelocity().Length();
	view->velHUD->setString(s.str());
	stringstream sss;
	sss << "Click: (" << input->lastClick.x << "," << input->lastClick.y << ")";
	view->thrustHUD->setString(sss.str());
	view->path->clear();
	view->directionUseEnvironmentWeapon->clear();
	//clear the old detection circle
	view->detectionRadiusCircle->clear();
	//clear the old hitbox
	view->hitBox->clear();
	view->weaponBox->clear();

	stringstream d;
	//d << "Detection Radius: " << detectionRadius;
	d << "Ricky Pos: " << state->ship->body->GetPosition().x << "," << state->ship->body->GetPosition().y;
	view->detectionRadiusHUD->setString(d.str());

	stringstream awr;
	//awr << "Zombie 1 Awarness: " << currAwareness;
	awr << "endpt: " << currentFingerPos.x << "," << currentFingerPos.y;
	view->zombieOneAwarenessHUD->setString(awr.str());

	//visualize the detection radius
	view->detectionRadiusCircle->drawCircle(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), detectionRadius, 0.0f, 1000, false, ccColor4F(0, 0, 2.0f, 1.0f));

	//visualize the hitbox for main character
	//view->hitBox->drawRect(Vec2(state->ship->body->GetPosition().x - 29.5f, state->ship->body->GetPosition().y - 55.0f), Vec2(state->ship->body->GetPosition().x + 29.5f, state->ship->body->GetPosition().y + 55.0f), ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));

	//visualize the arrow for activating the environment
	if (processDirection){
		view->directionUseEnvironmentWeapon->drawLine(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), Vec2(currentFingerPos.x, currentFingerPos.y), ccColor4F(0.0f, 0.0f, 0.0f, 0.8f));
	}

	if (state->ship->hasWeapon){
		view->hitBox->clear();
		view->weaponBox->drawRect(Vec2(weaponRectangle[0].x, weaponRectangle[0].y), Vec2(weaponRectangle[1].x, weaponRectangle[1].y), Vec2(weaponRectangle[3].x, weaponRectangle[3].y), Vec2(weaponRectangle[2].x, weaponRectangle[2].y),ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));
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
			view->path->drawLine(Vec2(x1, y1), Vec2(x2, y2), ccColor4F(1, 1, 1, 1.0f));
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


	stringstream st;
	total_kept += elapsedTime - AudioEngine::getCurrentTime(audioid);
	total_beats += 1;
	keepit = total_kept / total_beats;
	prevOnBeat = frameOnBeat;
	frameOnBeat = currentSong->isOnBeat(elapsedTime - keepit);
	if (frameOnBeat){//AudioEngine::getCurrentTime(audioid))){

		view->mainBeatHUD->setString("BEAT!");
		estimated_song_time = elapsedTime - keepit;

		st << "Difference is: " << keepit;
		//view->beatHUD->setString(st.str());
	}
	else{
		view->mainBeatHUD->setString("");
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

