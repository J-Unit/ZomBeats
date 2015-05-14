#include "SaveSerializer.h"
#include "GameController.h"
#include "AudioController.h"
#include "json\stringbuffer.h"
#include "json\writer.h"
#include <fstream>

void SaveSerializer::parseSave(GameController *gc, std::string f){
	f = cocos2d::FileUtils::getInstance()->getWritablePath() + f;
	if (FileUtils::getInstance()->isFileExist(f)){
		ssize_t bufsize = 1024 * 1024;
		std::string zz = FileUtils::getInstance()->getStringFromFile(f);
		const char *data = zz.c_str();
		d.Parse<0>(data);
		parseCalibration(gc->audio);
	}
}

void SaveSerializer::parseCalibration(AudioController *c){
	c->audioDelay = d["audio_delay"].GetDouble();
	c->videoDelay = d["video_delay"].GetDouble();
}

void SaveSerializer::exportSave(GameController *gc){
	d.SetObject();
	d.AddMember("audio_delay", gc->audio->audioDelay, d.GetAllocator());
	d.AddMember("video_delay", gc->audio->videoDelay, d.GetAllocator());

	//d["audio_delay"].SetDouble(gc->audio->audioDelay);
	//d["video_delay"].SetDouble(gc->audio->videoDelay);

	char cbuf[1024]; rapidjson::MemoryPoolAllocator<> allocator(cbuf, sizeof cbuf);
	typedef rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>> StringBuffer;
	StringBuffer buf(&allocator);
	rapidjson::Writer<StringBuffer> writer(buf, &allocator);
	d.Accept(writer);
	std::string json(buf.GetString(), buf.Size());
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + SAVE_FILE;
	FILE *fp = fopen(path.c_str(), "w");
	//std::ofstream of(SAVE_FILE);
	//of << json;
	//if (!of.good()) throw std::runtime_error("Can't write the JSON string to the file!");
	fputs(json.c_str(), fp);
	fclose(fp);
}

int SaveSerializer::parseLevel(std::string f){
	f = cocos2d::FileUtils::getInstance()->getWritablePath() + f;
	if (FileUtils::getInstance()->isFileExist(f)){
		ssize_t bufsize = 1024 * 1024;
		std::string zz = FileUtils::getInstance()->getStringFromFile(f);
		const char *data = zz.c_str();
		e.Parse<0>(data);
		return e["level_completed"].GetInt();
	}
	//file doesn't exist yet
	else {
		exportLevel(0);
		return 0;
	}
}


void SaveSerializer::exportLevel(int level){
	e.SetObject();
	e.AddMember("level_completed", level, e.GetAllocator());


	char cbuf[1024]; rapidjson::MemoryPoolAllocator<> allocator(cbuf, sizeof cbuf);
	typedef rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>> StringBuffer;
	StringBuffer buf(&allocator);
	rapidjson::Writer<StringBuffer> writer(buf, &allocator);
	e.Accept(writer);
	std::string json(buf.GetString(), buf.Size());
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + SAVE_LEVEL_FILE;
	FILE *fp = fopen(path.c_str(), "w");
	//std::ofstream of(SAVE_FILE);
	//of << json;
	//if (!of.good()) throw std::runtime_error("Can't write the JSON string to the file!");
	fputs(json.c_str(), fp);
	fclose(fp);
}