#include "Trashcan.h"
#include "ResourceLoader.h"
#include "FilmStrip.h"
#include "Box2D/Box2D.h"


Trashcan::Trashcan(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	isUsed = false;
	hasMoved = false;
	hitWall = false;
	cdTimer = 0.0f;
	onCooldown = false;
	e_weapon_type = 1; //TODO: Make array of different weapon types somewhere
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	box.SetAsBox(35.0f, 35.0f);
	fixture.shape = &box;
	body->CreateFixture(&fixture);
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("trashcan"), 1, 2, 2)); //might have to use setSpriteSh() below instead
}

//TODO: WONT NEED BUT MAKE TRASH CLASS INSTEAD WHICH JUST DOES WHAT MOWER DOES BUT CERTAIN DISTANCE OR JUST CREATE PROJECTILE
Trashcan::Trashcan(b2World *world, float x, float y, b2Vec2 dir)
{
	pos_x = x;
	pos_y = y;
	isUsed = false;
	hasMoved = true;
	hitWall = false;
	e_weapon_type = 1; //TODO: Make array of different weapon types somewhere

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.fixedRotation = true;
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	dynamicBox.SetAsBox(20.0f, 20.0f);
	fixture.shape = &dynamicBox;
	fixture.density = 200.0f;
	body->CreateFixture(&fixture);
	body->SetLinearDamping(1.0f);

	//body->SetAngularDamping(0.5f);
	setSpriteSh(FilmStrip::create(ResourceLoader::getInstance()->getTexture("trashcan"), 1, 2, 2));
}

bool Trashcan::update(float deltaTime, Vec2 dir) {
	frameRate++;
	if (frameRate > 100000) {
		frameRate = 0;
	}
	if (frameRate % TRASHCAN_FRAME_INTERVAL == 0) {
		advanFrame();
	}

	return true;
}

void Trashcan::advanFrame(){
	unsigned int frame = sheet->getFrame();
	if (frame == 0) {
		frame++;
	}
	else {
		frame = 0;
	}
	sheet->setFrame(frame);
}


void Trashcan::flipFrame(){
	
}

void Trashcan::setSpriteSh(FilmStrip* value) {
	// Release the current sprite if we have a reference
	if (sheet != NULL) {
		sheet->release();
	}
	sheet = value;
	value->setPhysicsBody(0);
	if (sheet != NULL) {
		sheet->retain(); // Do not delete it until we are done.
		sheet->setFrame(1);
		sheet->setPosition(pos_x, pos_y);
		sheet->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
}


Trashcan::~Trashcan()
{
	// Release the film strip if we have a reference
	if (sheet != NULL) {
		sheet->release();
	}
	sheet = NULL;
}
