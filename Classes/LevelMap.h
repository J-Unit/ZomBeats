#include <vector>
#include "Node.h"
#include "cocos2d.h"

#define BLOCKS_X 50
#define BLOCKS_Y 50

//struct MapNode;

class LevelMap
{
public:
	
	int levelWidth;
	int levelHeight;
	float tileWidth;
	float tileHeight;

	void shortestPath(MapNode *from, MapNode *to);


	LevelMap(int width, int height){
		//initialize level and tile dimensions
		levelWidth = width;
		levelHeight = height;
		tileWidth = levelWidth / (float)BLOCKS_X;
		tileHeight = levelHeight / (float)BLOCKS_Y;

		//initialize the mesh node array
		for (int r = 0; r < BLOCKS_X; r++){
			for (int c = 0; c < BLOCKS_Y; c++){
				mesh[r][c].setXY(r, c);
			}
		}
	}
	MapNode *locateCharacter(float charX, float charY);

	float getTileCenterX(MapNode *tile);
	float getTileCenterY(MapNode *tile);

	MapNode mesh[BLOCKS_X][BLOCKS_Y];
	
	~LevelMap();

private:
	static const int OFF_X[8]; 
	static const int OFF_Y[8];
	static const float DIST[8];

	void reversePath(MapNode *to);

	float heuristicDistance(MapNode *a, MapNode *b);

	



}; 

	