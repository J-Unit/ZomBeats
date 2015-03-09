#include "LevelMap.h"
#include <iostream>
#include "PriorityQueue.h"
#include "Wall.h"
#include "util.h"

const int LevelMap::OFF_X[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
const int LevelMap::OFF_Y[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
const float LevelMap::DIST[8] = { 1.41421356, 1, 1.41421356, 1, 1, 1.41421356, 1, 1.41421356 };


void LevelMap::markWallTiles(){
	for (int i = 0; i < BLOCKS_X; i++) for (int j = 0; j < BLOCKS_Y; j++){
		for (int k = 0; k < nWalls; k++){
			if (nodeWallOverlap(&mesh[i][j], &walls[k])){
				mesh[i][j].walkable = false;
			}
		}
	}
}

bool LevelMap::nodeWallOverlap(MapNode *node, Wall *wall){
	b2Vec2 v;
	float w, h;
	float x, y;
	float tw, th;
	v = wall->body->GetPosition();
	w = wall->width / 2;
	h = wall->height / 2;
	tw = tileWidth / 2;
	th = tileHeight / 2;
	x = getTileCenterX(node);
	y = getTileCenterY(node);

	return rectanglesOverlap(v.x - w, v.y - h, v.x + w, v.y + h, x - tw, y - tw, x + tw, y + tw);
}

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

//locate which MapNode a character is at given its X,Y position
MapNode *LevelMap::locateCharacter(float charX, float charY){
	return &mesh[int(charX / tileWidth)][int(charY / tileHeight)];
}

float LevelMap::getTileCenterX(MapNode *tile){
	return (tile->x + 0.5)*tileWidth;
}

float LevelMap::getTileCenterY(MapNode *tile){
	return (tile->y + 0.5)*tileHeight;
}

// A* implementation
void LevelMap::shortestPath(MapNode *from, MapNode *to){
	CTypedPtrHeap<MapNode> pq;
	MapNode *node, *neighbor, *best = 0;
	int x, y;
	float c;
	for (int i = 0; i < BLOCKS_X; i++) for (int j = 0; j < BLOCKS_Y; j++) {
		mesh[i][j].status = INITIAL;
		mesh[i][j].next = 0;
	}
	from->pathCost = 0;
	from->heuristicCost = heuristicDistance(from, to);
	from->prev = 0;
	pq.Insert(from);
	while (!pq.IsEmpty()){
		node = pq.ExtractMin();
		if (node == to){
			reversePath(to);
			return;
		}
		if (best == 0 || node->heuristicCost < best->heuristicCost){
			best = node;
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
	reversePath(best);
}

LevelMap::~LevelMap()
{
}
