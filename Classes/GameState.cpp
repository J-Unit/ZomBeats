#include "GameState.h"
#include "LevelMap.h"

GameState::GameState(int tilesX, int tilesY, int nodesX, int nodesY)
{
	world = new b2World(b2Vec2(0.0f, 0.0f));
	//world->SetContinuousPhysics(false);
	level = new LevelMap(tilesX, tilesY, nodesX, nodesY);
	ship = NULL;
	instrument = NULL;
	levelTip = "";
}


GameState::~GameState()
{
	delete ship;
	for (CTypedPtrDblElement<Zombie> *z = zombies.GetHeadPtr(); !zombies.IsSentinel(z); z = z->Next()) delete z->Data();
	for (CTypedPtrDblElement<Weapon> *w = weapons.GetHeadPtr(); !weapons.IsSentinel(w); w = w->Next()) delete w->Data();
	for (CTypedPtrDblElement<EnvironmentWeapon> *w = environment_weapons.GetHeadPtr(); !environment_weapons.IsSentinel(w); w = w->Next()) delete w->Data();
	delete instrument;
	delete level;
	delete world;
}
