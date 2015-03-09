#include "Wall.h"
#include "ResourceLoader.h"


Wall::Wall(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	box.SetAsBox(9.0f, 10.0f);
	fixture.shape = &box;
	body->CreateFixture(&fixture);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("wall")));

}

void Wall::setSprite(Sprite* value)
{
	b2Vec2 pos = body->GetPosition();
	sprite = value;
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
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
