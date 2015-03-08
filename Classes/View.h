#include <stdio.h>
#include "cocos2d.h"
#include <vector>

#define SPACE_TILE   256
#define TILE_AMOUNT	 10
#define BLOCKS_X 50
#define BLOCKS_Y 50
#define PLANET_SCALE 0.5f
#define PARALLAX_AMT 0.0f
#define PLANET1_POS  Vec2(950, 1100)
#define PLANET2_POS  Vec2(1600, 1500)
#define HUD_OFFSET   Vec2(10.0f,10.f)
#define WORLD_SIZE  SPACE_TILE * TILE_AMOUNT

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
	Label*  coordHUD;
	Label*	velHUD;
	Label*	thrustHUD;
	Label*  beatHUD;
	Label*  mainBeatHUD;
	Label*  detectionRadiusHUD;
	/** Node to hold all of our graphics. Necesary for resolution indepedence. */
	Node*   allSpace;
	/** Background in animation parallax. Stores the field of stars */
	Node*   background;
	/** Foreground in animation parallax. Stores the planets. */
	//Node*   nearSpace;

	DrawNode* meshVis;
	DrawNode* path;
	DrawNode* detectionRadiusCircle;

	Node*   enviornment;

	View(int w, int h);

	int screen_size_x;
	int screen_size_y;
	double pix_to_opengl_scale;

	/** Filmstrip representing the animated ship */
	FilmStrip* shipImage;
	/** A wall tile */
	Sprite* wallTile;
	Sprite* wallTile2;
	vector<Sprite*> walls;


	/**
	* Builds the scene graph for the game.
	*
	* We factored this out of init() to allow us to page-in and page-out
	* the scene graph at a later time.
	*/
	void buildScene(LevelMap *n, Layer* l);

	/**
	* Creates a new game with an instance of this controller class.
	*
	* This static controller exists because we do not return the GameController
	* to the caller; we return a Scene that has the GameController as an immediate
	* child. That is because a Scene anchors to the drawing window for display.
	*/
	cocos2d::Scene* createScene();

	~View();
};

