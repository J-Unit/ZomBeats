#include "SongDecomposition.h"
#include <math.h>

bool SongDecomposition::isOnBeat(float clickTime){
	float time_to_first_beat = trackStartTime;//.22;//.1564
	float mpb = 60 / bpm;
	float offset_clicktime = clickTime + time_to_first_beat;
	float clicked = fmod(offset_clicktime,mpb);
	if (clicked <= ERROR_WINDOW || clicked >= mpb - ERROR_WINDOW){
		return true;
	}
	else{
		return false;
	}
}

SongDecomposition::~SongDecomposition()
{
}
