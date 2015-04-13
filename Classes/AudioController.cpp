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
	audioDelay = videoDelay = 0.0f;
}


AudioController::~AudioController()
{
}

void AudioController::playTrack(SongDecomposition *track, bool loop){
	AudioEngine::stopAll();
	currentSong = track;
	curSongId = AudioEngine::play2d(currentSong->trackName, loop, 1);
	songTime = lastReportedSongTime = 0.0f;
	trackLoops = 0;
}

void AudioController::playEffect(std::string effect){
	AudioEngine::play2d(effect, false, 1);
}

void AudioController::setFrameOnBeat(float deltaTime){
	//total_kept += elapsedTime - AudioEngine::getCurrentTime(curSongId);
	//total_beats += 1;
	//keepit = total_kept / total_beats;
	//estimated_song_time = elapsedTime - keepit;
	songTime += deltaTime;
	float curTime = AudioEngine::getCurrentTime(curSongId);
	if (curTime != lastReportedSongTime){
		if (curTime < lastReportedSongTime){
			songTime = curTime;
			trackLoops++;
		}
		else{
			songTime = (songTime + curTime) / 2;
		}
		lastReportedSongTime = curTime;
	}
	prevOnBeat = frameOnBeat;
	frameOnBeat = currentSong->isOnBeat(songTime - audioDelay - videoDelay);
}

bool AudioController::wasOnBeat(float timeAgo){
	lastClickTime = songTime - timeAgo;
	return currentSong->isOnBeat(lastClickTime - audioDelay);
}

float AudioController::getBeatStart(){
	return currentSong->getBeatStart(lastClickTime - audioDelay - videoDelay);
}

float AudioController::timeToClosestBeat(){
	return currentSong->timeToClosestBeat(lastClickTime - audioDelay);
}

float AudioController::timeToBeat(int i){
	return currentSong->timeToBeat(lastClickTime - audioDelay, i);
}

bool AudioController::songIsOver(){
	return trackLoops > 0;
}