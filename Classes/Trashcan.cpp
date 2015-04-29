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
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("trashcan"), 1, 2, 2));
}

Trashcan::~Trashcan()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}
