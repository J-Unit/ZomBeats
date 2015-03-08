#include "Weapon.h"

void Weapon::setSprite(Sprite* value)
{
	b2Vec2 pos = body->GetPosition();
	sprite = value;
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
}