#include "Wall.h"


Wall::Wall(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	b2BodyDef bodyDef;
	//bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(25.0f, 25.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	body->CreateFixture(&fixtureDef);
}

void Wall::setSprite(Sprite* value)
{
	if (sprite != NULL) {
		//sprite->release();
	}
	sprite = value;
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos_x, pos_y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}


}

Wall::~Wall()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}
