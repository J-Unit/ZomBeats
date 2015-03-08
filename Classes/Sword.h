#include "Weapon.h"

class Sword : public Weapon
{
public:
	/**
	* Creates a new sword at the specified position (in world space).
	*
	* @param x The world space x-coordinate
	* @param y The world space y-coordinate
	*/
	Sword(b2World *world, float x, float y);
	~Sword();
};

