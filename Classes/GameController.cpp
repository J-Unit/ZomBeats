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
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include "audio/include/AudioEngine.h"

using namespace experimental;
/**
 * Initialize the game state.
 *
 * This is an overridden method inherited from the super(super) class Node.
 * The Director calls this soon after start up.
 */

void GameController::BeginContact(b2Contact* contact){

	//body->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);
}

void GameController::EndContact(b2Contact* contact){
	state->ship->isDestroyed = false; //I turned this off so it won't restart the game, hacky fix oh yeah
	//body->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);
}

bool GameController::init() {
	if (!Layer::init()) {
		return false;
	}
	Director* director = Director::getInstance();
	cocos2d::Size winsize = director->getWinSizeInPixels();
	view = new View(winsize.width, winsize.height);
	view->scene->addChild(this);

	state = new GameState();
	state->world = new b2World(b2Vec2(0.0f, 0.0f));
	state->level = new LevelMap(WORLD_SIZE, WORLD_SIZE);
	destination = 0;

	b2BodyDef b1, b2;
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
	b2b->CreateFixture(&c2, 0.0f);
	//b2b-

    // Build the scene graph and create the ship model.
    view->buildScene(state->level, this);
    state->ship = new Ship(state->world,SPACE_TILE*5.0f,SPACE_TILE*5.0f);
    state->ship->setSprite(view->shipImage);

	// set up the walls here
	// a vertical wall here
	for (int i = 0; i < 20; i++) {
		Wall* new_wall = new Wall(state->world, SPACE_TILE*5.5f, SPACE_TILE*(5.5f+i*0.25f));
		new_wall->setSprite(view->walls[i]);
	}

	// a horizontal wall here
	for (int i = 20; i < 30; i++) {
		Wall* new_wall = new Wall(state->world, SPACE_TILE*(5.5f+(i-19)*0.25f), SPACE_TILE*5.5f);
		new_wall->setSprite(view->walls[i]);
	}

	state->world->SetContactListener(this);

    // Start listening to input
    input = new InputController(_eventDispatcher);
    input->startInput();
    
	elapsedTime = 0.0;
	onBeat = false;

	currentSong = new SongDecomposition(128.0, "../Resources/songs/SimpleBeat2.wav", elapsedTime);
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("../Resources/songs/SimpleBeat2.wav"/*currentSong->trackName.c_str()*/, true);
	int _musicVol = 5;
	static int _backgroundAudioProfile = AudioEngine::INVALID_AUDIO_ID;

	int retval = AudioEngine::play2d("../Resources/songs/SimpleBeat2.mp3", true, 1);
	cout << "THIS IS RETVAL: " << retval << "\n";
	

	//AudioEngine::pause(_backgroundAudioProfile);
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

	if (state->ship->isDestroyed){
		state->world->DestroyBody(state->ship->body);
		delete state->ship;
		state->ship = new Ship(state->world, SPACE_TILE*5.0f, SPACE_TILE*5.0f);
		state->ship->setSprite(view->shipImage);
		input->clickProcessed = true;
		destination = 0;
	}
	float x = state->ship->body->GetPosition().x;
	float y = state->ship->body->GetPosition().y;
	MapNode *from = state->level->locateCharacter(x, y);
	if (!input->clickProcessed){
		//if on beat then flag it
		onBeat = currentSong->isOnBeat(elapsedTime);
		if (!onBeat) {
			state->ship->body->SetLinearVelocity(b2Vec2_zero);
			destination = 0;
		}
		else{
			MapNode *dest = state->level->locateCharacter((input->lastClick.x - view->screen_size_x/2.0) + x, 
				-(input->lastClick.y - view->screen_size_y/2.0) + y);
			state->level->shortestPath(from, dest);
			destination = from->next;
		}
		input->clickProcessed = true;
	}
	if (destination != 0 && from == destination){
			destination = from->next;
	}
	if (destination != 0){
		Vec2 dir = Vec2(state->level->getTileCenterX(destination) - x, state->level->getTileCenterY(destination) - y);
		dir.normalize();
		state->ship->update(deltaTime, dir);
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
	view->shipImage->setRotation(state->ship->body->GetAngle());

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

	if (currentSong->isOnBeat(elapsedTime)){
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

