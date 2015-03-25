#include "LevelMap.h"
#include "Box2D/Box2D.h"
#include "ResourceLoader.h"
#include "FilmStrip.h"
#include <string>
#include <math.h>
#include <iostream>
#include "Wall.h"

#pragma once
class GameMenu
{
public:
	GameMenu();
	Sprite* sprite;
	void setSprite(Sprite* value);
	~GameMenu();
};

