#include "EnvironmentWeapon.h"

#define IMPULSE 5000000
#define TDISTANCE 350.0f
#define TTOTAL 250.0f

class Trash : public EnvironmentWeapon
{
public:
	/**
	* Creates a new sword at the specified position (in world space).
	*
	* @param x The world space x-coordinate
	* @param y The world space y-coordinate
	*/
	Trash(b2World *world, float x, float y);
	bool   update(float deltaTime, Vec2 thrust);
	Vec2 startPos;
	void addParticles();
	~Trash();
};

