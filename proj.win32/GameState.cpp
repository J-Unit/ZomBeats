



/**
* Creates a new game with an instance of this controller class.
*
* This static controller exists because we do not return the GameController
* to the caller; we return a Scene that has the GameController as an immediate
* child. That is because a Scene anchors to the drawing window for display.
*/
Scene* GameState::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' (this object) is an autorelease object
	auto layer = GameController::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}