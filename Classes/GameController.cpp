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
	state->zombies.AddTail(z1);
	state->zombies.AddTail(z2);

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
	Wall *new_wall;
	// set up the walls here
	//we can move the below code to level editor later so it looks clean
	//-------------------------------------------------------------------------------------
	// a vertical wall here
	for (int i = 0; i < 20; i++) {
		new_wall = new Wall(state->world, SPACE_TILE*5.5f, SPACE_TILE*(5.5f + i*0.25f));
		view->enviornment->addChild(new_wall->sprite);
		//new_wall->setSprite(view->walls[i]);
	}

	// a horizontal wall here
	for (int i = 20; i < 30; i++) {
		new_wall = new Wall(state->world, SPACE_TILE*(5.5f + (i - 19)*0.25f), SPACE_TILE*5.5f);
		view->enviornment->addChild(new_wall->sprite);
		//new_wall->setSprite(view->walls[i]);
	}
}

void GameController::createWeapons(){
	//Sword *s1 = new Sword(state->world, SPACE_TILE*5.9f, SPACE_TILE*(5.5f + 1*0.25f)); //this makes it corner inside of wall
	Sword *s1 = new Sword(state->world, SPACE_TILE*9.0f, SPACE_TILE*(5.5f + 1 * 0.25f));
	state->weapons.AddTail(s1);
	view->enviornment->addChild(s1->sprite);
}



bool GameController::init() {
	if (!Layer::init()) {
		return false;
	}

	state = new GameState();
	state->world = new b2World(b2Vec2(0.0f, 0.0f));
	state->level = new LevelMap(WORLD_SIZE, WORLD_SIZE);
	destination = 0;
	Director* director = Director::getInstance();
	cocos2d::Size winsize = director->getWinSizeInPixels();
	view = new View(winsize.width, winsize.height);
	view->scene->addChild(this);

    // Build the scene graph and create the ship model.
    view->buildScene(state->level, this);
    state->ship = new Ship(state->world,SPACE_TILE*5.0f,SPACE_TILE*5.0f);
	view->allSpace->addChild(state->ship->getSprite());
	ai = new AIController();

	createZombies();
	createWalls();
	createWeapons();

	state->world->SetContactListener(this);

    // Start listening to input
    input = new InputController(_eventDispatcher);
    input->startInput();
    
	elapsedTime = 0.0;
	onBeat = false;

	//initial detection radius
	detectionRadius = INITIAL_DETECTION_RADIUS;
	currAwareness = 0.0f;

	currentSong = new SongDecomposition(120.0, "../Resources/songs/ChillDeepHouse.mp3", -.051);
	audioid = AudioEngine::play2d("../Resources/songs/ChillDeepHouse.mp3", true, 1);

	// Tell the director we are ready for animation.
    this->scheduleUpdate();
    return true;
}

/**
 * Update the game state.
 *
 * This method is called every animation frame.  There is no draw() or render()
 * counterpoint to this method; drawing is done automatically in the scene graph.
 * However, this method is responsible for updating any transforms in the scene graph.
 */
void GameController::update(float deltaTime) {
    // Read the thrust from the user input
    //input->update();
	Vec2 thrust = input->lastClick;
	elapsedTime += deltaTime; //a float in seconds

	//cout << "Elapsed time: "<< elapsedTime <<endl;
	// Read the thrust from the user input
	input->update();
	bool clicked = input->didClick();

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
		state->world->DestroyBody(state->ship->body);
		delete state->ship;
		state->ship = new Ship(state->world, SPACE_TILE*5.0f, SPACE_TILE*5.0f);
		input->clickProcessed = true;
		destination = 0;
	}


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
			if (detectionRadius < 350.0f) {
				detectionRadius += DETECTION_RADIUS_INCREASE;
			}
		}
		else{
			MapNode *dest = state->level->locateCharacter((input->lastClick.x - view->screen_size_x/2.0) + x, 
				-(input->lastClick.y - view->screen_size_y/2.0) + y);
			state->level->shortestPath(from, dest);
			destination = from->next;

			//if it is on beat, decrease the detection radius slightly
			if (detectionRadius > 50.0f) {
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
		state->ship->update(deltaTime, dir);
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
    Vec2 center(0.5f,0.5f);
    
    // Anchor points are in texture coordinates (0 to 1). Scale it.
    offset.x = offset.x/view->allSpace->getContentSize().width;
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
	pos = z->Data()->body->GetPosition();
	while (!state->zombies.IsSentinel(z)){
		z->Data()->sprite->setPosition(pos.x, pos.y);
		z = z->Next();
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

	stringstream d;
	d << "Detection Radius: " << detectionRadius;
	view->detectionRadiusHUD->setString(d.str());
	
	stringstream awr;
	awr << "Zombie 1 Awarness: " << currAwareness;
	view->zombieOneAwarenessHUD->setString(awr.str());

	//visualize the detection radius
	view->detectionRadiusCircle->drawCircle(Vec2(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y), detectionRadius, 0.0f, 1000, false, ccColor4F(0, 0, 2.0f, 1.0f));

	if (destination != 0){
		MapNode *last = state->level->locateCharacter(state->ship->body->GetPosition().x, state->ship->body->GetPosition().y);
		MapNode *cur = destination;
		do {
			float x1 = state->level->getTileCenterX(last);
			float y1 = state->level->getTileCenterY(last);
			float x2 = state->level->getTileCenterX(cur);
			float y2 = state->level->getTileCenterY(cur);
			view->path->drawLine(Vec2(x1, y1), Vec2(x2, y2), ccColor4F(0, 0, 0, 1.0f));
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

