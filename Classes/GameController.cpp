//
//  GameController.cpp
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#include "GameController.h"
#include "SongDecomposition.h"
#include "ResourceLoader.h"
#include "InputController.h"
#include "FilmStrip.h"
#include "Ship.h"
#include "SimpleAudioEngine.h"
#include <math.h>
#include <iostream>
#include <string>

// Macros to eliminate magic numbers
#define SPACE_TILE   256
#define TILE_AMOUNT	 10
#define PLANET_SCALE 0.5f
#define PARALLAX_AMT 0.0f
#define PLANET1_POS  Vec2(950, 1100)
#define PLANET2_POS  Vec2(1600, 1500)
#define HUD_OFFSET   Vec2(10.0f,10.f)
#define WORLD_SIZE  SPACE_TILE * TILE_AMOUNT

/**
 * Creates a new game with an instance of this controller class.
 *
 * This static controller exists because we do not return the GameController
 * to the caller; we return a Scene that has the GameController as an immediate
 * child. That is because a Scene anchors to the drawing window for display.
 */
Scene* GameController::createScene(int w, int h) {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' (this object) is an autorelease object
    auto layer = GameController::create();
	layer->screen_size_x = w;
	layer->screen_size_y = h;
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

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
	shipModel->isDestroyed = true;
	//body->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);
}

