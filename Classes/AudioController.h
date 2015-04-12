
#include <string>
class SongDecomposition;

class AudioController
{
public:
	SongDecomposition *currentSong;
	int curSongId;


	AudioController();
	~AudioController();

	void playTrack(SongDecomposition *s);
	void playEffect(std::string e);
	bool frameOnBeat;
	bool prevOnBeat;
	void setFrameOnBeat(float elpasedTime);
	float getBeatStart();

	float lastbeat = 0;
	float keepit = 0;
	float total_kept = 0;
	float total_beats = 0;
	float estimated_song_time = 0;
	float timeToClosestBeat();
};

