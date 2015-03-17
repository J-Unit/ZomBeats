#include "GameState.h"
#include "LevelMap.h"

GameState::GameState(int tilesX, int tilesY, int nodesX, int nodesY)
{
	world = new b2World(b2Vec2(0.0f, 0.0f));
	level = new LevelMap(tilesX, tilesY, nodesX, nodesY);
}


GameState::~GameState()
{
}