bool GameController::init() {
    if (!Layer::init()) {
        return false;
    }
    
    // Load the resources. This is NOT an asynchronous loader.
    // We would design an asynchronous loader slightly differently.
    ResourceLoader::loadContent();

	world = new b2World(b2Vec2(0.0f, 0.0f));
	level = new LevelMap(WORLD_SIZE, WORLD_SIZE);
	destination = 0;
    // Build the scene graph and create the ship model.
    buildScene();
    shipModel = new Ship(world,SPACE_TILE*5.0f,SPACE_TILE*5.0f);
    shipModel->setSprite(shipImage);

	world->SetContactListener(this);

    // Start listening to input
    input = new InputController(_eventDispatcher);
    input->startInput();
    
	elapsedTime = 0.0;
	onBeat = false;

	currentSong = new SongDecomposition(128.0, "../Resources/songs/SimpleBeat2.wav", elapsedTime);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("../Resources/songs/SimpleBeat2.wav"/*currentSong->trackName.c_str()*/, true);
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

	if (shipModel->isDestroyed){
		world->DestroyBody(shipModel->body);
		delete shipModel;
		shipModel = new Ship(world, SPACE_TILE*5.0f, SPACE_TILE*5.0f);
		shipModel->setSprite(shipImage);
		input->clickProcessed = true;
		destination = 0;
	}
	float x = shipModel->body->GetPosition().x;
	float y = shipModel->body->GetPosition().y;
	MapNode *from = level->locateCharacter(x, y);
	if (!input->clickProcessed){
		//if on beat then flag it
		onBeat = currentSong->isOnBeat(elapsedTime);
		if (!onBeat) {
			shipModel->body->SetLinearVelocity(b2Vec2_zero);
			destination = 0;
		}
		else{
			MapNode *dest = level->locateCharacter((input->lastClick.x - screen_size_x/2.0) + x, 
				-(input->lastClick.y - screen_size_y/2.0) + y);
			level->shortestPath(from, dest);
			destination = from->next;
		}
		input->clickProcessed = true;
	}
	if (destination != 0 && from == destination){
			destination = from->next;
	}
	if (destination != 0){
		Vec2 dir = Vec2(level->getTileCenterX(destination) - x, level->getTileCenterY(destination) - y);
		dir.normalize();
		shipModel->update(deltaTime, dir);
	}




    // Move the ship (MODEL ONLY)
    //shipModel->setForward(thrust.y);
    //shipModel->setTurning(thrust.x);
    //shipModel->update(deltaTime, thrust);
	world->Step(deltaTime, 8, 3);
	//world->ClearForces();
	
    
    // "Drawing" code.  Move everything BUT the ship
    // Update the HUD
    displayPosition(coordHUD, shipModel->body->GetPosition());

	b2Vec2 pos = shipModel->body->GetPosition();
	Vec2 offset = Vec2(pos.x, pos.y);// -enviornment->getPosition();
    Vec2 center(0.5f,0.5f);
    
    // Anchor points are in texture coordinates (0 to 1). Scale it.
    offset.x = offset.x/allSpace->getContentSize().width;
    offset.y = offset.y/allSpace->getContentSize().height;

    // Reanchor the node at the center of the screen and rotate about center.
    //farSpace->setAnchorPoint(offset*PARALLAX_AMT+center);
    //farSpace->setRotation(-shipModel->body->GetAngle());
	//shipImage->setAnchorPoint(-offset);
	enviornment->setAnchorPoint(offset);
	// Reanchor the node at the center of the screen and rotate about center.
    //nearSpace->setAnchorPoint(offset+center);
    //nearSpace->setRotation(-shipModel->body->GetAngle());
	shipImage->setRotation(shipModel->body->GetAngle());

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
    coordHUD->setString(ss.str());
	stringstream s;
	s << "Speed: " << shipModel->body->GetLinearVelocity().Length();
	velHUD->setString(s.str());
	stringstream sss;
	sss << "Click: (" << input->lastClick.x << "," << input->lastClick.y << ")";
	thrustHUD->setString(sss.str());
	path->clear();
	if (destination != 0){
		MapNode *last = level->locateCharacter(shipModel->body->GetPosition().x, shipModel->body->GetPosition().y);
		MapNode *cur = destination;
		do {
			float x1 = level->getTileCenterX(last);
			float y1 = level->getTileCenterY(last);
			float x2 = level->getTileCenterX(cur);
			float y2 = level->getTileCenterY(cur);
			path->drawLine(Vec2(x1, y1), Vec2(x2, y2), ccColor4F(0,0,0,1.0f));
			last = cur;
			cur = cur->next;
		} while (cur != 0);
		
	}

	if (currentSong->isOnBeat(elapsedTime)){
		mainBeatHUD->setString("BEAT!");
	}
	else{
		mainBeatHUD->setString("");
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
	beatHUD->setString(st.str());
}

/**
 * Builds the scene graph for the game.
 *
 * We factored this out of init() to allow us to page-in and page-out
 * the scene graph at a later time.
 */
void GameController::buildScene() {
    Size dimen  = Director::getInstance()->getVisibleSize();
	pix_to_opengl_scale = dimen.width / screen_size_x;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center(origin.x+dimen.width/2.0f,origin.y+dimen.height/2.0f);
    Vec2 anchor(0.5f,0.5f);
    
    // Create the master node.  This, unlike the layer, is resolution independent.
    // If we do not do this, the children will not all line up correctly on different devices.
    allSpace = Node::create();
    allSpace->setContentSize(Size(SPACE_TILE*TILE_AMOUNT,SPACE_TILE*TILE_AMOUNT));
    allSpace->setPosition(center);
    allSpace->setAnchorPoint(anchor);
    
    // Everything else is relative to all space, not the screen!
    center.set(allSpace->getContentSize().width/2.0f,allSpace->getContentSize().height/2.0f);
    background = Node::create();
	background->setContentSize(allSpace->getContentSize());
	background->setPosition(center);
    background->setAnchorPoint(anchor);

	enviornment = Node::create();
	enviornment->setContentSize(allSpace->getContentSize());
	enviornment->setPosition(center);
	enviornment->setAnchorPoint(anchor);
	enviornment->addChild(background);


	meshVis = DrawNode::create();
	meshVis->setContentSize(allSpace->getContentSize());
	meshVis->setPosition(center);
	meshVis->setAnchorPoint(anchor);
	for (int i = 0; i < BLOCKS_X; i++) for (int j = 0; j < BLOCKS_Y; j++){
		MapNode *n = &(level->mesh[i][j]);
		Vec2 loc = Vec2(level->getTileCenterX(n), level->getTileCenterY(n));
		meshVis->drawPoint(loc, 7.0f, cocos2d::ccColor4F(0, 0, 0, 0.7f));
	}
	path = DrawNode::create();
	path->setContentSize(allSpace->getContentSize());
	path->setPosition(center);
	path->setAnchorPoint(anchor);
    
    // Tile the background with deep space
    Vec2 rivet;
    Texture2D* bkgd = ResourceLoader::getInstance()->getTexture("space");
    for(int ii = -TILE_AMOUNT/2; ii <= TILE_AMOUNT/2; ii++) {
        for(int jj = -TILE_AMOUNT/2; jj <= TILE_AMOUNT/2; jj++) {
            // Create a new autorelease sprite for each tile
            auto tile = Sprite::createWithTexture(bkgd);
            rivet.x = background->getContentSize().width/2.0f+ii*SPACE_TILE;
            rivet.y = background->getContentSize().height/2.0f+jj*SPACE_TILE;
            tile->setPosition(rivet);
            tile->setAnchorPoint(anchor);
            background->addChild(tile);
        }
    }
	enviornment->addChild(meshVis);
	enviornment->addChild(path);
    // Put planets in the foreground.
    /*nearSpace = Node::create();
    nearSpace->setContentSize(allSpace->getContentSize());
    nearSpace->setPosition(center);
    nearSpace->setAnchorPoint(anchor);*/
    
    Sprite* planet = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("planet1"));
    planet->setScale(PLANET_SCALE,PLANET_SCALE);
    planet->setPosition(PLANET1_POS);
    planet->setAnchorPoint(anchor);
	enviornment->addChild(planet);
   // nearSpace->addChild(planet);
	b2BodyDef b1, b2;
	b1.position.Set(PLANET1_POS.x, PLANET1_POS.y);
	b2.position.Set(PLANET2_POS.x, PLANET2_POS.y);
	b2Body* b1b;
	b2Body* b2b;
	b1b = world->CreateBody(&b1);
	b2b = world->CreateBody(&b2);
	b2CircleShape c1, c2;
	c1.m_radius = 60;
	c2.m_radius = 60;
	b1b->CreateFixture(&c1, 0.0f);
	b2b->CreateFixture(&c2, 0.0f);
	//b2b-

    
    planet = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("planet2"));
    planet->setScale(PLANET_SCALE,PLANET_SCALE);
    planet->setPosition(PLANET2_POS);
    planet->setAnchorPoint(anchor);
    enviornment->addChild(planet);
    
    //shipImage = FilmStrip::create(ResourceLoader::getInstance()->getTexture("ship"),4,5,18);
	shipImage = FilmStrip::create(ResourceLoader::getInstance()->getTexture("ship"), 1, 1, 1);

    coordHUD = Label::create();
    coordHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
    coordHUD->setPosition(HUD_OFFSET);
    coordHUD->setAnchorPoint(Vec2::ZERO);

	velHUD = Label::create();
	velHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	velHUD->setPosition(Vec2(HUD_OFFSET.x*20, HUD_OFFSET.y));
	velHUD->setAnchorPoint(Vec2::ZERO);

	thrustHUD = Label::create();
	thrustHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	thrustHUD->setPosition(Vec2(HUD_OFFSET.x * 40, HUD_OFFSET.y));
	thrustHUD->setAnchorPoint(Vec2::ZERO);

	beatHUD = Label::create();
	beatHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	beatHUD->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y*40));
	beatHUD->setScale(2.0f);
	beatHUD->setAnchorPoint(Vec2::ZERO);

	mainBeatHUD = Label::create();
	mainBeatHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	mainBeatHUD->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 50));
	mainBeatHUD->setScale(1.6f);
	mainBeatHUD->setAnchorPoint(Vec2::ZERO);
    
    // Remove the welcome screen and display the game.
    allSpace->addChild(enviornment,0);
    //allSpace->addChild(nearSpace,0.5);
    allSpace->addChild(shipImage,1);
    this->addChild(allSpace);
    this->addChild(coordHUD);  // On top of scene graph.
	this->addChild(velHUD);
	this->addChild(thrustHUD);
	this->addChild(beatHUD);
	this->addChild(mainBeatHUD);
}

