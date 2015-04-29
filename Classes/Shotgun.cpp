#include "Shotgun.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"


Shotgun::Shotgun(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	width = 190.0f;
	durability = 3;
	range = 200.0f;
	detectionRange = 0.55f * range;
	isDesroyed = false;
	weapon_type = 2; //TODO: Make array of different weapon types somewhere
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	box.SetAsBox(25.0f, 25.0f);
	fixture.shape = &box;
	body->CreateFixture(&fixture);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("shotgun")));
}


Shotgun::~Shotgun()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}