
#include "cocos2d.h"
#include "Box2D.h"

using namespace cocos2d;

class GameController;


class GameState {
protected: 
	GameController* controller;
	Scene* scene;
	b2World* world;
};

/**
* Creates a new game with an instance of this controller class.
*
* This static controller exists because we do not return the GameController
* to the caller; we return a Scene that has the GameController as an immediate
* child. That is because a Scene anchors to the drawing window for display.
*/
static cocos2d::Scene* createScene();

