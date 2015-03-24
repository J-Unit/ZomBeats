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
#include "Zombie.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include "audio/include/AudioEngine.h"
#include "AIController.h"

using namespace experimental;
float lastbeat = 0;
int audioid = 0;
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

	//wall contact
	if(b1->type == WallType || b2->type== WallType) return;
	
	//weapon pickup
	Weapon *w;
	if ((b1->type == WeaponType && b2->type == ShipType) || (b1->type == ShipType && b2->type == WeaponType)){
		w = (b1->type == WeaponType) ? b1->getWeapon() : b2->getWeapon();
		w->isDesroyed = true;
		state->ship->hasWeapon = true;
		state->ship->currentWeapon = w;
		return;
	}

	//fucking up zombies
	Zombie *z;
	if ((b1->type == ZombieType && b2->type == ShipType) || (b1->type == ShipType && b2->type == ZombieType)){
		if (state->ship->hasWeapon){
			z = (b1->type == ZombieType) ? b1->getZombie() : b2->getZombie();
			z->isDestroyed = true;
		}
		else{
			state->ship->isDestroyed = true;;
		}
	}
}

void GameController::EndContact(b2Contact* contact){

}

void GameController::createZombies(){
	Zombie *z1 = new Zombie(PLANET1_POS.x, PLANET1_POS.y, state->world);
	Zombie *z2 = new Zombie(PLANET2_POS.x, PLANET2_POS.y, state->world);
	state->zombies.AddTail(z1);
	state->zombies.AddTail(z2);
	view->enviornment->addChild(z1->sprite);
	view->enviornment->addChild(z2->sprite);

	/*b2BodyDef b1, b2;
	b1.position.Set(PLANET1_POS.x, PLANET1_POS.y);
	b2.position.Set(PLANET2_POS.x, PLANET2_POS.y);
	b2Body* b1b;
	b2Body* b2b;
	b1b = state->world->CreateBody(&b1);
	b2b = state->world->CreateBody(&b2);
	b2CircleShape c1, c2;
	c1.m_radius = 60;
	c2.m_radius = 60;
	b1b->CreateFixture(&c1, 0.0f);
	b2b->CreateFixture(&c2, 0.0f);*/
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
}

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

