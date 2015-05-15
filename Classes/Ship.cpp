//
//  Ship.cpp
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#include "Ship.h"
#include "FilmStrip.h"
#include "math.h"
#include "ResourceLoader.h"
#include "Box2D/Box2D.h"

// Macros to eliminate magic numbers
// Some of these are carried over from the 3152 lab and unused.
#define ANGLE_RADIANS (PI/180.0f)
#define SHIP_MAX_TURN      1.0f
#define SHIP_MAX_SPEED    10.0f
#define SHIP_THRUST_FACTOR 0.4f
#define IMPULSE 1500 //was 2875, slightly decreased it to see how it works with grooviness
#define GROOVY_BONUS 2000


#define DCOS_90(a)  (cos(M_PI*(a+90.0f)/180.0f))
#define DSIN_90(a)  (sin(M_PI*(a+90.0f)/180.0f))

#define RANGE_CLAMP(x,y,z)  (x < y ? y : (x > z ? z : x))

/**
 * Creates a new ship at the specified position (in world space).
 *
 * @param x The world space x-coordinate
 * @param y The world space y-coordinate
 */
Ship::Ship(b2World *world, float x, float y, float mx, float my) {
   // position.set(x,y);
   // velocity.set(0.0f,0.0f);
    
   // angle   = 0.0f;
   // turning = 0.0f;
   // forward = 0.0f;
	boostFrames = 0;
	frameRate = 0;
	isDestroyed = false;
	hasWeapon = false;
	isActivatingEnvironment = false;
	currentWeapon = NULL;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.fixedRotation = true;
	hasEnvironmentWeapon = false;
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	dynamicBox.SetAsBox(SHIP_WIDTH, SHIP_HEIGHT);
	fixture.shape = &dynamicBox;
	fixture.density = 0.1f;
	fixture.friction = 0.5f;
	body->CreateFixture(&fixture);
	body->SetLinearDamping(3.0f);
	//body->SetAngularDamping(0.5f);
    sprite  = NULL;

	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("ricky_gun"), 8, 6, 48), mx, my);
	lastPosition = body->GetPosition();
	prevFrame = 0; //initialize the initial frame number
}

/**
 * Disposes the ship, releasing all resources.
 */
Ship::~Ship() {
    // Release the film strip if we have a reference
    if (sprite != NULL) {
        sprite->release();
    }
    sprite = NULL;
	body->GetWorld()->DestroyBody(body);
}

/**
 * Sets the film strip representing this ship.
 *
 * Setting this to NULL clears the value
 *
 * @param value The ship film strip.
 */
void Ship::setSprite(FilmStrip* value, float mx, float my) {
    // Release the current sprite if we have a reference
    if (sprite != NULL) {
        sprite->release();
    }
    sprite = value;
	value->setPhysicsBody(0);
	sprite->setScale(RICKY_SCALE);
    if (sprite != NULL) {
        sprite->retain(); // Do not delete it until we are done.
        sprite->setFrame(SHIP_IMG_FLAT);
        sprite->setPosition(Vec2(mx, my));
        sprite->setAnchorPoint(Vec2(0.5f,0.5f));
    }
	sprite->setFrame(0);
}

/**
 * Updates the state of the model
 *
 * This method moves the ship forward, dampens the forces (if necessary)
 * and updates the sprite if it exists.
 *
 * @param deltaTime Time elapsed since last called.
 */
bool Ship::update(float deltaTime, Vec2 dir, float grooviness) {
	if (boostFrames <= 0) {
		body->SetLinearDamping(NORMAL_DAMPENING);
		return false;
	}
	boostFrames--;
	frameRate++;
	if (frameRate > 10000) {
		frameRate = 0;
	}
	//float32 angle = body->GetAngle();
	body->ApplyLinearImpulse(thrustFactor * b2Vec2(dir.x * (IMPULSE + GROOVY_BONUS*grooviness), dir.y * (IMPULSE + GROOVY_BONUS*grooviness)), body->GetPosition(), true);
	//body->ApplyForceToCenter(b2Vec2(thrust.y * 1000 * DCOS_90(angle)  * SHIP_THRUST_FACTOR, thrust.y * 1000 * (-DSIN_90(angle)) * SHIP_THRUST_FACTOR), true);
	//body->ApplyTorque(10*thrust.x, true);
	//body->ApplyAngularImpulse(4000.0f * thrust.x, true);

    // Adjust the active forces.
   // forward = RANGE_CLAMP(forward, -SHIP_MAX_SPEED, SHIP_MAX_SPEED);
   // turning = RANGE_CLAMP(turning, -SHIP_MAX_TURN, SHIP_MAX_TURN);
	if (sprite != NULL && hasEnvironmentWeapon){
		//sprite->setFrame(13);
		sprite->setFrame(0);
	}
    else if (sprite != NULL && hasWeapon) {
		if (frameRate % FRAME_INTERVAL == 0) {
			Vec2* moveDir = new Vec2(round(body->GetPosition().x - lastPosition.x), round(body->GetPosition().y - lastPosition.y));
			advanceFrameShotty(moveDir);
		}
    }
	else if(sprite!=NULL && !hasWeapon) {
		if (frameRate % FRAME_INTERVAL == 0) {
			Vec2* moveDir = new Vec2(round(body->GetPosition().x - lastPosition.x), round(body->GetPosition().y - lastPosition.y));
			advanceFrameEmpty(moveDir);
		}
	}
    
    // Process the ship thrust.
   /* if (forward != 0.0f) {
        // Thrust key pressed; increase the ship velocity.
        velocity.x = forward * DCOS_90(angle)  * SHIP_THRUST_FACTOR;
        velocity.y = forward * (-DSIN_90(angle)) * SHIP_THRUST_FACTOR;
    }
    
    // Move the ship, updating it.
    // Adjust the angle by the change in angle
    angle += turning;  // INVARIANT: -360 < ang < 720
    if (angle > 360) angle -= 360;
    if (angle < 0)   angle += 360;
    
    // Move the ship
    position += velocity;*/
	lastPosition = body->GetPosition();
	return true;
}

