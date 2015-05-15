#include "Zombie.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"
#include "FilmStrip.h"
#include "math.h"

Zombie::Zombie(float x, float y, b2World *world, float originAwareness)
{
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	isDestroyed = false;
	body = world->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(30.0f, 60.0f);
	fixture.shape = &dynamicBox;
	fixture.density = 0.5f;
	body->CreateFixture(&fixture);
	body->SetLinearDamping(1.0f);
	type = Type(this);
	body->SetUserData(&type);
	direction.Set(1, 0);
	//setVecRandom(&direction);
	frameRate = 0;
	prevFrame = 0;
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("zombie"), 8, 3, 24));
	//setDyingSprite();
	//initialize the awareness
	awareness[0] = originAwareness;
	awareness[1] = INITIAL_AWARENESS;
	awareness[2] = 0.0;
	interestPoint.SetZero(); 
	initialLocation.Set(x, y);
	cohesion.SetZero();
	seperation.SetZero();
	attraction.SetZero();
	zombiness.SetZero();
	alignment.SetZero();
}

void Zombie::setSprite(FilmStrip* value)
{
	sprite = value;
	value->setPhysicsBody(0);
	sprite->setScale(ZOMBIE_SCALE, ZOMBIE_SCALE);
	b2Vec2 pos = body->GetPosition();
	if (sprite != NULL) {
		sprite->retain(); // Do not delete it until we are done.
		sprite->setPosition(pos.x, pos.y);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	}
	exclamation = Sprite::createWithTexture(ResourceLoader::getInstance()->getTexture("exclamation"));
	exclamation->setAnchorPoint(Vec2(0.5f, 0.5f));
	exclamation->setPosition(Vec2(sprite->getContentSize().width/2, sprite->getContentSize().height * 1.1f));
	exclamation->setVisible(false);
	exclamation->setScale(0.2f);
	sprite->addChild(exclamation, 1);
}

void Zombie::advanceFrame() {
	frameRate++;
	if (frameRate > 10000) {
		frameRate = 0;
	}

	if (frameRate % ZOMBIE_FRAME_INTERVAL == 0) {
		float yMovement = round(body->GetPosition().y - lastPosition.y);
		float xMovement = round(body->GetPosition().x - lastPosition.x);
		unsigned int frame = sprite->getFrame();
		//zombie is moving up
		if (yMovement > 0 && xMovement == 0) {
			//if it is not the current up frame
			if (!(frame >=3 && frame <=5)) {
				frame = 3;
			}
			//go back to sequence 0
			else if (frame == 4 || frame == 5) {
				frame = 3;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 3 && prevFrame == 4) {
				frame = 5;
			}
			else {
				frame = 4;
			}
		}
		//zombie is moving down
		else if (yMovement < 0 && xMovement == 0) {
			//if it is not the current down frame
			if (!(frame >= 0 && frame <= 2)) {
				frame = 0;
			}
			//go back to sequence 0
			else if (frame == 1 || frame == 2) {
				frame = 0;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 0 && prevFrame == 1) {
				frame = 2;
			}
			else {
				frame = 1;
			}
		}
		//zombie is moving left
		else if (xMovement < 0 && yMovement == 0) {
			//if it is not the current up frame
			if (!(frame >= 9 && frame <= 11)) {
				frame = 9;
			}
			//go back to sequence 0
			else if (frame == 10 || frame == 11) {
				frame = 9;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 9 && prevFrame == 10) {
				frame = 11;
			}
			else {
				frame = 10;
			}
		}
		//zombie is moving right
		else if (xMovement > 0 && yMovement == 0) {
			//if it is not the current up frame
			if (!(frame >= 6 && frame <= 8)) {
				frame = 6;
			}
			//go back to sequence 0
			else if (frame == 7 || frame == 8) {
				frame = 6;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 6 && prevFrame == 7) {
				frame = 8;
			}
			else {
				frame = 7;
			}
		}
		//moving right-down
		else if (xMovement > 0 && yMovement < 0) {
			//if it is not the current right-down frame
			if (!(frame >= 12 && frame <= 14)) {
				frame = 12;
			}
			//go back to sequence 0
			else if (frame == 13 || frame == 14) {
				frame = 12;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 12 && prevFrame == 13) {
				frame = 14;
			}
			else {
				frame = 13;
			}
		}
		//moving left-down
		else if (xMovement < 0 && yMovement < 0) {
			//if it is not the current right-down frame
			if (!(frame >= 15 && frame <= 17)) {
				frame = 15;
			}
			//go back to sequence 0
			else if (frame == 16 || frame == 17) {
				frame = 15;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 15 && prevFrame == 16) {
				frame = 17;
			}
			else {
				frame = 16;
			}
		}
		//moving right-up
		else if (xMovement > 0 && yMovement > 0) {
			//if it is not the current right-down frame
			if (!(frame >= 18 && frame <= 20)) {
				frame = 18;
			}
			//go back to sequence 0
			else if (frame == 19 || frame == 20) {
				frame = 18;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 18 && prevFrame == 19) {
				frame = 20;
			}
			else {
				frame = 19;
			}
		}
		//moving left-up
		else if (xMovement < 0 && yMovement > 0) {
			//if it is not the current right-down frame
			if (!(frame >= 21 && frame <= 23)) {
				frame = 21;
			}
			//go back to sequence 0
			else if (frame == 22 || frame == 23) {
				frame = 21;
			}
			//0-1-0-2-0-1-0-2
			else if (frame == 21 && prevFrame == 22) {
				frame = 23;
			}
			else {
				frame = 22;
			}
		}
		else {
			frame = sprite->getFrame();
		}
		prevFrame = sprite->getFrame();
		sprite->setFrame(frame);
		lastPosition = body->GetPosition();
	}
	
}

