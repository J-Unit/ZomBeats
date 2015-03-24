#include "Zombie.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"

Zombie::Zombie(float x, float y, b2World *world)
{
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	isDestroyed = false;
	body = world->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(30.0f, 60.0f);
	fixture.shape = &dynamicBox;
	fixture.density = 0.5f;
	body->CreateFixture(&fixture);
	body->SetLinearDamping(1.0f);
	type = Type(this);
	body->SetUserData(&type);
	direction.Set(1, 0);
	//setVecRandom(&direction);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("zombie")));
	//initialize the awareness
	awareness = INITIAL_AWARENESS;
	cohesion.SetZero();
	seperation.SetZero();
	attraction.SetZero();
	zombiness.SetZero();
	alignment.SetZero();
}

void Zombie::setSprite(Sprite* value)
{
	sprite = value;
	value->setPhysicsBody(0);
	sprite->setScale(PLANET_SCALE, PLANET_SCALE);
	b2Vec2 pos = body->GetPosition();
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
}

void Zombie::increaseAwarness()
{
	awareness += AWARENESS_INCREASE;
}


Zombie::~Zombie()
{
}
