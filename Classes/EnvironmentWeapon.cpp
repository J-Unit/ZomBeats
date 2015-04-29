#include "EnvironmentWeapon.h"

void EnvironmentWeapon::setSprite(FilmStrip* value)
{
	b2Vec2 pos = body->GetPosition();
	sprite = value;
	value->setPhysicsBody(0);
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
	sprite->setFrame(0);
}

void EnvironmentWeapon::update(){
	/*
	frameRate++;
	if (frameRate > 10000) {
		frameRate = 0;
	}

	if (frameRate % EW_FRAME_INTERVAL == 0) {
		unsigned int frame = sprite->getFrame();
		if (frame == 0) {
			frame = 1;
		}
		else {
			frame = 0;
		}
		sprite->setFrame(frame);
	}
	*/
	unsigned int frame = sprite->getFrame();
	if (frame == 0) {
		frame = 1;
	}
	else {
		frame = 0;
	}
	sprite->setFrame(frame);
}

void EnvironmentWeapon::update2(){
	frameRate++;
	if (frameRate > 10000) {
		frameRate = 0;
	}

	if (frameRate % EW_FRAME_INTERVAL == 0) {
		
		unsigned int frame = sprite->getFrame();
		if (!(frame >= 2 && frame <= 5)) {
			frame = 2;
		}
		else if (frame == 5){
			frame = 2;
		}
		else {
			frame++;
		}
		sprite->setFrame(2);
	}
}
