#include "EnvironmentWeapon.h"

#define IMPULSE 2800000
#define TRASHCAN_FRAME_INTERVAL 10
class FilmStrip;

class Trashcan : public EnvironmentWeapon
{
public:
	/**
	* Creates a new sword at the specified position (in world space).
	*
	* @param x The world space x-coordinate
	* @param y The world space y-coordinate
	*/
	Trashcan(b2World *world, float x, float y);
	Trashcan(b2World *world, float x, float y, b2Vec2 dir);
	bool   update(float deltaTime, Vec2 thrust);
	void flipFrame();
	unsigned int prevFrame;
	FilmStrip *sheet;
	void setSpriteSh(FilmStrip* value);
	void advanFrame();
	int frameRate;
	~Trashcan();
};

