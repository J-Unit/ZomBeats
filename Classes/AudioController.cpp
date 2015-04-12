#include "AudioController.h"
#include "SongDecomposition.h"
#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d::experimental;

AudioController::AudioController()
{
	currentSong = NULL;
	curSongId = 0;
	total_kept = 0;
	total_beats = 0;
	keepit = 0;
	estimated_song_time = 0;
	prevOnBeat = frameOnBeat = false;
}


AudioController::~AudioController()
{
}

void AudioController::playTrack(SongDecomposition *track){
	AudioEngine::stopAll();
	currentSong = track;
	curSongId = AudioEngine::play2d(currentSong->trackName, true, 1);
}

void AudioController::playEffect(std::string effect){
	AudioEngine::play2d(effect, false, 1);
}

void AudioController::setFrameOnBeat(float elapsedTime){
	total_kept += elapsedTime - AudioEngine::getCurrentTime(curSongId);
	total_beats += 1;
	keepit = total_kept / total_beats;
	estimated_song_time = elapsedTime - keepit;
	prevOnBeat = frameOnBeat;
	frameOnBeat = currentSong->isOnBeat(estimated_song_time);
}

float AudioController::getBeatStart(){
	return currentSong->getBeatStart(estimated_song_time);
}

float AudioController::timeToClosestBeat(){
	return currentSong->timeToClosestBeat(estimated_song_time);
}