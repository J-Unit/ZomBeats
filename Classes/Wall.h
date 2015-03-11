#ifndef __TD_WALL_H__
#define __TD_WALL_H__

#include <stdio.h>
#include <cocos2d.h>
#include <cocos2d/external/Box2D/Box2D.h>
#include "BoxObject.h"

using namespace cocos2d;
#pragma once
class Wall : public BoxObject
{
public:
	/**
	* Creates a new tile at the specified position (in world space).
	*
	* @param x The world space x-coordinate
	* @param y The world space y-coordinate
	*/
	//Wall(b2World *world, float x, float y);
	Wall(){
	}
	void init(b2World *world, float x, float y);
	float width, height;
	Sprite* sprite;
	b2PolygonShape box;
	void    setSprite(Sprite* value);
	~Wall();
};

#endif