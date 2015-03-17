#include <cocos2d.h>
#include "BoxObject.h"
#include "Box2D/Box2D.h"

#define PLANET_SCALE 1.0f
#define AWARENESS_INCREASE 1.5f
#define INITIAL_AWARENESS 0.0f 

using namespace cocos2d;
class Zombie : public BoxObject
{
public:
	float awareness;
	bool isDestroyed;
	b2CircleShape circle;

	Sprite *sprite;
	b2Vec2 direction;


	void setSprite(Sprite *value);
	void increaseAwarness();

	Zombie(float x, float y, b2World* world);
	~Zombie();
};

