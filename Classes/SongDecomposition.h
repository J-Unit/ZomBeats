#include <string>

#define ERROR_WINDOW 0.08 //in seconds

class SongDecomposition
{
public:
	float bpm; //beats per minute
	float spb; //
	std::string trackName;
	float trackStartTime;

	SongDecomposition(float beats, std::string name, float start) {
		bpm = beats;
		spb = 60 / bpm;
		trackName = name;
		trackStartTime = start;
	}

	bool isOnBeat(float clickTime);
	float getBeatStart(float time);
	float timeToClosestBeat(float time);
	~SongDecomposition();
};

