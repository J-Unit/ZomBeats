#include "LevelSerializer.h"
#include "GameState.h"
#include "LevelMap.h"
#include "Wall.h"
#include "Sword.h"
#include "Zombie.h"
#include "View.h"
#include "Lawnmower.h"

GameState *LevelSerializer::parseLevel(std::string f){
	bool t = FileUtils::getInstance()->isFileExist(f);
	ssize_t bufsize = 1024*1024;
	std::string zz = FileUtils::getInstance()->getStringFromFile(f);
	const char *data = zz.c_str();
	d.Parse<0>(data);
	int x = d["bkgX"].GetInt();
	GameState *s = new GameState(x, d["bkgY"].GetInt(), d["nodesX"].GetInt(), d["nodesY"].GetInt());
	return s;
}

void LevelSerializer::addObjects(GameState *s){
	int top = s->level->bkgTilesY * SPACE_TILE;
	s->level->nWalls = d["walls"].Size();
	s->level->walls = new Wall[s->level->nWalls];
	for (int i = 0; i < s->level->nWalls; i++){
		s->level->walls[i].init(s->world, s->level->tileWidth*(d["walls"][i]["x"].GetInt()+0.5f), top-s->level->tileHeight*(d["walls"][i]["y"].GetInt()+0.5f));
	}
	s->level->markWallTiles();
	for (int i = 0; i < d["weapons"].Size(); i++){
		s->weapons.AddTail(new Sword(s->world, s->level->tileWidth*(d["weapons"][i]["x"].GetInt()+0.5f), top-s->level->tileHeight*(d["weapons"][i]["y"].GetInt()+0.5f)));
	}
	for (int i = 0; i < d["environment_weapons"].Size(); i++){
		s->environment_weapons.AddTail(new Lawnmower(s->world, s->level->tileWidth*(d["environment_weapons"][i]["x"].GetInt() + 0.5f), top - s->level->tileHeight*(d["environment_weapons"][i]["y"].GetInt() + 0.5f)));
	}
	for (int i = 0; i < d["zombies"].Size(); i++){
		s->zombies.AddTail(new Zombie(s->level->tileWidth*(d["zombies"][i]["x"].GetInt()+0.5f), top-s->level->tileHeight*(d["zombies"][i]["y"].GetInt()+0.5f), s->world));
	}
	s->ship = new Ship(s->world, s->level->tileWidth*(d["startX"].GetInt()+0.5f), top-s->level->tileHeight*(d["startY"].GetInt()+0.5f), 0.5f * d["bkgX"].GetInt() * 256, 0.5f * d["bkgY"].GetInt() * 256);
}