#include <string>
#include "json\document.h"
#include "Box2D\Box2D.h"

class GameState;
class SongDecomposition;

class LevelSerializer{
public:
	rapidjson::Document d;
	SongDecomposition *getLevelTrack();
	GameState *parseLevel(std::string f);
	void addObjects(GameState *s);
	float getWallOffsetByType(std::string type);
};
