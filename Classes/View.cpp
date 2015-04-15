#include "View.h"
#include "LevelMap.h"
#include "Box2D/Box2D.h"
#include "ResourceLoader.h"
#include "FilmStrip.h"
#include <string>
#include <math.h>
#include <iostream>
#include "SongDecomposition.h"
#include "Wall.h"
#include "util.h"

View::View(int w, int h){
	screen_size_x = w;
	screen_size_y = h;
	ResourceLoader::loadContent();
	meshVis = path = detectionRadiusCircle = directionUseEnvironmentWeapon = meter = durabilityBox = durabilityHolder = durabilitySpriteContainer = hitBox = weaponBox = ai = zombiePositions = NULL;
	grooviness = durability = objective = NULL; 
	enviornment = zombies = shipImage = NULL;
	// Load the resources. This is NOT an asynchronous loader.
	// We would design an asynchronous loader slightly differently.
	scene = createScene();
}
/**
* Creates a new game with an instance of this controller class.
*
* This static controller exists because we do not return the GameController
* to the caller; we return a Scene that has the GameController as an immediate
* child. That is because a Scene anchors to the drawing window for display.
*/
Scene* View::createScene() {
	// 'scene' is an autorelease object
	scene = Scene::create();
	// return the scene
	return scene;
}

void View::makeResolutionIndependent(Node *n){
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center(size.width / 2.0f + origin.x, size.height / 2.0f + origin.y);
	float aspect = size.width / (float)size.height;
	float width = VIEW_WDITH;
	float height = VIEW_HEIGHT;

	// Masks the hidden areas if aspect ratio is not a match.
	if (aspect > width / height) {
		// Time for horizontal bars.
		n->setScale(size.height / height);

		Size band((size.width - width*size.height / height) / 2.0f, size.height);
		Vec2 offset(band.width, band.height);

		if (band.width > LETTERBOX_EPSILON && band.height > LETTERBOX_EPSILON) {
			auto left = DrawNode::create();
			left->setContentSize(band);
			left->drawSolidRect(Vec2::ZERO, offset, Color4F(0, 0, 0, 1));
			left->setAnchorPoint(Vec2(0, 0));
			left->setPosition(origin);
			this->addChild(left, 1);

			auto rght = DrawNode::create();
			rght->setContentSize(band);
			rght->drawSolidRect(Vec2::ZERO, offset, Color4F(0, 0, 0, 1));
			rght->setAnchorPoint(Vec2(1, 1));
			rght->setPosition(origin + size);
			this->addChild(rght, 1);
		}
	}
	else if (aspect < width / height) {
		// Time for vertical bars.
		n->setScale(size.width / width);

		Size band(size.width, (size.height - height*size.width / width) / 2.0f);
		Vec2 offset(band.width, band.height);

		if (band.width > LETTERBOX_EPSILON && band.height > LETTERBOX_EPSILON) {
			auto bot = DrawNode::create();
			bot->setContentSize(band);
			bot->drawSolidRect(Vec2::ZERO, offset, Color4F(0, 0, 0, 1));
			bot->setAnchorPoint(Vec2(0, 0));
			bot->setPosition(origin);
			this->addChild(bot, 1);

			auto top = DrawNode::create();
			top->setContentSize(band);
			top->drawSolidRect(Vec2::ZERO, offset, Color4F(0, 0, 0, 1));
			top->setAnchorPoint(Vec2(1, 1));
			top->setPosition(origin + size);
			this->addChild(top, 2);
		}
	}
	else {
		n->setScale(size.width / width);
	}
}
#define RELEASE(p) if(p!=NULL) p->release();
void View::releaseScene(){
	RELEASE(path);
	RELEASE(detectionRadiusCircle);
	RELEASE(directionUseEnvironmentWeapon);
	RELEASE(hitBox);
	RELEASE(weaponBox);
	RELEASE(ai);
	RELEASE(zombiePositions);
	RELEASE(grooviness);
	RELEASE(durability);
	RELEASE(durabilityBox);
	RELEASE(meter);
	RELEASE(durabilityHolder);
	RELEASE(durabilitySpriteContainer);
	RELEASE(objective);
	RELEASE(enviornment);
	RELEASE(zombies);
	RELEASE(shipImage);
}

