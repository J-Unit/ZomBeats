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
	bodyDef.position.Set(x, y);
	bodyDef.fixedRotation = true;
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	dynamicBox.SetAsBox(20.0f, 20.0f);
	fixture.shape = &dynamicBox;
	fixture.density = 100.0f;
	body->CreateFixture(&fixture);
	body->SetLinearDamping(1.0f);

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


	return true;
}


Trash::~Trash()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}
