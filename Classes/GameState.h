#include "PriorityQueue.h"
#include "Ship.h"
#include <vector>

class Zombie;
class LevelMap;
class Weapon;
class EnvironmentWeapon;
class GoalObject;

class GameState
{
public:
	Ship *ship;
	CTypedPtrDblList<Zombie> zombies, dyingZombies;
	CTypedPtrDblList<Weapon> weapons;
	CTypedPtrDblList<EnvironmentWeapon> environment_weapons;
	GoalObject *instrument;
	LevelMap *level;
	b2World *world;
	int bkgTilesX;
	int bkgTilesY;

	//used to track multiple goals
	int numZombiesRemain;
	int zomGoal;
	std::string levelTip;
	GameState(int tx, int ty, int nx, int ny);
	~GameState();
};

