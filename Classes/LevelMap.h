#include <vector>
#include "Node.h"
#include "View.h"
#include "cocos2d.h"

//struct MapNode;
class Wall;

class LevelMap
{
public:
	int nodesX;
	int nodesY;
	int bkgTilesX;
	int bkgTilesY;
	
	int levelWidth;
	int levelHeight;
	float tileWidth;
	float tileHeight;

	int nWalls;
	Wall *walls;

	void shortestPath(MapNode *from, MapNode *to);
	void markWallTiles();



	LevelMap(int tx, int ty, int nx, int ny){
		//initialize level and tile dimensions
		levelWidth = tx * SPACE_TILE;
		levelHeight = ty * SPACE_TILE;
		bkgTilesX = tx;
		bkgTilesY = ty;
		nodesX = nx;
		nodesY = ny;
		tileWidth = levelWidth / (float)nodesX;
		tileHeight = levelHeight / (float)nodesY;
		//initialize the mesh node array
		mesh = new MapNode*[nodesX];
		for (int r = 0; r < nodesX; r++){
			mesh[r] = new MapNode[nodesY];
			for (int c = 0; c < nodesY; c++){
				mesh[r][c].setXY(r, c);
			}
		}
	}
	MapNode *locateCharacter(float charX, float charY);

	float getTileCenterX(MapNode *tile);
	float getTileCenterY(MapNode *tile);

	//MapNode mesh[BLOCKS_X][BLOCKS_Y];
	MapNode **mesh;
	~LevelMap();

private:
	static const int OFF_X[8]; 
	static const int OFF_Y[8];
	static const float DIST[8];

	void reversePath(MapNode *to);
	bool nodeWallOverlap(MapNode *node, Wall *wall);

	float heuristicDistance(MapNode *a, MapNode *b);

	



}; 

	