void Zombie::attractToInterestPoint(b2Vec2 point, float amount){
	interestPoint = point;
	increaseAwarness(2, amount);
}



void Zombie::playZombieDeathAnimation() 
{
	deadFrameRate++;
	if (deadFrameRate > 10000) {
		deadFrameRate = 0;
	}

	if (deadFrameRate % ZOMBIE_FRAME_INTERVAL == 0) {
		unsigned int frame = dyingSprite->getFrame();
		frame++;
		dyingSprite->setFrame(frame);
	}
}


void Zombie::addParticles(){
	emitter = ParticleSystemQuad::create();
	emitter->setDuration(2.0f);
	emitter->setLife(1.6f);
	emitter->setLifeVar(0.2f);
	emitter->setSpeed(80.0f);
	emitter->setSpeedVar(8.0f);
	emitter->setAngle(90.0f);
	emitter->setAngleVar(5.0f);
	emitter->setPosVar(Vec2(15,15));
	//emitter->setSpeed(0.0f);
	//emitter->setSpeedVar(0.0f);
	emitter->setEmissionRate(75.0f);

	emitter->setGravity(ccp(0, -150));
	emitter->setTangentialAccelVar(0);
	emitter->setRadialAccelVar(0);
	/*emitter->setSpeed(1000.0f);

	emitter->setEmissionRate(5.0f);


	emitter->setRadialAccelVar(200.0f);*/
	emitter->setRadialAccel(0);
	emitter->setTangentialAccel(0);
	emitter->setAnchorPoint(Vec2(0.5f, 0.5f));
	emitter->setPosition(sprite->getContentSize() / 2);
	emitter->setStartSize(6.0);
	emitter->setEndSize(4.5);
	emitter->setStartSizeVar(4.0);
	emitter->setEndSizeVar(3);
	emitter->setBlendAdditive(false);
	emitter->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);

	emitter->setTexture(ResourceLoader::getInstance()->getTexture("blood"));
	emitter->setStartColorVar(ccColor4F(0.5f, 0.5f, 0.5f, 0));
	emitter->setEndColorVar(ccColor4F(0.1f, 0.1f, 0.1f, 0));
	emitter->setStartColor(ccColor4F(1.0f, 1.0f, 1.0f, 0.9f));
	emitter->setEndColor(ccColor4F(0.3f, 0.3f, 0.3f, 0.4f));
	sprite->addChild(emitter, 1);
}

void Zombie::increaseAwarness(int ixInterest, float amt){
	awareness[ixInterest] = std::min(awareness[ixInterest] + amt, MAX_AWARENESS);
}

void Zombie::increaseAwarness()
{
	increaseAwarness(1, AWARENESS_INCREASE);
}


Zombie::~Zombie()
{
	// Release the film strip if we have a reference
	if (sprite != NULL) {
		sprite->release();
	}
	sprite = NULL;
}
