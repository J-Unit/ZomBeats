#include "EnvironmentWeapon.h"

#define IMPULSE 2800000

class Lawnmower : public EnvironmentWeapon
{
public:
	/**
	* Creates a new sword at the specified position (in world space).
	*
	* @param x The world space x-coordinate
	* @param y The world space y-coordinate
	*/
	Lawnmower(b2World *world, float x, float y);
	Lawnmower(b2World *world, float x, float y, b2Vec2 dir);
	bool   update(float deltaTime, Vec2 thrust);
	~Lawnmower();
	CCParticleSystemQuad *emitter;
	void addSmoke();
	void addParticles();
	void addParticlesForTexture(std::string t, int z, float r, float sa);
};
