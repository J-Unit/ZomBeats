#include <cocos2d.h>
#include "BoxObject.h"
#include "Box2D/Box2D.h"
#include "FilmStrip.h"
#include "math.h"

#define ZOMBIE_SCALE 1.5f
#define AWARENESS_INCREASE 3.0f
#define INITIAL_AWARENESS 0.0f 
#define ORIGIN_AWARENESS 0.8f
#define BOTTLE_AWARENES_INCREASE 10.0f
#define MOWER_AWARENES_INCREASE 0.05f
#define MAX_AWARENESS 10.0f
#define ZOMBIE_FRAME_INTERVAL 10
#define ENVI_ATTRACT_RANGE 400.0f

using namespace cocos2d;
class Zombie : public BoxObject
{
public:
	float awareness[3];
	b2Vec2 initialLocation;
	b2Vec2 interestPoint;
	int frameRate;
	int prevFrame; //the previous animation frame
	bool isDestroyed;
	b2PolygonShape dynamicBox;
	b2Vec2 lastPosition;
	FilmStrip* sprite;
	b2Vec2 direction, seperation, zombiness, cohesion, alignment, attraction, aidir;
	ParticleSystemQuad *emitter;



	void setSprite(FilmStrip* value);
	void advanceFrame();
	void increaseAwarness();
	void addParticles();
	void attractToInterestPoint(b2Vec2 p, float a);
	Zombie(float x, float y, b2World* world, float oa);
	~Zombie();

private:
	void increaseAwarness(int ix, float amt);

};

