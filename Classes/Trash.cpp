#include "Trash.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"
#include <math.h>



Trash::Trash(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	startPos =  Vec2(x, y);
	isUsed = false;
	onCooldown = false;
	cdTimer = 0.0f;
	hasMoved = true;
	hitWall = false;
	e_weapon_type = 2; //TODO: Make array of different weapon types somewhere

	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = false;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	dynamicBox.SetAsBox(20.0f, 20.0f);
	fixture.shape = &dynamicBox;
	fixture.density = 0.0000000000001;
	body->CreateFixture(&fixture);
	body->SetLinearDamping(0.000001f); 
	//body->SetAngularDamping(0.0);
	body->SetFixedRotation(true);
	//body->SetAngularVelocity(999999999999999999999999999999999999999999999999999.0);

	//body->SetAngularDamping(0.5f);
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("trash"), 1, 1, 1));
}

bool Trash::update(float deltaTime, Vec2 dir) {
	/*
	if (boostFrames <= 0) return false;
	boostFrames--;*/
	//if (!hasMoved) return false;
	//float32 angle = body->GetAngle();
	Vec2 currP =  Vec2(body->GetPosition().x, body->GetPosition().y);
	if (abs(currP.distance(startPos)) >= TTOTAL){
		hitWall = true;
		return false;
	}
	if (body->GetLinearVelocity().Length() < 0.001f){
		body->ApplyLinearImpulse(b2Vec2(dir.x * IMPULSE * 100.0f, dir.y * IMPULSE *100.0f), body->GetPosition(), true);
	}
	else{
		b2Vec2 temp = body->GetLinearVelocity();
		temp.Normalize();
		body->ApplyLinearImpulse(b2Vec2(temp.x * IMPULSE*100.0f, temp.y * IMPULSE*100.0f), body->GetPosition(), true);
	}
	//body->SetAngularVelocity(IMPULSE*10000000000000000.0);

	return true;
}

void Trash::addParticles(){
	ParticleSystemQuad *emitter = ParticleSystemQuad::create();
	emitter->setDuration(0.23f);
	emitter->setLife(0.17f);
	emitter->setLifeVar(0.02f);
	emitter->setSpeed(250.0f);
	emitter->setSpeedVar(50.0f);
	emitter->setAngleVar(360.0f);
	emitter->setPosVar(Vec2(5, 5));
	//emitter->setSpeed(0.0f);
	//emitter->setSpeedVar(0.0f);
	emitter->setEmissionRate(200.0f);
	emitter->setAutoRemoveOnFinish(true);
	emitter->setGravity(ccp(0, 0));
	emitter->setTangentialAccelVar(0);
	emitter->setRadialAccelVar(0);
	/*emitter->setSpeed(1000.0f);

	emitter->setEmissionRate(5.0f);


	emitter->setRadialAccelVar(200.0f);*/
	emitter->setRadialAccel(0);
	emitter->setTangentialAccel(0);
	emitter->setAnchorPoint(Vec2(0.5f, 0.5f));
	emitter->setPosition(sprite->getPosition());
	emitter->setStartSize(10.0);
	emitter->setEndSize(8.0);
	emitter->setStartSizeVar(3.0);
	emitter->setEndSizeVar(2);
	emitter->setBlendAdditive(true);
	//emitter->setBlendAdditive(false);
	//emitter->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);

	emitter->setTexture(ResourceLoader::getInstance()->getTexture("glass"));
	emitter->setStartColorVar(ccColor4F(0.4f, 0.4f, 0.4f, 0.2f));
	emitter->setEndColorVar(ccColor4F(0.4f, 0.4f, 0.4f, 0.2f));
	emitter->setStartColor(ccColor4F(1.0f, 1.0, 1.0f, 0.8f));
	emitter->setEndColor(ccColor4F(0.6f, 0.6f, 0.6f, 0.35f));
	sprite->getParent()->addChild(emitter, 1);
}


Trash::~Trash()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}
