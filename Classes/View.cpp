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

#define VIEW_SCALE 0.70

float View::resIndepScale = 1.0f;

View::View(int w, int h){
	screen_size_x = w;
	screen_size_y = h;
	meshVis = path = detectionRadiusCircle = directionUseEnvironmentWeapon = meter = durabilityBox = durabilityHolder = durabilitySpriteContainer = hitBox = weaponBox = ai = zombiePositions = NULL;
	grooviness = durability = objective = collectionGoal = NULL;
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
	if(resIndepScale == 1.0f) resIndepScale = n->getScale();
}

#define RELEASE(p) if(p!=NULL) p->release();
void View::releaseScene(){
	/*RELEASE(path);
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
	RELEASE(hitBox);*/
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

	weaponPositions = DrawNode::create();
	weaponPositions->setContentSize(allSpace->getContentSize());
	weaponPositions->setPosition(center);
	weaponPositions->setAnchorPoint(anchor);

	zombiePositions = DrawNode::create();
	zombiePositions->setContentSize(allSpace->getContentSize());
	zombiePositions->setPosition(center);
	zombiePositions->setAnchorPoint(anchor);

	detectionRadiusCircle = DrawNode::create();
	//detectionRadiusCircle->setContentSize(allSpace->getContentSize());
	//detectionRadiusCircle->setPosition(center);
	//detectionRadiusCircle->setAnchorPoint(anchor);
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
	walls = Node::create();

	//enviornment->addChild(meshVis);
	enviornment->addChild(path);
	enviornment->addChild(ai, 10);
	enviornment->addChild(weaponPositions, 10);
	enviornment->addChild(zombiePositions, 4); //z-order to zombiePositions should be larger than fog's 
	enviornment->addChild(detectionRadiusCircle);
	enviornment->addChild(hitBox, 3);
	enviornment->addChild(weaponBox,5);

	enviornment->addChild(directionUseEnvironmentWeapon);
	enviornment->addChild(zombies, 2);
	enviornment->addChild(walls, 2);
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

	collectionGoal = Label::create();
	//put the objective label in calibration level to lower bottom 
	collectionGoal->setTTFConfig(*ResourceLoader::getInstance()->getFont("NewFont"));
	//objective->setPosition(Point(visibleSizeLabel.width/2 - 20, visibleSizeLabel.height / 7));
	collectionGoal->setPosition(Vec2(HUD_OFFSET.x*44.5f, HUD_OFFSET.y * 7.5f));
	collectionGoal->setAnchorPoint(anchor);

	objective = Label::create();
	//put the objective label in calibration level to lower bottom 
	objective->setTTFConfig(*ResourceLoader::getInstance()->getFont("NewFont"));
	//Size visibleSizeLabel = Director::getInstance()->getVisibleSize();
	//Vec2 originLabel = Director::getInstance()->getVisibleOrigin();
	//objective->setPosition(Point(visibleSizeLabel.width/2 - 20, visibleSizeLabel.height / 7));
	objective->setPosition(Vec2(HUD_OFFSET.x*44.5f, HUD_OFFSET.y * 10));
	objective->setAnchorPoint(anchor);

	beatHUD = Label::create();
	beatHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	beatHUD->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 50));
	beatHUD->setScale(1.8f);
	beatHUD->setAnchorPoint(Vec2::ZERO);

	mainBeatHUD = Label::create();
	mainBeatHUD->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	mainBeatHUD->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 56));
	mainBeatHUD->setScale(1.4f);
	mainBeatHUD->setAnchorPoint(Vec2::ZERO);

	grooviness = Label::create();
	grooviness->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	grooviness->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 42));
	grooviness->setAnchorPoint(Vec2::ZERO);

	beatSprite = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("hudBeat"));
	beatSprite->setPosition(Vec2(0, HUD_OFFSET.y * 49.5));
	beatSprite->setVisible(false);
	beatSprite->setAnchorPoint(Vec2::ZERO);
	beatSprite->setScale(0.3f);
	hitSprite = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("hudHit"));
	hitSprite->setPosition(Vec2(2, HUD_OFFSET.y * 42.5));
	hitSprite->setVisible(false);
	hitSprite->setAnchorPoint(Vec2::ZERO);
	hitSprite->setScale(0.3f);
	missSprite = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("hudMiss"));
	missSprite->setPosition(Vec2(0, HUD_OFFSET.y * 42.5));
	missSprite->setVisible(false);
	missSprite->setScale(0.3f);
	missSprite->setAnchorPoint(Vec2::ZERO);

	
	meter = DrawNode::create();
	meter->setPosition(HUD_OFFSET.x * 4.0f, HUD_OFFSET.y * 23);
	meter->setContentSize(allSpace->getContentSize());
	meter->setAnchorPoint(Vec2::ZERO);

	//durability
	durability = Label::create();
	durability->setTTFConfig(*ResourceLoader::getInstance()->getFont("MarkerFelt"));
	durability->setPosition(Vec2(HUD_OFFSET.x, HUD_OFFSET.y * 18.5));
	durability->setAnchorPoint(Vec2::ZERO);
	durability->setString("Durability");

	durabilityBox = DrawNode::create();
	durabilityBox->setPosition(HUD_OFFSET.x, HUD_OFFSET.y * 18);
	durabilityBox->setContentSize(allSpace->getContentSize());
	durabilityBox->setAnchorPoint(Vec2::ZERO);
	durabilityBox->setScale(0.8f);

	durabilityHolder = DrawNode::create();
	durabilityHolder->setPosition(0.0f,0.0f);
	durabilityHolder->setContentSize(allSpace->getContentSize());
	durabilityHolder->setAnchorPoint(Vec2::ZERO);
	durabilityHolder->drawSolidRect(Vec2(0, 0), Vec2(110, -150), ccColor4F(0.0f, 0.0f, 0.0f, 1.0f));
	durabilityHolder->drawRect(Vec2(0, 0), Vec2(110, -150), ccColor4F(0.5f, 0.5f, 0.5f, 1.0f));


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
	resIndepScreen->addChild(objective, 5);
	resIndepScreen->addChild(collectionGoal, 5);

	//l->addChild(coordHUD);  // On top of scene graph.
	//l->addChild(velHUD);
	//l->addChild(thrustHUD);
	resIndepScreen->addChild(beatHUD);
	resIndepScreen->addChild(beatSprite);
	resIndepScreen->addChild(hitSprite);
	resIndepScreen->addChild(missSprite);
	resIndepScreen->addChild(mainBeatHUD);
	resIndepScreen->addChild(grooviness);
	resIndepScreen->addChild(durability);
	resIndepScreen->addChild(meter);
	resIndepScreen->addChild(durabilityBox);
	durabilityBox->addChild(durabilityHolder);
	durabilityBox->addChild(durabilitySpriteContainer);
	allSpace->setScale(VIEW_SCALE);
	//l->addChild(detectionRadiusHUD);
	//l->addChild(zombieOneAwarenessHUD); //remove this later
}

