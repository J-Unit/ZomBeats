#pragma once
#include <string>
#include "json\document.h"


class GameController;
class AudioController;

class SaveSerializer
{
public:
	rapidjson::Document d; //for calibration
	rapidjson::Document e; //for save level

	//for calibration
	void parseSave(GameController *gc, std::string f);
	void parseCalibration(AudioController *c);
	void exportSave(GameController *gc);

	//for save level
	int parseLevel(std::string f);
	void exportLevel(int level);
	static int curMaxLevel;
};

