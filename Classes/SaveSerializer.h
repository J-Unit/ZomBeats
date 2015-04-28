#pragma once
#include <string>
#include "json\document.h"


class GameController;
class AudioController;

class SaveSerializer
{
public:
	rapidjson::Document d;

	void parseSave(GameController *gc, std::string f);
	void parseCalibration(AudioController *c);
	void exportSave(GameController *gc);
};