void View::redrawDurability(int dur){
	durabilitySpriteContainer->removeAllChildrenWithCleanup(true);
	for (int i = 0; i < 3 && dur>0; ++i){
		for (int j = 0; j < 2 && dur>0; ++j, dur--){
			//draw the durability circle
			Sprite* new_bullet = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("bullet"));
			new_bullet->setPosition(Vec2(25.0f + j*50.0f, -(25.0f + i*50.0f)));
			new_bullet->setScale(0.3f);
			durabilitySpriteContainer->addChild(new_bullet);
			//durabilitySpriteContainer->drawSolidCircle(Vec2(25.0f + j*50.0f, -(25.0f + i*50.0f)), 8.0f, 0.0f, 20.0f, ccColor4F(0.5f, 0, 0, 1.0f));
		}
	}
}

void View::drawGroove(){
	meter->drawRect(Vec2(-22, 0), Vec2(+22, 180), ccColor4F(0.5f, 0.5f, 0.5f, 1.0f));
	Sprite* bar;
	for (int i = 0; i < 45; i++) {
		if (i < 15) {
			bar = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("groovy_bar_low"));
		}
		else if (i >= 15 && i < 30) {
			bar = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("groovy_bar_medium"));
		}
		else {
			bar = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("groovy_bar_high"));
		}
		bar->setPosition(Vec2(0, 2 + i * 4));
		bar->setVisible(false);
		meter->addChild(bar, -1);
		grooveBars[i] = bar;
	}
}

void View::redrawGroove(float g) {
	//meter->removeAllChildrenWithCleanup(true);
	//draw outer rectangle
	int upper_bound = round(45 * g);
	//upper_bound =  45;
	for (int i = 0; i < 45; i++) {
		grooveBars[i]->setVisible(i < upper_bound);
	}
	//view->meter->drawSolidRect(Vec2(-15, 0), Vec2(+15, g * 180), ccColor4F(0.0f, 1.0f, 1.0f, 1.0f));

}

void View::shake(float start, float now, Vec2 dir, double errorWindow){
	float v = sin((now - start) * M_PI / errorWindow);
	allSpace->setPosition(shakeCenter + v * SHAKE_STRENGTH * dir);
}

void View::createMusicNotePath(Sprite **mem){
	Sprite* singleMusicNote;
	for (int i = 0; i < MAX_MUSIC_NOTES; i++){
		singleMusicNote = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("music_note"));
		singleMusicNote->setScale(MUSIC_NOTE_SCALE);
		enviornment->addChild(singleMusicNote, 1);
		mem[i] = singleMusicNote;
	}
}



View::~View()
{
}
