#include "PriorityQueue.h"
#include "Ship.h"

class Zombie;
class LevelMap;

class GameState
{
public:
	Ship *ship;
	CTypedPtrDblList<Zombie> zombies;
	LevelMap *level;

	GameState();
	~GameState();
};

