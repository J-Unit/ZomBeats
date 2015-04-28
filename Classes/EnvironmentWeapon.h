#ifndef EWEAPON_H
#define EWEAPON_H

#include <stdio.h>
#include <cocos2d.h>
#include "Box2D/Box2D.h"
#include "BoxObject.h"

using namespace cocos2d;

class EnvironmentWeapon : public BoxObject
{
public:
	b2PolygonShape dynamicBox;
	int e_weapon_type;
	bool hasMoved;
	Sprite* sprite;
	b2PolygonShape box;
	float pos_x;
	float pos_y;
	bool onCooldown;
	float cdTimer;
	bool isUsed;
	bool hitWall;
	void setSprite(Sprite* value);
};

#endif
