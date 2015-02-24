#include <vector>
#include "Node.h"

#define BLOCKS_X 20
#define BLOCKS_Y 20

//struct MapNode;

class LevelMap
{
public:
	
	LevelMap();
	LevelMap(int px, int py) : LevelMap(){

	}

	void shortestPath(MapNode *from, MapNode *to);

	
	
	~LevelMap();

private:
	static const int OFF_X[8]; 
	static const int OFF_Y[8];
	static const float DIST[8];

	void reversePath(MapNode *to);

	float heuristicDistance(MapNode *a, MapNode *b);

	
	MapNode mesh[BLOCKS_X][BLOCKS_Y];

}; 

	