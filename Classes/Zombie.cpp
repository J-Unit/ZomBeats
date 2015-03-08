#include "Zombie.h"
#include "ResourceLoader.h"


Zombie::Zombie(int x, int y, b2World *world)
{
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	circle.m_radius = 60;
	fixture.shape = &circle;
	body->CreateFixture(&fixture);
	type = Type(this);
	body->SetUserData(&type);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("planet1")));
}

void Zombie::setSprite(Sprite* value)
{
	sprite = value;
	sprite->setScale(PLANET_SCALE, PLANET_SCALE);
	b2Vec2 pos = body->GetPosition();
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
}


Zombie::~Zombie()
{
}