/**
* Builds the scene graph for the game.
*
* We factored this out of init() to allow us to page-in and page-out
* the scene graph at a later time.
*/
void View::buildScene(LevelMap *level, Layer* l, int levNum) {
	Size dimen = Director::getInstance()->getVisibleSize();
	pix_to_opengl_scale = dimen.width / screen_size_x;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	shakeCenter = Vec2(origin.x + dimen.width / 2.0f, origin.y + dimen.height / 2.0f);
	Vec2 anchor(0.5f, 0.5f);

	resIndepScreen = Node::create();
	resIndepScreen->setPosition(Vec2::ZERO);
	makeResolutionIndependent(resIndepScreen);
	shakeCenter = shakeCenter / resIndepScreen->getScale();
	// Create the master node.  This, unlike the layer, is resolution independent.
	// If we do not do this, the children will not all line up correctly on different devices.
	allSpace = Node::create();
	allSpace->setContentSize(Size(SPACE_TILE*level->bkgTilesX, SPACE_TILE*level->bkgTilesY));
	allSpace->setPosition(shakeCenter);
	allSpace->setAnchorPoint(anchor);


	// Everything else is relative to all space, not the screen!
	Vec2 center(allSpace->getContentSize().width / 2.0f, allSpace->getContentSize().height / 2.0f);
	background = Node::create();
	background->setContentSize(allSpace->getContentSize());
	background->setPosition(center);
	background->setAnchorPoint(anchor);

	enviornment = Node::create();
	enviornment->setContentSize(allSpace->getContentSize());
	enviornment->setPosition(center);
	enviornment->setAnchorPoint(anchor);
	enviornment->addChild(background);


	/*meshVis = DrawNode::create();
	meshVis->setContentSize(allSpace->getContentSize());
	meshVis->setPosition(center);
	meshVis->setAnchorPoint(anchor);*/
	/*for (int i = 0; i < BLOCKS_X; i++) for (int j = 0; j < BLOCKS_Y; j++){
		MapNode *n = &(level->mesh[i][j]);
		if (n->walkable){
			Vec2 loc = Vec2(level->getTileCenterX(n), level->getTileCenterY(n));
			meshVis->drawPoint(loc, 7.0f, cocos2d::ccColor4F(0, 0, 0, 0.7f));
		}
	}*/
	path = DrawNode::create();
	path->setContentSize(allSpace->getContentSize());
	path->setPosition(center);
	path->setAnchorPoint(anchor);

	ai = DrawNode::create();
	ai->setContentSize(allSpace->getContentSize());
	ai->setPosition(center);
	ai->setAnchorPoint(anchor);

	zombiePositions = DrawNode::create();
	zombiePositions->setContentSize(allSpace->getContentSize());
	zombiePositions->setPosition(center);
	zombiePositions->setAnchorPoint(anchor);

	detectionRadiusCircle = DrawNode::create();
	detectionRadiusCircle->setContentSize(allSpace->getContentSize());
	detectionRadiusCircle->setPosition(center);
	detectionRadiusCircle->setAnchorPoint(anchor);
	detectionRadiusCircle->setVisible(false);

	directionUseEnvironmentWeapon = DrawNode::create();
	directionUseEnvironmentWeapon->setContentSize(allSpace->getContentSize());	
	directionUseEnvironmentWeapon->setPosition(center);
	directionUseEnvironmentWeapon->setAnchorPoint(anchor);

	hitBox = DrawNode::create();
	hitBox->setContentSize(allSpace->getContentSize());
	hitBox->setPosition(center);
	hitBox->setAnchorPoint(anchor);

	weaponBox = DrawNode::create();
	weaponBox->setContentSize(allSpace->getContentSize());
	weaponBox->setPosition(center);
	weaponBox->setAnchorPoint(anchor);

	// Tile the background with deep space
	Vec2 rivet;
	Texture2D* bkgd = ResourceLoader::getInstance()->getTexture("tile1");
	for (int ii = -level->bkgTilesX / 2 + 1; ii <= level->bkgTilesX / 2; ii++) {
		for (int jj = -level->bkgTilesY / 2 + 1; jj <= level->bkgTilesY / 2; jj++) {
			// Create a new autorelease sprite for each tile
			auto tile = Sprite::createWithTexture(bkgd);
			rivet.x = background->getContentSize().width / 2.0f + ii*SPACE_TILE;
			rivet.y = background->getContentSize().height / 2.0f + jj*SPACE_TILE;
			tile->setPosition(rivet);
			tile->setAnchorPoint(Vec2::ONE);
			background->addChild(tile);
		}
	}

	zombies = Node::create();

	//enviornment->addChild(meshVis);
	enviornment->addChild(path);
	enviornment->addChild(ai, 1);
	enviornment->addChild(zombiePositions, 4); //z-order to zombiePositions should be larger than fog's 
	enviornment->addChild(detectionRadiusCircle);
	enviornment->addChild(hitBox, 3);
	enviornment->addChild(weaponBox);

	enviornment->addChild(directionUseEnvironmentWeapon);
	enviornment->addChild(zombies);
	// Put planets in the foreground.
	/*nearSpace = Node::create();
	nearSpace->setContentSize(allSpace->getContentSize());
	nearSpace->setPosition(center);
	nearSpace->setAnchorPoint(anchor);*/

	/*Sprite* planet = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("planet1"));
	planet->setScale(PLANET_SCALE, PLANET_SCALE);
	planet->setPosition(PLANET1_POS);
	planet->setAnchorPoint(anchor);
	enviornment->addChild(planet);
	// nearSpace->addChild(planet);


	planet = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("planet2"));
	planet->setScale(PLANET_SCALE, PLANET_SCALE);
	planet->setPosition(PLANET2_POS);
	planet->setAnchorPoint(anchor);
	enviornment->addChild(planet);*/

	//shipImage = FilmStrip::create(ResourceLoader::getInstance()->getTexture("ship"),4,5,18);
	/*shipImage = FilmStrip::create(ResourceLoader::getInstance()->getTexture("ship"), 1, 1, 1);

	//create 30 wall bricks 
	for (int i = 0; i < 30; i++) {
		Sprite* n_wallTile = );
		walls.push_back(n_wallTile);
		enviornment->addChild(n_wallTile);
	}*/


	/*coordHUD = Label::create();
	coordHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	coordHUD->setPosition(HUD_OFFSET);
	coordHUD->setAnchorPoint(Vec2::ZERO);

	velHUD = Label::create();
	velHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	velHUD->setPosition(Vec2(HUD_OFFSET.x * 20, HUD_OFFSET.y));
	velHUD->setAnchorPoint(Vec2::ZERO);

	thrustHUD = Label::create();
	thrustHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	thrustHUD->setPosition(Vec2(HUD_OFFSET.x * 40, HUD_OFFSET.y));
	thrustHUD->setAnchorPoint(Vec2::ZERO);

    detectionRadiusHUD = Label::create();
	detectionRadiusHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	detectionRadiusHUD->setPosition(Vec2(HUD_OFFSET.x * 60, HUD_OFFSET.y));
	detectionRadiusHUD->setAnchorPoint(Vec2::ZERO);

	zombieOneAwarenessHUD = Label::create();
	zombieOneAwarenessHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	zombieOneAwarenessHUD->setPosition(Vec2(HUD_OFFSET.x * 60, HUD_OFFSET.y*5));
	zombieOneAwarenessHUD->setAnchorPoint(Vec2::ZERO);*/

	objective = Label::create();
	objective->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	objective->setPosition(Vec2(HUD_OFFSET.x*13, HUD_OFFSET.y * 57));
	objective->setAnchorPoint(anchor);

	beatHUD = Label::create();
	beatHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	beatHUD->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 52));
	beatHUD->setScale(1.8f);
	beatHUD->setAnchorPoint(Vec2::ZERO);

	mainBeatHUD = Label::create();
	mainBeatHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	mainBeatHUD->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 57));
	mainBeatHUD->setScale(1.4f);
	mainBeatHUD->setAnchorPoint(Vec2::ZERO);

	grooviness = Label::create();
	grooviness->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	grooviness->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 42));
	grooviness->setAnchorPoint(Vec2::ZERO);
	
	meter = DrawNode::create();
	meter->setPosition(HUD_OFFSET.x * 2.0f, HUD_OFFSET.y * 23);
	meter->setContentSize(allSpace->getContentSize());
	meter->setAnchorPoint(Vec2::ZERO);

	//durability
	durability = Label::create();
	durability->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	durability->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 18.5));
	durability->setAnchorPoint(Vec2::ZERO);

	durabilityBox = DrawNode::create();
	durabilityBox->setPosition(HUD_OFFSET.x, HUD_OFFSET.y * 18);
	durabilityBox->setContentSize(allSpace->getContentSize());
	durabilityBox->setAnchorPoint(Vec2::ZERO);

	durabilityHolder = DrawNode::create();
	durabilityHolder->setPosition(0.0f,0.0f);
	durabilityHolder->setContentSize(allSpace->getContentSize());
	durabilityHolder->setAnchorPoint(Vec2::ZERO);

	durabilitySpriteContainer = DrawNode::create();
	durabilitySpriteContainer->setPosition(0.0f, 0.0f);
	durabilitySpriteContainer->setContentSize(allSpace->getContentSize());
	durabilitySpriteContainer->setAnchorPoint(Vec2::ZERO);
	//durabilitySpriteContainer->setScale(0.25f);


	//a fake pause button goes here
	Sprite* pause = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("pause_button"));
	Size visibleSize2 = Director::getInstance()->getVisibleSize();
	Vec2 origin2 = Director::getInstance()->getVisibleOrigin();
	pause->setScale(0.2f);
	pause->setPosition(Point(visibleSize2.width*0.8 + origin2.x, visibleSize2.height*0.94 + origin2.y));

	//a pause menu goes here
	//pauseMenu->setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("pause_menu")));
	//pauseMenu->sprite->setScale(0.15f);
	//pauseMenu->sprite->setPosition(Vec2(HUD_OFFSET.x * 50, HUD_OFFSET.y * 35));

	// Remove the welcome screen and display the game.
	l->addChild(resIndepScreen);
	resIndepScreen->addChild(allSpace);
	allSpace->addChild(enviornment, 0);
	//allSpace->addChild(nearSpace,0.5);
	//allSpace->addChild(shipImage, 1);
	resIndepScreen->addChild(objective);

	//l->addChild(coordHUD);  // On top of scene graph.
	//l->addChild(velHUD);
	//l->addChild(thrustHUD);
	resIndepScreen->addChild(beatHUD);
	resIndepScreen->addChild(mainBeatHUD);
	resIndepScreen->addChild(grooviness);
	resIndepScreen->addChild(durability);
	resIndepScreen->addChild(meter);
	resIndepScreen->addChild(durabilityBox);
	durabilityBox->addChild(durabilityHolder);
	durabilityBox->addChild(durabilitySpriteContainer);

	//l->addChild(detectionRadiusHUD);
	//l->addChild(zombieOneAwarenessHUD); //remove this later
}

void View::shake(float start, float now, Vec2 dir){
	float v = sin((now - start) * 2 * M_PI / ERROR_WINDOW);
	allSpace->setPosition(shakeCenter + v * SHAKE_STRENGTH * dir);
}



View::~View()
{
}
