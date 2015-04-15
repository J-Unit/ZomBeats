#include <stdio.h>
#include "cocos2d.h"
#include <vector>

#ifndef VIEW_H
#define VIEW_H

#define VIEW_WDITH 800.0f
#define VIEW_HEIGHT 600.0f
#define LETTERBOX_EPSILON 0.00001f

#define SPACE_TILE   256
#define PARALLAX_AMT 0.0f
#define PLANET1_POS  Vec2(950, 1100)
#define PLANET2_POS  Vec2(1600, 1500)
#define HUD_OFFSET   Vec2(10.0f,10.0f)
#define SHAKE_STRENGTH 4.0f

using namespace cocos2d;
using namespace std;

class FilmStrip;
class LevelMap;
struct b2Vec2;


class View : public Layer
{
public:
	// VIEW
	Scene* scene;
	
	/** Label for on-screen coordinate HUD */
	Node* resIndepScreen;
	Label*  coordHUD;
	Label*	velHUD;
	Label*	thrustHUD;
	Label*  beatHUD;
	Label*  mainBeatHUD;
	Label*  detectionRadiusHUD;
	Label*  zombieOneAwarenessHUD; //remove this later
	/** Node to hold all of our graphics. Necesary for resolution indepedence. */
	Node*   allSpace;
	/** Background in animation parallax. Stores the field of stars */
	Node*   background;
	/** Foreground in animation parallax. Stores the planets. */
	//Node*   nearSpace;

	DrawNode* meshVis;
	DrawNode* path;
	DrawNode* detectionRadiusCircle;
	DrawNode* directionUseEnvironmentWeapon;
	DrawNode* hitBox;
	DrawNode* weaponBox;
	DrawNode* ai;
	DrawNode* zombiePositions;
	Label* grooviness;
	Label* durability;
	DrawNode* durabilityBox;
	DrawNode* meter;
	DrawNode* durabilityHolder;
	DrawNode* durabilitySpriteContainer;
	Vec2 shakeCenter;
	Label* objective;

	Node*   enviornment;
	Node* zombies;

	View(int w, int h);

	int screen_size_x;
	int screen_size_y;
	double pix_to_opengl_scale;

	/** Filmstrip representing the animated ship */
	FilmStrip* shipImage;



	/**
	* Builds the scene graph for the game.
	*
	* We factored this out of init() to allow us to page-in and page-out
	* the scene graph at a later time.
	*/
	void buildScene(LevelMap *n, Layer* l, int level);

	void setMesh();

	void shake(float start, float now, Vec2 dir);

	/**
	* Creates a new game with an instance of this controller class.
	*
	* This static controller exists because we do not return the GameController
	* to the caller; we return a Scene that has the GameController as an immediate
	* child. That is because a Scene anchors to the drawing window for display.
	*/
	cocos2d::Scene* createScene();
	void releaseScene();
	~View();

private:
	void makeResolutionIndependent(Node *n);
};

#endif