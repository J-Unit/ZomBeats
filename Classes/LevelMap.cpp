#include "LevelMap.h"
#include <iostream>
#include "PriorityQueue.h"

const int LevelMap::OFF_X[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
const int LevelMap::OFF_Y[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
const float LevelMap::DIST[8] = { 1.41421356, 1, 1.41421356, 1, 1, 1.41421356, 1, 1.41421356 };




// Reverses prev backpointers into forward next pointers so path can be followed
void LevelMap::reversePath(MapNode* last)
{
	MapNode* cur = last->prev;
	while (cur != 0){
		cur->next = last;
		last = cur;
		cur = cur->prev;
	}
}

float LevelMap::heuristicDistance(MapNode *a, MapNode *b){
	float x = a->x - b->x;
	float y = a->y - b->y;
	return sqrt(x*x + y*y);
}

// A* implementation
void LevelMap::shortestPath(MapNode *from, MapNode *to){
	CTypedPtrHeap<MapNode> pq;
	MapNode *node, *neighbor;
	int x, y;
	float c;
	for (int i = 0; i < BLOCKS_X; i++) for (int j = 0; j < BLOCKS_Y; j++) {
		mesh[i][j].status = INITIAL;
		mesh[i][j].next = 0;
	}
	from->pathCost = 0;
	from->prev = 0;
	pq.Insert(from);
	while (!pq.IsEmpty()){
		node = pq.ExtractMin();
		if (node == to){
			reversePath(to);
			return;
		}
		node->status = VISITED;
		for (int i = 0; i < 8; i++){
			x = node->x + OFF_X[i];
			y = node->y + OFF_Y[i];
			if (x >= 0 && x < BLOCKS_X && y >= 0 && y < BLOCKS_Y){
				neighbor = &mesh[x][y];
				if (neighbor->walkable){
					c = node->pathCost + DIST[i];
					switch (neighbor->status){
					case INITIAL:
						neighbor->pathCost = c;
						neighbor->prev = node;
						neighbor->status = FRONTIER;
						neighbor->heuristicCost = this->heuristicDistance(neighbor, to);
						pq.Insert(neighbor);
						break;
					case FRONTIER:
						if (c < neighbor->pathCost){
							neighbor->pathCost = c;
							neighbor->prev = node;
							pq.Update(neighbor);
						}
						break;
					}
				}
			}
		}
	}
}

LevelMap::~LevelMap()
{
}
