#ifndef EWEAPON_H
#define EWEAPON_H

#include <stdio.h>
#include <cocos2d.h>
#include "Box2D/Box2D.h"
#include "BoxObject.h"
#include "FilmStrip.h"
#define EW_FRAME_INTERVAL 20
using namespace cocos2d;

class EnvironmentWeapon : public BoxObject
{
public:
	b2PolygonShape dynamicBox;
	int e_weapon_type;
	bool hasMoved;
	FilmStrip* sprite;
	b2PolygonShape box;
	float pos_x;
	float pos_y;
	bool onCooldown;
	float cdTimer;
	bool isUsed;
	bool isJammed;
	bool hitWall;
	void setSprite(FilmStrip* value);
	void update();
	void update2();
	int frameRate;
	bool beingUsed;
};

#endif