void GameController::updateFog() {
	if (fogSp != NULL && fogSpOuter != NULL) {
		if (INITIAL_DETECTION_RADIUS / detectionRadius > 0.65f) {
			fogSp->setScale(FOG_SCALE*(INITIAL_DETECTION_RADIUS / detectionRadius), FOG_SCALE*(INITIAL_DETECTION_RADIUS / detectionRadius));

		}

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

	loadLevel(1);

	//add the fog of war here
	createFog();


	ai = new AIController();
	//createZombies();
	//createWalls();
	//createWeapons();
    // Start listening to input
    input = new InputController(_eventDispatcher);
    input->startInput();

	// Tell the director we are ready for animation.
	this->scheduleUpdate();
    return true;
}

void GameController::loadLevel(int i){
	destination = 0;
	onBeat = false;
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
	for(int i=0;i<state->level->nWalls;i++) view->enviornment->addChild(state->level->walls[i].sprite);
	for (CTypedPtrDblElement<Zombie> *cz = state->zombies.GetHeadPtr(); !state->zombies.IsSentinel(cz); cz = cz->Next()) view->enviornment->addChild(cz->Data()->sprite);
	for (CTypedPtrDblElement<Weapon> *cw = state->weapons.GetHeadPtr(); !state->weapons.IsSentinel(cw); cw = cw->Next()) view->enviornment->addChild(cw->Data()->sprite);
	//initial detection radius
	detectionRadius = INITIAL_DETECTION_RADIUS;
	currAwareness = 0.0f;

	currentSong = new SongDecomposition(120.0, "songs/ChillDeepHouse.mp3", -.051);
	audioid = AudioEngine::play2d("songs/ChillDeepHouse.mp3", true, 1);


}

//restart the game upon death or reset
void GameController::restartGame() {
	AudioEngine::stopAll();
	this->removeAllChildren();
	loadLevel(1);
	createFog();
	input->clickProcessed = true;
	destination = 0;
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

		CTypedPtrDblElement<Zombie> *zombie = state->zombies.GetHeadPtr();
		CTypedPtrDblElement<Zombie> *zombToDel = NULL;
		while (!state->zombies.IsSentinel(zombie))
		{
			Zombie *zomb = zombie->Data();
			if (zomb->isDestroyed){
				zomb->isDestroyed = false;
				toDelete = weapon;
				state->world->DestroyBody(zomb->body);
				view->enviornment->removeChild(zomb->sprite);
			}
			zombie = zombie->Next();
		}
		if (zombToDel != NULL){
			state->zombies.Remove(zombToDel);
		}

		if (state->ship->isDestroyed){
			restartGame();
		}

		updateFog();
		float x = state->ship->body->GetPosition().x;
		float y = state->ship->body->GetPosition().y;
		MapNode *from = state->level->locateCharacter(x, y);
		if (!input->clickProcessed){
			//if on beat then flag it
			onBeat = currentSong->isOnBeat(AudioEngine::getCurrentTime(audioid));
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
				MapNode *dest = state->level->locateCharacter((input->lastClick.x - view->screen_size_x / 2.0) + x,
					-(input->lastClick.y - view->screen_size_y / 2.0) + y);
				state->level->shortestPath(from, dest);
				destination = from->next;

				//if it is on beat, decrease the detection radius slightly
				if (detectionRadius > MIN_DETECTION_RADIUS) {
					detectionRadius -= DETECTION_RADIUS_DECREASE;
				}

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
				if (!onBeat && dis < detectionRadius) {
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
		state->ship->getSprite()->setRotation(state->ship->body->GetAngle());
		CTypedPtrDblElement<Zombie> *z = state->zombies.GetHeadPtr();
		while (!state->zombies.IsSentinel(z)){
			pos = z->Data()->body->GetPosition();
			z->Data()->sprite->setPosition(pos.x, pos.y);
			z = z->Next();
		}
	}
	//game is paused, do sth else
	else {


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
    ss << "Coords: (" << (int)coords.x/10 << "," << (int)coords.y/10 << ")";
	view->coordHUD->setString(ss.str());
	stringstream s;
	s << "Speed: " << state->ship->body->GetLinearVelocity().Length();
	view->velHUD->setString(s.str());
	stringstream sss;
	sss << "Click: (" << input->lastClick.x << "," << input->lastClick.y << ")";
	view->thrustHUD->setString(sss.str());
	view->path->clear();
	//clear the old detection circle
	view->detectionRadiusCircle->clear();
	//clear the old hitbox
	view->hitBox->clear();

	stringstream d;
	d << "Detection Radius: " << detectionRadius;
	view->detectionRadiusHUD->setString(d.str());
	
	stringstream awr;
	awr << "Zombie 1 Awarness: " << currAwareness;
	view->zombieOneAwarenessHUD->setString(awr.str());

	//visualize the detection radius
	view->detectionRadiusCircle->drawCircle(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), detectionRadius, 0.0f, 1000, false, ccColor4F(0, 0, 2.0f, 1.0f));

	//visualize the hitbox for main character
	view->hitBox->drawRect(Vec2(state->ship->body->GetPosition().x-30.0f, state->ship->body->GetPosition().y-40.0f), Vec2(state->ship->body->GetPosition().x+30.0f, state->ship->body->GetPosition().y+40.0f), ccColor4F(2.0f, 2.0f, 2.0f, 1.0f));

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

	if (currentSong->isOnBeat(AudioEngine::getCurrentTime(audioid))){
		view->mainBeatHUD->setString("BEAT!");
	}
	else{
		view->mainBeatHUD->setString("");
	}

	/*if (!input->clickProcessed) {
		mouse->drawDot(input->lastClick, 2.0f, ccColor4F(0.0f, 0.0f, 0.0f, 0.0f));
		input->clickProcessed = true;
	}*/
	stringstream st;
	if (onBeat){
		st << "HIT";
	}
	else{
		st << "MISS";
	}
	view->beatHUD->setString(st.str());
	//TODO: Add new HUD for BPM
}

