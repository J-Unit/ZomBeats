#define INITIAL 0
#define FRONTIER 1
#define VISITED 2

struct MapNode {
	int x;
	int y;
	bool walkable;
	MapNode *prev;
	MapNode *next;
	float pathCost;
	float heuristicCost;
	int status;


	MapNode(int xx, int yy, bool w){
		x = xx;
		y = yy;
		walkable = w;
		pathCost = 0;
		heuristicCost = 0;
		prev = 0;
		status = INITIAL;
	}
	MapNode(int xx, int yy) : MapNode(xx, yy, true){}

	/////// PQ STUFF DO NOT ALTER ///////
	int pqIndex;
	int& Index(void) {
		return pqIndex;
	}
	int Index(void) const {
		return pqIndex;
	}
	/////////////////////////////////////

};

inline int operator < (const MapNode& a, const MapNode& b)
{
	return a.pathCost + a.heuristicCost < b.pathCost + b.heuristicCost;
}

