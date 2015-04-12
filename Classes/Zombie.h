#include <cocos2d.h>
#include "BoxObject.h"
#include "Box2D/Box2D.h"
#include "FilmStrip.h"
#include "math.h"

#define ZOMBIE_SCALE 1.5f
#define AWARENESS_INCREASE 1.5f
#define INITIAL_AWARENESS 0.0f 
#define ZOMBIE_FRAME_INTERVAL 10

using namespace cocos2d;
class Zombie : public BoxObject
{
public:
	float awareness;
	int frameRate;
	int prevFrame; //the previous animation frame
	bool isDestroyed;
	b2PolygonShape dynamicBox;
	b2Vec2 lastPosition;
	FilmStrip* sprite;
	b2Vec2 direction, seperation, zombiness, cohesion, alignment, attraction, aidir;



	void setSprite(FilmStrip* value);
	void advanceFrame();
	void increaseAwarness();

	Zombie(float x, float y, b2World* world);
	~Zombie();
};

