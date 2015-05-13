#ifndef GOAL_H
#define GOAL_H

#include <stdio.h>
#include <cocos2d.h>
#include "Box2D/Box2D.h"
#include "BoxObject.h"
#include <string>

using namespace cocos2d;

class GoalObject : public BoxObject
{
public:
	Sprite* sprite;
	b2PolygonShape box;
	float pos_x;
	float pos_y;
	bool isCollected;
	GoalObject(b2World *world, float x, float y, std::string object_name);
	void setSprite(Sprite* value);
	float range;//how far out the weapon reaches away from ricky
	float width; //how wide the weapon hits side to side
	float detectionRange;
	~GoalObject();
};

#endif
