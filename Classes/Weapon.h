#include <stdio.h>
#include <cocos2d.h>
#include "BoxObject.h"

using namespace cocos2d;

class Weapon : public BoxObject
{
public:
	int weapon_type;
	Sprite* sprite;
	b2PolygonShape box;
	float pos_x;
	float pos_y;
	bool isDesroyed;
	void setSprite(Sprite* value);
};
