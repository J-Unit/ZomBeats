#include "Lawnmower.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"


Lawnmower::Lawnmower(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	isUsed = false;
	hasMoved = false;
	cdTimer = 0.0f;
	hitWall = false;
	onCooldown = false;
	isJammed = false;
	e_weapon_type = 0; //TODO: Make array of different weapon types somewhere
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	box.SetAsBox(35.0f, 35.0f);
	fixture.shape = &box;
	fixture.isSensor = true;
	body->CreateFixture(&fixture);
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("mower"), 1, 1, 1));
}

void Lawnmower::addParticles(){
	addParticlesForTexture("grass1", 1, 8.0f, 0.32f);
	addParticlesForTexture("grass2", -1, 13.5f, 1.0f);
	addParticlesForTexture("grass3", -1, 13.5f, 1.0f);
	addParticlesForTexture("grass1", -1, 13.5f, 1.0f);
}

void Lawnmower::addParticlesForTexture(std::string text, int z, float r, float sa){
	//emitter = CCParticleSun::create();
	//emitter->setEmitterMode(kCCParticleModeGravity);
	emitter = CCParticleSun::createWithTotalParticles(100);

	emitter->setDuration(kCCParticleDurationInfinity);
	emitter->setLife(1.02);
	emitter->setLifeVar(0.1);
	emitter->setSpeed(55.0f);
	emitter->setSpeedVar(15.0f);
	emitter->setPosVar(Vec2(15,15));
	//emitter->setSpeed(0.0f);
	//emitter->setSpeedVar(0.0f);
	emitter->setEmissionRate(r);

	emitter->setGravity(ccp(0, -3));
	//emitter->setTangentialAccelVar(10.0f);
	//emitter->setRadialAccelVar(10.0f);
	/*emitter->setSpeed(1000.0f);

	emitter->setEmissionRate(5.0f);


	emitter->setRadialAccelVar(200.0f);*/
	//emitter->setRadialAccel(2.0f);
	//emitter->setTangentialAccelVar(200.0f);
	emitter->setAnchorPoint(Vec2(0.5f, 0.5f));
	emitter->setPosition(sprite->getContentSize() / 2);
	emitter->setStartSize(7.0);
	emitter->setEndSize(6.5);
	emitter->setStartSizeVar(2.0);
	emitter->setEndSizeVar(1);
	emitter->setBlendAdditive(false);
	emitter->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);

	emitter->setTexture(ResourceLoader::getInstance()->getTexture(text));
	emitter->setStartColorVar(ccColor4F(0.05f, 0.05f, 0.05f, 0));
	emitter->setEndColorVar(ccColor4F(0, 0, 0, 0));
	emitter->setStartColor(ccColor4F(1.0f, 1.0f, 1.0f, sa));
	emitter->setEndColor(ccColor4F(0.97f, 0.97f, 0.97f, 0.5f));
	sprite->addChild(emitter, z);
}

Lawnmower::Lawnmower(b2World *world, float x, float y, b2Vec2 dir)
{
	pos_x = x;
	pos_y = y;
	isUsed = false;
	onCooldown = false;
	isJammed = false;
	hasMoved = true;
	hitWall = false;
	e_weapon_type = 0; //TODO: Make array of different weapon types somewhere

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
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("mower"), 1, 1, 1));
}

bool Lawnmower::update(float deltaTime, Vec2 dir) {
	/*
	if (boostFrames <= 0) return false;
	boostFrames--;*/
	if (!hasMoved) return false;
	//float32 angle = body->GetAngle();
	if (body->GetLinearVelocity().Length() < 0.001f){
		body->ApplyLinearImpulse(b2Vec2(dir.x * IMPULSE, dir.y * IMPULSE), body->GetPosition(), true);
	}
	else{
		b2Vec2 temp = body->GetLinearVelocity();
		temp.Normalize();
		body->ApplyLinearImpulse(b2Vec2(temp.x * IMPULSE, temp.y * IMPULSE), body->GetPosition(), true);
	}
	

	return true;
}


Lawnmower::~Lawnmower()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}
