#include "SongDecomposition.h"
#include <math.h>

bool SongDecomposition::isOnBeat(float clickTime){
	float time_to_first_beat = trackStartTime;//.22;//.1564
	float offset_clicktime = clickTime + time_to_first_beat;
	float clicked = fmod(offset_clicktime, spb);
	return (clicked <= ERROR_WINDOW || clicked >= spb - ERROR_WINDOW);
}

float SongDecomposition::getBeatStart(float time){
	return std::floor((time + trackStartTime) / spb) * spb - ERROR_WINDOW;
}

SongDecomposition::~SongDecomposition()
{
}
