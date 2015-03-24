#include "Zombie.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"
#include "FilmStrip.h"
#include "math.h"

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
	frameRate = 0;
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("zombie"), 2, 2, 4));
	//initialize the awareness
	awareness = INITIAL_AWARENESS;
	cohesion.SetZero();
	seperation.SetZero();
	attraction.SetZero();
	zombiness.SetZero();
	alignment.SetZero();
}

void Zombie::setSprite(FilmStrip* value)
{
	sprite = value;
	value->setPhysicsBody(0);
	sprite->setScale(ZOMBIE_SCALE, ZOMBIE_SCALE);
	b2Vec2 pos = body->GetPosition();
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
}

void Zombie::advanceFrame() {
	frameRate++;
	if (frameRate > 10000) {
		frameRate = 0;
	}

	if (frameRate % ZOMBIE_FRAME_INTERVAL == 0) {
		unsigned int frame = sprite->getFrame();
		if (frame == 3) {
			frame = 0;
		}
		else {
			frame++;
		}
		sprite->setFrame(frame);
	}
}

void Zombie::increaseAwarness()
{
	awareness += AWARENESS_INCREASE;
}


Zombie::~Zombie()
{
}
