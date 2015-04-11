//
//  GameController.h
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#ifndef __TD_GAME_CONTROLLER_H__
#define __TD_GAME_CONTROLLER_H__

#include <stdio.h>
//#include "Box2D.h"
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "LevelSerializer.h"

//fog scale
#define FOG_SCALE 1.25f
#define OUTER_FOG_SCALE 2.8f
#define PAUSE_BUTTON_HALF_WIDTH 15
#define PAUSE_BUTTON_HALF_HEIGHT 15
#define GAME_MENU_SCALE 0.15f
#define GAME_MENU_BUTTON_SCALE 0.75f
#define GAME_MENU_BUTTON_OFFSET 150


//delays
#define ENVIRONMENTAL_WEAPON_DELAY 1.0f
#define ENVIRONMENTAL_WEAPON_DELAY_MAX 5.0f

using namespace cocos2d;
using namespace std;

// Forward declarations of classes for field variables.
// It is better to do this in C++ instead of including headers inside headers
class InputController;
class Ship;
class SongDecomposition;
class GameState;
struct MapNode;
class View;
class AIController;
class EnvironmentWeapon;
class Lawnmower;
class GrooveMeter;

/**
 * Class represents the root node for the game engine.
 * 
 * As we have mentioned in class, Cocos2D has decided to violate MVC separation
 * in horrible, horrible ways.  To get a game on a 2D device, you NEED to have
 * a layer.  The layer is what (1) displays the graphics, (2) receives the
 * touch inputs, and (3) has an update method that is called regularly.
 *
 * Instead of fighting it, we simply make the Layer class the root of our
 * game engine.  However, from this point onward, we obey MVC separation
 * as much as possible.
 */
class GameController : public Layer, public b2ContactListener{
public:
    // CONTROLLERS
    /** Controller for abstracting out input away from layer */
    InputController* input;

	//void GameController::BeginContact(b2Contact* contact);
	//void GameController::EndContact(b2Contact* contact);
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	//for game menu use
	bool isPaused;

	GrooveMeter *meter;

	/**
    // VIEW
    /** Filmstrip representing the animated ship 
    */
	View* view;

	//fog sprite
	Sprite* fogSp;
	Sprite* fogSpOuter;

	//game menu sprite and associated buttons
	Sprite* pauseMenuBackground;
	//the in-game menu
	Menu* menu;

    // MODEL
    // A page-out could dispose of the view as long as it just has this.
    /** The current coordinates of the ship */
	int curLevel;
	SongDecomposition*  currentSong;
	AIController *ai;
	GameState *state;
	// Box2D world for physics, collisions etc.
	MapNode* destination;
	//Elapsed time of game
	float elapsedTime;

	//Whether the user is on beat or not
	bool onBeat;

	//zombie awarness output, remove it later
	float currAwareness;

	//current finger position for drawing activation arrow
	Vec2 currentFingerPos;

	//timer used to signal if safe to choose environmental object direction
	float environmentalTimer;

	bool processDirection;
	bool prevOnBeat;
	bool frameOnBeat;

	EnvironmentWeapon *currentEnvironment;
	Lawnmower *currentMower;

    /** 
     * Writes the current ship position to the HUD.
     *
     * @param label The HUD label
     * #param coords The current ship coordinates
     */
    void displayPosition(Label* label, const b2Vec2& coords);
    
	/**
	* Initialize the game state.
	*
	* This is an overridden method inherited from the super(super) class Node.
	* The Director calls this soon after start up.
	*/
	bool init() override;

	CREATE_FUNC(GameController);
    
    /**
     * Update the game state.
     *
     * This method is called every animation frame.  There is no draw() or render()
     * counterpoint to this method; drawing is done automatically in the scene graph.
     * However, this method is responsible for updating any transforms in the scene graph.
     */
    void update(float dt) override;

	void loadLevel(int i);

	void restartGame();

	void pauseGame();

	void resumeGame();

	//helper method, check if user has clicked the pause botton during game
	bool hasPressedPauseButton();

	b2Vec2 weaponRectangle[4];

	Vec2 *dRickyTap;

	Vec2 mouseToWorld(Vec2 click);



private:
	void createZombies();
	void createWalls();
	void createWeapons();
	void createFog();
	void createGameMenu();
	void removeGameMenu();
	void updateFog();
	void createWeaponRanges(float weapWidth, float weapRange, b2Vec2 dir);
	bool isZombieHit(b2Vec2 az, b2Vec2 bz, b2Vec2 ab, b2Vec2 bc);

	LevelSerializer ls;

};

#endif /* defined(__TD_GAME_CONTROLLER_H__) */
