#include "GoalObject.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"


GoalObject::GoalObject(b2World *world, float x, float y, std::string object_name)
{
	pos_x = x;
	pos_y = y;
	isCollected = false;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	box.SetAsBox(25.0f, 25.0f);
	fixture.shape = &box;
	fixture.isSensor = true;
	body->CreateFixture(&fixture);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture(object_name)));
}

void GoalObject::setSprite(Sprite* value)
{
	b2Vec2 pos = body->GetPosition();
	sprite = value;
	value->setPhysicsBody(0);
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
}

GoalObject::~GoalObject()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}