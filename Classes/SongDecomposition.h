#include <string>

#define ERROR_WINDOW 0.06 //in seconds

class SongDecomposition
{
public:
	float bpm;
	std::string trackName;
	float trackStartTime;

	SongDecomposition(float beats, std::string name, float start) {
		bpm = beats;
		trackName = name;
		trackStartTime = start;
	}

	bool isOnBeat(float clickTime);

	~SongDecomposition();
};

