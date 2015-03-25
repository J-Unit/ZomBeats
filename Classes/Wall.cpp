#include "Wall.h"
#include "ResourceLoader.h"

#define WALL_WIDTH 35;
#define WALL_HEIGHT 35;
/*
Wall::Wall(b2World *world, float x, float y)
{
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	width = 18;
	height = 20;
	box.SetAsBox(width/2, height/2);
	fixture.shape = &box;
	body->CreateFixture(&fixture);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("wall")));

}*/

void Wall::init(b2World *world, float x, float y, std::string ty){
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	setSize(ty);
	box.SetAsBox(width, height);
	fixture.shape = &box;
	body->CreateFixture(&fixture);
	setSprite(Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture(ty)));
}

void Wall::setSize(std::string type){
	if (type == "tree"){
		width = 2 * WALL_WIDTH;
		height = 2 * WALL_HEIGHT;
	}
	else if (type == "house"){
		width = 4 * WALL_WIDTH;
		height = 4 * WALL_HEIGHT;
	}
	else{
		width = WALL_WIDTH;
		height = WALL_HEIGHT;
	}
}

void Wall::setSprite(Sprite* value)
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

Wall::~Wall()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}
