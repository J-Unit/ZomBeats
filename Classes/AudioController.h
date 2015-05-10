#include <string>

#define EFFECT_VOLUME 0.60f
#define MUSIC_VOLUME 0.70f

class SongDecomposition;

class AudioController
{
public:
	SongDecomposition *currentSong;
	int curSongId;


	AudioController();
	~AudioController();

	void playTrack(SongDecomposition *s, bool loop=true);
	void playEffect(std::string e, float vol);
	bool frameOnBeat;
	bool prevOnBeat;
	void setFrameOnBeat(float elpasedTime);
	float getBeatStart();
	int wasOnBeat(float timeAgo);
	float songTime;
	float lastReportedSongTime;
	float lastbeat = 0;
	float keepit = 0;
	float total_kept = 0;
	float total_beats = 0;
	float estimated_song_time = 0;
	float timeToClosestBeat();
	float timeToBeat(int beat);
	float lastClickTime;
	bool songIsOver();
	int trackLoops;
	float audioDelay;
	float videoDelay;
	bool paused;


	void stop();
};

