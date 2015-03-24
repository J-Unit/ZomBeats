//
//  Ship.h
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#ifndef __TD_SHIP_H__
#define __TD_SHIP_H__

#include <stdio.h>
#include <cocos2d.h>
#include "Box2D/Box2D.h"
#include "BoxObject.h"

using namespace cocos2d;

// Ship Frame Sprite numbers
#define SHIP_IMG_LEFT   0
#define SHIP_IMG_FLAT   9
#define SHIP_IMG_RIGHT 17
#define MAX_BOOST_FRAMES 45
#define FRAME_INTERVAL 10


// Forward declaration to minimize imports in header
class FilmStrip;

/**
 * Model class represting a ship here.
 *
 * All positional information about the ship goes in this class and not
 * in the sprite node.  That is because we are not animating the ship
 * (other than using the filmstrip to show banking).  We are animating
 * the background.  This forces us to decouple the model from the sprite.
 *
 * You should ALWAYS do this. If you do not do this, I will be most
 * displeased with you.
 */
class Ship : public BoxObject{
private:
    /** 
     * Determines the next animation frame for the ship and applies it to the sprite.
     *
     * This method includes some dampening of the turn, and should be called before
     * moving the ship.
     */
    void advanceFrame(Vec2* dir);

protected:

    /** Position of the ship in world space */
   // Vec2    position;
    /** Current ship velocity */
    //Vec2    velocity;
    /** Angle of the ship in the world space */
   // float   angle;
    /** Current turning thrust (stored to facilitate decay) */
   // float       turning;
    /** Current forward thrust (stored to facilitate decay) */
    //float       forward;
    /** Reference to image in SceneGraph for animation */
    

    
public:
    /**
     * Creates a new ship at the origin
     */
    Ship(b2World *world);
	bool isDestroyed;
	bool hasWeapon;
	bool isActivatingEnvironment;
	Weapon *currentWeapon;
	b2PolygonShape dynamicBox; 
	int boostFrames;
	int frameRate;
	b2Vec2 lastPosition;
	FilmStrip* sprite;
    /**
     * Creates a new ship at the specified position (in world space).
     *
     * @param x The world space x-coordinate
     * @param y The world space y-coordinate
     */
    Ship(b2World *world, float x, float y, float mx, float my);
    
    /**
     * Disposes the ship, releasing all resources.
     */
    ~Ship();
    
    // SHORT METHODS CAN BE IN-LINED IN C++
    
    /**
     * Returns the ship velocity as a reference. This allows us to modify
     * the value.
     *
     * @return The ship velocity.
     */
    //Vec2&   getVelocity()           { return velocity;      }

    /**
     * Returns the ship position as a reference. This allows us to modify
     * the value.
     *
     * @return The ship position.
     */
    //Vec2&   getPosition()           { return position;      }
    
    /**
     * Returns the current angle of the ship.
     *
     * @return The ship angle
     */
    //float   getAngle()              { return angle;         }

    /**
     * Sets the current angle of the ship.
     *
     * @param value The ship angle
     */
    //void    setAngle(float value)   { angle = value;        }
    
    /**
     * Returns the current turning force on the ship
     *
     * @return The ship turning force
     */
    //float   getTurning()            { return turning;       }
    
    /**
     * Sets the current turning force on the ship
     *
     * @param value The ship turning force
     */
    //void    setTurning(float value) { turning = value;      }

    /**
     * Returns the current forward force on the ship
     *
     * @return The ship forward force
     */
    //float   getForward()            { return forward;       }

    /**
     * Sets the current forward force on the ship
     *
     * @param value The ship forward force
     */
    //void    setForward(float value) { forward = value;      }
    
    /**
     * Returns the film strip representing this ship.
     *
     * It returns NULL if there is no active film strip.
     *
     * @return The ship film strip.
     */
    FilmStrip* getSprite()          { return sprite;        }
    
    /**
     * Sets the film strip representing this ship.
     *
     * Setting this to NULL clears the value
     *
     * @param value The ship film strip.
     */
    void    setSprite(FilmStrip* value, float mx, float my);
    
    /**
     * Updates the state of the model
     *
     * This method moves the ship forward, dampens the forces (if necessary)
     * and updates the sprite if it exists.
     *
     * @param deltaTime Time elapsed since last called.
     */
    bool   update(float deltaTime, Vec2 thrust);    
};

#endif /* defined(__TD_SHIP_H__) */
