#include "PriorityQueue.h"
#include "Ship.h"
#include <vector>

class Zombie;
class LevelMap;
class Weapon;

class GameState
{
public:
	Ship *ship;
	CTypedPtrDblList<Zombie> zombies;
	CTypedPtrDblList<Weapon> weapons;
	LevelMap *level;
	b2World *world;

	GameState();
	~GameState();
};

