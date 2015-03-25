#include <string>
#include "cocos2d/external/json/document.h"
#include "cocos2d\external\Box2D\Box2D.h"

class GameState;

class LevelSerializer{
public:
	rapidjson::Document d;
	GameState *parseLevel(std::string f);
	void addObjects(GameState *s);
	float getWallOffsetByType(std::string type);
};