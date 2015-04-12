#ifndef WEAPON_H
#define WEAPON_H

#include <stdio.h>
#include <cocos2d.h>
#include "Box2D/Box2D.h"
#include "BoxObject.h"

using namespace cocos2d;

class Weapon : public BoxObject
{
public:
	int weapon_type;
	int durability;
	Sprite* sprite;
	b2PolygonShape box;
	float pos_x;
	float pos_y;
	bool isDesroyed;
	void setSprite(Sprite* value);
	float range;//how far out the weapon reaches away from ricky
	float width; //how wide the weapon hits side to side
};

#endif
