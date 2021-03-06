#include "Sword.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"


Sword::Sword(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	width = 130.0f;
	durability = 3;
	range = 105.0f;
	isDesroyed = false;
	weapon_type = 0; //TODO: Make array of different weapon types somewhere
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	box.SetAsBox(25.0f, 25.0f);
	fixture.shape = &box;
	fixture.isSensor = true;
	body->CreateFixture(&fixture);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("sword")));
}


Sword::~Sword()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
	body->GetWorld()->DestroyBody(body);
}
