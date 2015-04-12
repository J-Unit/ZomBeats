#include "SongDecomposition.h"
#include <math.h>

bool SongDecomposition::isOnBeat(float clickTime){
	float offset_clicktime = clickTime + trackStartTime;
	float clicked = fmod(offset_clicktime, spb);
	return (clicked <= ERROR_WINDOW || clicked >= spb - ERROR_WINDOW);
}

float SongDecomposition::getBeatStart(float time){
	return floor((time + trackStartTime) / spb) * spb - ERROR_WINDOW;
}

float SongDecomposition::timeToClosestBeat(float time){
	float prev = time - getBeatStart(time) + 2 * ERROR_WINDOW;
	float next = getBeatStart(time + spb) - time;
	if (prev <= next) return prev;
	return -next;
}

SongDecomposition::~SongDecomposition()
{
}
