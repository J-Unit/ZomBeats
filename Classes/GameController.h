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
#include <vector>
#include "util.h"
#include "FilmStrip.h"
#include "SaveSerializer.h"

//fog scale
#define FOG_SCALE 1.25f
#define OUTER_FOG_SCALE 2.8f
#define PAUSE_BUTTON_HALF_WIDTH 15
#define PAUSE_BUTTON_HALF_HEIGHT 15
#define GAME_MENU_SCALE 0.2f
#define GAME_MENU_BUTTON_SCALE 0.75f
#define GAME_MENU_BUTTON_OFFSET 160
#define DIALOGUE_POPUP_SCALE 0.13f
#define COUNT_DOWN_SCALE 0.6f
#define PANNING_SCALE 0.3f
#define GAME_VIEW_SCALE 0.7f

#define SAVE_FILE "save.zbs"

//delays
#define ENVIRONMENTAL_WEAPON_DELAY 1.0f
#define TRASHCAN_CD 8.0f
#define ENVIRONMENTAL_WEAPON_DELAY_MAX 5.0f

//environment activation shizz
#define ENVIRONMENT_METER_LENGTH 200.0f
#define ENVIRONMENT_METER_WIDTH 30.0f
#define ENVIRONMENT_METER_INC 0.35f 
#define ENVIRONMENT_METER_DEC_OFFBEAT 0.17f
#define ENVIRONMENT_METER_DEC 0.002f

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
class AudioController;
class EnvironmentWeapon;
class Lawnmower;
class Trashcan;
class Trash;
class GrooveMeter;
class CalibrationController;

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
	//the count down
	FilmStrip* countDown;
	int countDownCounter;

    // MODEL
    // A page-out could dispose of the view as long as it just has this.
	SongDecomposition*  currentSong;
	AIController *ai;
	CalibrationController *calibration;
	AudioController *audio;
	GameState *state;
	// Box2D world for physics, collisions etc.
	MapNode* destination;
	//Elapsed time of game
	float elapsedTime;


	//zombie awarness output, remove it later
	float currAwareness;

	//current finger position for drawing activation arrow
	Vec2 currentFingerPos;

	//timer used to signal if safe to choose environmental object direction
	float environmentalTimer;
	float trashTimer;

	//env activation
	float currentEnvironmentMeter;

	bool processDirection;
	bool trashOnCD;
	bool doneActivating;

	EnvironmentWeapon *currentEnvironment;
	Lawnmower *currentMower;
	Trash *currentTrash;

    /** 
     * Writes the current ship position to the HUD.
     *
     * @param label The HUD label
     * #param coords The current ship coordinates
     */
    void displayPosition(Label* label, const b2Vec2& coords);
	void startVideoCalibration();
	bool hasWonLevel();

	/**
	* Initialize the game state.
	*
	* This is an overridden method inherited from the super(super) class Node.
	* The Director calls this soon after start up.
	*/
	bool init() override;

	/**
	* Intialize the environment of the game, this is the real method that populates the game
	* state and level
	*/
	void initEnvironment();

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

	void goBackToMainMenu(); //used for in pause menu to go back to level select and stuff

	void pauseGame();

	void resumeGame();

	void pauseGameOnly();

	void resumeGameOnly(int index);

	b2Vec2 weaponRectangle[4];

	b2Vec2 weaponDetectionRectangle[4];

	b2Vec2 outterEnvironmentActivation[4];

	b2Vec2 innerEnvironmentActivation[4];

	Vec2 transformedInnerEnvironmentActivation[4];

	Vec2 transformedOuterEnvironmnetActivation[4];

	Vec2 *dRickyTap;

	Vec2 mouseToWorld(Vec2 click);

	void drawMusicNotePath();
	Sprite *musicNotes[MAX_MUSIC_NOTES];

	int musicNoteCounter;

	bool activationDelay;

	int currentLevel;
	void setCurrentLevel(int level);

private:
	void createZombies();
	void createWalls();
	void createWeapons();
	void createFog();
	void createPauseButton();
	void createOkButton(int index);
	void createGameMenu();
	void removeGameMenu();
	void updateFog();
	void createWeaponRanges(float weapWidth, float weapRange, float weapDetectionRange, b2Vec2 dir);
	void createEnvironmentActivationMeter(float meterWidth, float meterLength, float percentFull, b2Vec2 dir);
	bool isZombieHit(b2Vec2 az, b2Vec2 bz, b2Vec2 ab, b2Vec2 bc);
	void createCountDown();
	void panning();
	void unPanning();

	void removeDeadWeapons();
	void removeDeadEWeapons();
	void removeDeadZombies();
	void removeDyingZombies();

	//the dialogue popup used in calibration level
	Sprite* popup;
	//the okay button menu used in calibration menu
	Menu* okButtonMenu;

	LevelSerializer ls;
	SaveSerializer save;
	bool removed;

	//used for the initial pause and begin count down
	bool beginCountDown;

};

#endif /* defined(__TD_GAME_CONTROLLER_H__) */
