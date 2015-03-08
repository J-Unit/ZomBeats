#include "PriorityQueue.h"
#include "Ship.h"
#include <vector>

class Zombie;
class LevelMap;

class GameState
{
public:
	Ship *ship;
	CTypedPtrDblList<Zombie> zombies;
	LevelMap *level;
	b2World *world;

	GameState();
	~GameState();
};

