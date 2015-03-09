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

// Macros to eliminate magic numbers
// Some of these are carried over from the 3152 lab and unused.
#define ANGLE_RADIANS (PI/180.0f)
#define SHIP_MAX_TURN      1.0f
#define SHIP_MAX_SPEED    10.0f
#define SHIP_THRUST_FACTOR 0.4f
#define IMPULSE 7000

#define DCOS_90(a)  (cos(M_PI*(a+90.0f)/180.0f))
#define DSIN_90(a)  (sin(M_PI*(a+90.0f)/180.0f))

#define RANGE_CLAMP(x,y,z)  (x < y ? y : (x > z ? z : x))

/**
 * Creates a new ship at the origin
 */
Ship::Ship(b2World *world) : Ship::Ship(world, 0.0f, 0.0f) { }

/**
 * Creates a new ship at the specified position (in world space).
 *
 * @param x The world space x-coordinate
 * @param y The world space y-coordinate
 */
Ship::Ship(b2World *world, float x, float y) {
   // position.set(x,y);
   // velocity.set(0.0f,0.0f);
    
   // angle   = 0.0f;
   // turning = 0.0f;
   // forward = 0.0f;
	isDestroyed = false;
	hasWeapon = false;
	currentWeapon = NULL;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.fixedRotation = true;
	body = world->CreateBody(&bodyDef);
	type = Type(this);
	body->SetUserData(&type);
	dynamicBox.SetAsBox(60.0f, 80.0f);
	fixture.shape = &dynamicBox;
	fixture.density = 0.1f;
	fixture.friction = 0.5f;
	body->CreateFixture(&fixture);
	body->SetLinearDamping(3.0f);
	//body->SetAngularDamping(0.5f);
    sprite  = NULL;
	setSprite(FilmStrip::create(ResourceLoader::getInstance()->getTexture("ricky"), 1, 2, 2));
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
}

/**
 * Sets the film strip representing this ship.
 *
 * Setting this to NULL clears the value
 *
 * @param value The ship film strip.
 */
void Ship::setSprite(FilmStrip* value) {
    // Release the current sprite if we have a reference
    if (sprite != NULL) {
        sprite->release();
    }
    sprite = value;
	sprite->setScale(0.8f);
    if (sprite != NULL) {
        sprite->retain(); // Do not delete it until we are done.
        sprite->setFrame(SHIP_IMG_FLAT);
        sprite->setPosition(Vec2(body->GetPosition().x, body->GetPosition().y));
        sprite->setAnchorPoint(Vec2(0.5f,0.5f));
    }
}

/**
 * Updates the state of the model
 *
 * This method moves the ship forward, dampens the forces (if necessary)
 * and updates the sprite if it exists.
 *
 * @param deltaTime Time elapsed since last called.
 */
void Ship::update(float deltaTime, Vec2 dir) {
    
	//float32 angle = body->GetAngle();
	body->ApplyLinearImpulse(b2Vec2(dir.x * IMPULSE, dir.y * IMPULSE), body->GetPosition(), true);
	//body->ApplyForceToCenter(b2Vec2(thrust.y * 1000 * DCOS_90(angle)  * SHIP_THRUST_FACTOR, thrust.y * 1000 * (-DSIN_90(angle)) * SHIP_THRUST_FACTOR), true);
	//body->ApplyTorque(10*thrust.x, true);
	//body->ApplyAngularImpulse(4000.0f * thrust.x, true);

    // Adjust the active forces.
   // forward = RANGE_CLAMP(forward, -SHIP_MAX_SPEED, SHIP_MAX_SPEED);
   // turning = RANGE_CLAMP(turning, -SHIP_MAX_TURN, SHIP_MAX_TURN);
    if (sprite != NULL && hasWeapon) {
		sprite->setFrame(1);
    }
	else if(sprite!=NULL && !hasWeapon) {
		sprite->setFrame(0);
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
}

/**
 * Determines the next animation frame for the ship and applies it to the sprite.
 *
 * This method includes some dampening of the turn, and should be called before
 * moving the ship.
 */
/*void Ship::advanceFrame() {
    // Our animation depends on the current frame.
    unsigned int frame = sprite->getFrame();
    
	float32 turning = RANGE_CLAMP(body->GetAngularVelocity()/20, -SHIP_MAX_TURN, SHIP_MAX_TURN);
    // Process the ship turning.
    if (turning < 0.0f) {
        float offset = (turning/SHIP_MAX_TURN)*(SHIP_IMG_FLAT-SHIP_IMG_RIGHT);
        unsigned int goal  = SHIP_IMG_FLAT+offset;
        if (frame != goal) {
            frame += (frame < goal ? 1 : -1);
        }
        if (frame == SHIP_IMG_FLAT) {
            turning = 0.0f;
        }
    } else if (turning > 0.0f) {
        float offset = (turning/SHIP_MAX_TURN)*(SHIP_IMG_FLAT-SHIP_IMG_LEFT);
        unsigned int goal  = SHIP_IMG_FLAT-offset;
        if (frame != goal) {
            frame += (frame < goal ? 1 : -1);
        }
        if (frame == SHIP_IMG_FLAT) {
            turning = 0.0f;
        }
    } else {
        if (frame < SHIP_IMG_FLAT) {
            frame++;
        } else if (frame > SHIP_IMG_FLAT) {
            frame--;
        }
    }
    
    sprite->setFrame(frame);
}*/

