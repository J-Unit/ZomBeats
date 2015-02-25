//
//  GameController.h
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#ifndef __TD_GAME_CONTROLLER_H__
#define __TD_GAME_CONTROLLER_H__

#include <stdio.h>
#include "Box2D.h"
#include "LevelMap.h"

using namespace cocos2d;
using namespace std;

// Forward declarations of classes for field variables.
// It is better to do this in C++ instead of including headers inside headers
class InputController;
class FilmStrip;
class Ship;

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
protected:
    // CONTROLLERS
    /** Controller for abstracting out input away from layer */
    InputController* input;

	void GameController::BeginContact(b2Contact* contact);
	void GameController::EndContact(b2Contact* contact);


	int screen_size_x;
	int screen_size_y;
	double pix_to_opengl_scale;

    // VIEW
    /** Filmstrip representing the animated ship */
    FilmStrip* shipImage;
    /** Label for on-screen coordinate HUD */
    Label*  coordHUD;
	Label*	velHUD;
	Label*	thrustHUD;
    /** Node to hold all of our graphics. Necesary for resolution indepedence. */
    Node*   allSpace;
    /** Background in animation parallax. Stores the field of stars */
    Node*   background;
    /** Foreground in animation parallax. Stores the planets. */
    //Node*   nearSpace;

	DrawNode* meshVis;
	DrawNode* path;

	Node*   enviornment;

    // MODEL
    // A page-out could dispose of the view as long as it just has this.
    /** The current coordinates of the ship */
    Ship*   shipModel;
   
	// Box2D world for physics, collisions etc.
	b2World* world;

	//Movement stuff
	LevelMap* level;
	MapNode* destination;

    /** 
     * Writes the current ship position to the HUD.
     *
     * @param label The HUD label
     * #param coords The current ship coordinates
     */
    void displayPosition(Label* label, const b2Vec2& coords);
    
    /**
     * Builds the scene graph for the game.
     * 
     * We factored this out of init() to allow us to page-in and page-out
     * the scene graph at a later time.
     */
    void buildScene();
    
public:
    /**
     * Creates a new game with an instance of this controller class.
     *
     * This static controller exists because we do not return the GameController
     * to the caller; we return a Scene that has the GameController as an immediate
     * child. That is because a Scene anchors to the drawing window for display.
     */
    static cocos2d::Scene* createScene(int w, int h);

    /** 
     *  This macro implements the "static create()" method.
     *
     *  This class is a subclass of a Scene Graph nodes.  All of the Scene Graph
     *  nodes support reference counting.  That means you should never implement
     *  a constructor or a destructor; force the user to go through static methods
     *  that apply the reference counting rules.
     */
    CREATE_FUNC(GameController);

    /**
     * Initialize the game state.
     *
     * This is an overridden method inherited from the super(super) class Node.
     * The Director calls this soon after start up.
     */
    bool init() override;
    
    /**
     * Update the game state.
     *
     * This method is called every animation frame.  There is no draw() or render()
     * counterpoint to this method; drawing is done automatically in the scene graph.
     * However, this method is responsible for updating any transforms in the scene graph.
     */
    void update(float dt) override;

};

#endif /* defined(__TD_GAME_CONTROLLER_H__) */