/**
 * Determines the next animation frame for the ship and applies it to the sprite.
 *
 * This method includes some dampening of the turn, and should be called before
 * moving the ship.
 */
void Ship::advanceFrameEmpty(Vec2* dir) {
    // Our animation depends on the current frame.
    unsigned int frame = sprite->getFrame();
    
	//if ricky's moving up
	if (dir->y > 0 && dir->x == 0) {
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
	//ricky's moving down
	else if (dir->y < 0 && dir->x == 0) {
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
	//moving to the right
	else if (dir->x > 0 && dir->y == 0) {
		//if it is not the current right frame
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
	//moving to the left
	else if (dir->x < 0 && dir->y == 0) {
		//if it is not the current left frame
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
	//moving righ-down
	else if (dir->x > 0 && dir->y < 0) {
		//if it is not the current left frame
		if (!(frame >= 36 && frame <= 38)) {
			frame = 36;
		}
		//go back to sequence 0
		else if (frame == 37 || frame == 38) {
			frame = 36;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 36 && prevFrame == 37) {
			frame = 38;
		}
		else {
			frame = 37;
		}
	}
	//moving left-down
	else if (dir->x < 0 && dir->y < 0) {
		//if it is not the current left frame
		if (!(frame >= 42 && frame <= 44)) {
			frame = 42;
		}
		//go back to sequence 0
		else if (frame == 43 || frame == 44) {
			frame = 42;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 42 && prevFrame == 43) {
			frame = 44;
		}
		else {
			frame = 43;
		}
	}
	//moving righ-up
	else if (dir->x > 0 && dir->y > 0) {
		//if it is not the current left frame
		if (!(frame >= 30 && frame <= 32)) {
			frame = 30;
		}
		//go back to sequence 0
		else if (frame == 31 || frame == 32) {
			frame = 30;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 30 && prevFrame == 31) {
			frame = 32;
		}
		else {
			frame = 31;
		}
	}
	//moving left-up
	else if (dir->x < 0 && dir->y > 0) {
		//if it is not the current left frame
		if (!(frame >= 24 && frame <= 26)) {
			frame = 24;
		}
		//go back to sequence 0
		else if (frame == 25 || frame == 26) {
			frame = 24;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 24 && prevFrame == 25) {
			frame = 26;
		}
		else {
			frame = 25;
		}
	}
	else {
		frame = sprite->getFrame();
	}
	prevFrame = sprite->getFrame();
    sprite->setFrame(frame);
}

void Ship::advanceFrameShotty(Vec2* dir) {
	// Our animation depends on the current frame.
	unsigned int frame = sprite->getFrame();

	//if ricky's moving up
	if (dir->y > 0 && dir->x == 0) {
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
	//ricky's moving down
	else if (dir->y < 0 && dir->x == 0) {
		//if it is not the current down frame
		if (!(frame >= 3 && frame <= 5)) {
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
	//moving to the right
	else if (dir->x > 0 && dir->y == 0) {
		//if it is not the current right frame
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
	//moving to the left
	else if (dir->x < 0 && dir->y == 0) {
		//if it is not the current left frame
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
	//moving righ-down
	else if (dir->x > 0 && dir->y < 0) {
		//if it is not the current left frame
		if (!(frame >= 39 && frame <= 41)) {
			frame = 39;
		}
		//go back to sequence 0
		else if (frame == 40 || frame == 41) {
			frame = 39;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 39 && prevFrame == 40) {
			frame = 41;
		}
		else {
			frame = 40;
		}
	}
	//moving left-down
	else if (dir->x < 0 && dir->y < 0) {
		//if it is not the current left frame
		if (!(frame >= 45 && frame <= 47)) {
			frame = 45;
		}
		//go back to sequence 0
		else if (frame == 46 || frame == 47) {
			frame = 45;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 45 && prevFrame == 46) {
			frame = 47;
		}
		else {
			frame = 46;
		}
	}
	//moving righ-up
	else if (dir->x > 0 && dir->y > 0) {
		//if it is not the current left frame
		if (!(frame >= 33 && frame <= 35)) {
			frame = 33;
		}
		//go back to sequence 0
		else if (frame == 34 || frame == 35) {
			frame = 33;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 33 && prevFrame == 34) {
			frame = 35;
		}
		else {
			frame = 34;
		}
	}
	//moving left-up
	else if (dir->x < 0 && dir->y > 0) {
		//if it is not the current left frame
		if (!(frame >= 27 && frame <= 29)) {
			frame = 27;
		}
		//go back to sequence 0
		else if (frame == 28 || frame == 29) {
			frame = 27;
		}
		//0-1-0-2-0-1-0-2
		else if (frame == 27 && prevFrame == 28) {
			frame = 29;
		}
		else {
			frame = 28;
		}
	}
	else {
		frame = sprite->getFrame();
	}
	prevFrame = sprite->getFrame();
	sprite->setFrame(frame);
}

void Ship::addShrapnelParticles(float a){
	ParticleSystemQuad *emitter = ParticleSystemQuad::create();
	emitter->setDuration(0.17f);
	emitter->setLife(0.17f);
	emitter->setLifeVar(0.05f);
	emitter->setSpeed(1000.0f);
	emitter->setSpeedVar(80.0f);
	emitter->setAngle(a);
	emitter->setAngleVar(35.0f);
	emitter->setPosVar(Vec2(5, 5));
	//emitter->setSpeed(0.0f);
	//emitter->setSpeedVar(0.0f);
	emitter->setEmissionRate(10000.0f);
	emitter->setAutoRemoveOnFinish(true);
	emitter->setGravity(ccp(0, 0));
	emitter->setTangentialAccelVar(0);
	emitter->setRadialAccelVar(0);
	/*emitter->setSpeed(1000.0f);

	emitter->setEmissionRate(5.0f);


	emitter->setRadialAccelVar(200.0f);*/
	emitter->setRadialAccel(0);
	emitter->setTangentialAccel(0);
	emitter->setAnchorPoint(Vec2(0.5f, 0.5f));
	emitter->setPosition(sprite->getContentSize() / 2);
	emitter->setStartSize(3.0);
	emitter->setEndSize(5.0);
	emitter->setStartSizeVar(1.0);
	emitter->setEndSizeVar(2);
	emitter->setBlendAdditive(true);
	//emitter->setBlendAdditive(false);
	//emitter->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);

	emitter->setTexture(ResourceLoader::getInstance()->getTexture("shrapnel"));
	emitter->setStartColorVar(ccColor4F(1.0f, 1.0f, 1.0f, 0));
	emitter->setEndColorVar(ccColor4F(0.7f, 0.7f, 0.7f, 0));
	emitter->setStartColor(ccColor4F(0.5f, 0.5, 0.5f, 0.5f));
	emitter->setEndColor(ccColor4F(0.5f, 0.5f, 0.5f, 0.0f));
	sprite->addChild(emitter, 1);
}

void Ship::addDustParticles(){
	CCParticleSun *emitter = CCParticleSun::createWithTotalParticles(100);
	emitter->setAutoRemoveOnFinish(true);
	emitter->setDuration(0.06f);
	emitter->setLife(0.7);
	emitter->setLifeVar(0.1);
	emitter->setSpeed(20.0f);
	emitter->setSpeedVar(7.0f);
	emitter->setPosVar(Vec2(15, 15));
	//emitter->setSpeed(0.0f);
	//emitter->setSpeedVar(0.0f);
	emitter->setEmissionRate(90.0f);

	emitter->setGravity(ccp(0, -3));
	//emitter->setTangentialAccelVar(10.0f);
	//emitter->setRadialAccelVar(10.0f);
	/*emitter->setSpeed(1000.0f);

	emitter->setEmissionRate(5.0f);


	emitter->setRadialAccelVar(200.0f);*/
	//emitter->setRadialAccel(2.0f);
	//emitter->setTangentialAccelVar(200.0f);
	emitter->setAnchorPoint(Vec2(0.5f, 0.5f));
	float w = sprite->getContentSize().width / 2;
	float h = 4;
	emitter->setPosition(w, h);
	emitter->setStartSize(7.0);
	emitter->setEndSize(6.5);
	emitter->setStartSizeVar(2.0);
	emitter->setEndSizeVar(1);
	emitter->setBlendAdditive(false);
	emitter->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);

	emitter->setTexture(ResourceLoader::getInstance()->getTexture("dust"));
	emitter->setStartColorVar(ccColor4F(0.25f, 0.25f, 0.25f, 0.2f));
	emitter->setEndColorVar(ccColor4F(0.2f, 0.2f, 0.2f, 0.2f));
	emitter->setStartColor(ccColor4F(1.0f, 1.0f, 1.0f, 0.5f));
	emitter->setEndColor(ccColor4F(0.8f, 0.8f, 0.8f, 0.5f));
	sprite->addChild(emitter, 1);
}
