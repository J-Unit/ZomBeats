#include "SongDecomposition.h"
#include <math.h>

int SongDecomposition::isOnBeat(float clickTime){
	float offset_clicktime = clickTime + trackStartTime;
	float clicked = fmod(offset_clicktime, spb);
	float clickedhalf = fmod(offset_clicktime, spb / 2.0);
	if (clicked <= quarterWindow || clicked >= (spb - quarterWindow)){
		return 1;
	}
	else if (clickedhalf <= eighthWindow || clickedhalf >= (spb / 2.0 - eighthWindow)){
		return 2;
	}
	else return 0;
}

float SongDecomposition::getBeatStart(float time){
	return floor((time + trackStartTime) / spb) * spb - quarterWindow;
}

float SongDecomposition::timeToClosestBeat(float time){
	float prev = (time+trackStartTime) - (getBeatStart(time) + quarterWindow);
	float next = (getBeatStart(time + spb) + quarterWindow) - (time+trackStartTime);
	/*if (prev <= ERROR_WINDOW || next <= ERROR_WINDOW){
		bool onBeat = isOnBeat(time+trackStartTime);
	}*/
	if (prev <= next) return prev;
	return -next;
}

float SongDecomposition::timeToBeat(float time, int beat){
	float beatTime = trackStartTime + spb * beat;
	return time - beatTime;
}

SongDecomposition::~SongDecomposition()
{
}
