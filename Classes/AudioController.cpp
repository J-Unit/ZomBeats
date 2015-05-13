#include "AudioController.h"
#include "SongDecomposition.h"

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
	hisses.maxInstances = 3;
	hisses.name = "hisses";
	//AudioEngine::setMaxAudioInstance(4);
}


AudioController::~AudioController()
{
}

void AudioController::playTrack(SongDecomposition *track, bool loop){
	AudioEngine::stopAll();
	currentSong = track;
	curSongId = AudioEngine::play2d(currentSong->trackName, loop, MUSIC_VOLUME);
	songTime = lastReportedSongTime = 0.0f;
	trackLoops = 0;
}

void AudioController::playEffect(std::string effect, float vol){
	if (effect == "sound_effects/ZombieHiss.mp3"){
		AudioEngine::play2d(effect, false, vol, &hisses);

	}
	else{
		AudioEngine::play2d(effect, false, vol);
	}
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
	frameOnBeat = currentSong->isOnBeat(songTime - audioDelay - videoDelay) == 1;
}

int AudioController::wasOnBeat(float timeAgo){
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

void AudioController::stop(){
	trackLoops++;
	AudioEngine::stopAll();
}

void AudioController::pauseSounds(){
	AudioEngine::pauseAll();
	paused = true;
}

void AudioController::resumeSounds(){
	AudioEngine::resumeAll();
	paused = false;
}