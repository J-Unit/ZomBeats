#include <vector>
#include "Node.h"

#define BLOCKS_X 20
#define BLOCKS_Y 20

//struct MapNode;

class LevelMap
{
public:
	
	

	void shortestPath(MapNode *from, MapNode *to);

	LevelMap();
	
	~LevelMap();

private:
	int OFF_X[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int OFF_Y[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	float DIST[8] = { 1.41421356, 1, 1.41421356, 1, 1, 1.41421356, 1, 1.41421356 };

	void reversePath(MapNode *to);

	float heuristicDistance(MapNode *a, MapNode *b);

	
	MapNode mesh[BLOCKS_X][BLOCKS_Y];

}; 

	