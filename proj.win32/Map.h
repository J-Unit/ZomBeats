#include <vector>
#include <allocators>

#define BLOCKS_X 20
#define BLOCKS_Y 20

class Map
{
public:

	struct Node {
		int x;
		int y;
		bool walkable;

		Node(int xx, int yy, bool w){
			x = xx;
			y = yy;
			walkable = w;
		}
		Node(int xx, int yy) : Node(xx, yy, true){}	


	};


	std::vector<Node> shortestPath(Node from, Node to);


	Map();
	~Map();

private:
	void forEachNeighbor(void(*fun)(Node));
	
	Node *mesh;

};

	