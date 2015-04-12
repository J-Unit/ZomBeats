#include "Lawnmower.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"


Lawnmower::Lawnmower(b2World *world, float x, float y)
{
	pos_x = x;
	pos_y = y;
	isUsed = false;
	hasMoved = false;
	hitWall = false;
	e_weapon_type = 0; //TODO: Make array of different weapon types somewhere
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	box.SetAsBox(35.0f, 35.0f);
	fixture.shape = &box;
	body->CreateFixture(&fixture);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("mower")));
}

Lawnmower::Lawnmower(b2World *world, float x, float y, b2Vec2 dir)
{
	pos_x = x;
	pos_y = y;
	isUsed = false;
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
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("mower")));
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
