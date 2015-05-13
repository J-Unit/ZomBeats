#include "LevelSerializer.h"
#include "GameState.h"
#include "LevelMap.h"
#include "Wall.h"
#include "Sword.h"
#include "Zombie.h"
#include "View.h"
#include "Trashcan.h"
#include "Lawnmower.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "GoalObject.h"
#include "SongDecomposition.h"

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

SongDecomposition *LevelSerializer::getLevelTrack(){
	return new SongDecomposition(d["track"]["bpm"].GetDouble(), d["track"]["path"].GetString(), d["track"]["offset"].GetDouble());
}

void LevelSerializer::addObjects(GameState *s){
	int top = s->level->bkgTilesY * SPACE_TILE;
	s->level->nWalls = d["walls"].Size();
	s->level->walls = new Wall[s->level->nWalls];
	for (int i = 0; i < s->level->nWalls; i++){
		float off = getWallOffsetByType(d["walls"][i]["type"].GetString());
		s->level->walls[i].init(s->world, s->level->tileWidth*(d["walls"][i]["x"].GetInt()+off), top-s->level->tileHeight*(d["walls"][i]["y"].GetInt()+off), d["walls"][i]["type"].GetString());
	}
	s->level->markWallTiles();
	for (unsigned int i = 0; i < d["weapons"].Size(); i++){
		Weapon *w;
		std::string type = d["weapons"][i]["type"].GetString();
		if (type == "shotgun"){
			w = new Shotgun(s->world, s->level->tileWidth*(d["weapons"][i]["x"].GetInt() + 0.5f), top - s->level->tileHeight*(d["weapons"][i]["y"].GetInt() + 0.5f));
		}
		else{
			w = new Sword(s->world, s->level->tileWidth*(d["weapons"][i]["x"].GetInt() + 0.5f), top - s->level->tileHeight*(d["weapons"][i]["y"].GetInt() + 0.5f));
		}
		s->weapons.AddTail(w);
	}
	for (unsigned int i = 0; i < d["environment_weapons"].Size(); i++){
		EnvironmentWeapon *ew;
		std::string type = d["environment_weapons"][i]["type"].GetString();
		if (type == "lawnmower"){
			ew = new Lawnmower(s->world, s->level->tileWidth*(d["environment_weapons"][i]["x"].GetInt() + 0.5f), top - s->level->tileHeight*(d["environment_weapons"][i]["y"].GetInt() + 0.5f));
		}
		else{
			ew = new Trashcan(s->world, s->level->tileWidth*(d["environment_weapons"][i]["x"].GetInt() + 0.5f), top - s->level->tileHeight*(d["environment_weapons"][i]["y"].GetInt() + 0.5f));
		}
		s->environment_weapons.AddTail(ew);
	}
	for (unsigned int i = 0; i < d["zombies"].Size(); i++){
		s->zombies.AddTail(new Zombie(s->level->tileWidth*(d["zombies"][i]["x"].GetInt()+0.5f), top-s->level->tileHeight*(d["zombies"][i]["y"].GetInt()+0.5f), s->world));
	}
	s->ship = new Ship(s->world, s->level->tileWidth*(d["startX"].GetInt()+0.5f), top-s->level->tileHeight*(d["startY"].GetInt()+0.5f), 0.5f * d["bkgX"].GetInt() * 256, 0.5f * d["bkgY"].GetInt() * 256+10);

	//the new goal object
	if (d.HasMember("goal_object")){
		s->instrument = new GoalObject(s->world, s->level->tileWidth*(d["goal_object"]["x"].GetInt() + 0.5f), top - s->level->tileHeight*(d["goal_object"]["y"].GetInt() + 0.5f), d["goal_object"]["type"].GetString());
	}
	
}

float LevelSerializer::getWallOffsetByType(std::string type){
	if (type == "tree"){
		return 1.0f;
	}
	else if (type == "house"){
		return 2.0f;
	}
	else{
		return 0.5f;
	}
}