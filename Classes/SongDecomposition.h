#include <string>

//#define ERROR_WINDOW 0.08 //in seconds
//#define EIGHTH_NOTE_WINDOW 0.075
#define ERROR_PERCENT 0.12
#define EIGHTH_ERROR_PERCENT 0.1

class SongDecomposition
{
public:
	double bpm; //beats per minute
	double spb; //sec per beat
	double quarterWindow;
	double eighthWindow;
	std::string trackName;
	float trackStartTime;

	SongDecomposition(float beats, std::string name, float start) {
		bpm = beats;
		spb = 60 / bpm;
		quarterWindow = spb * ERROR_PERCENT;
		eighthWindow = spb * EIGHTH_ERROR_PERCENT;
		trackName = name;
		trackStartTime = start;
	}

	int isOnBeat(float clickTime);
	float getBeatStart(float time);
	float timeToClosestBeat(float time);
	float timeToBeat(float time, int beat);
	~SongDecomposition();
};

