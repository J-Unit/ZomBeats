#include <cocos2d.h>
#include "BoxObject.h"

#define PLANET_SCALE 0.5f
#define AWARENESS_INCREASE 1.5f
#define INITIAL_AWARENESS 0.0f 

using namespace cocos2d;
class Zombie : public BoxObject
{
public:
	float awareness;
	b2CircleShape circle;

	Sprite *sprite;


	void setSprite(Sprite *value);
	void increaseAwarness();

	Zombie(int x, int y, b2World* world);
	~Zombie();
};

