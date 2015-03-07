#include "SongDecomposition.h"
#include <math.h>

bool SongDecomposition::isOnBeat(float clickTime){

	float mpb = 60 / bpm;
	float clicked = fmod(clickTime,mpb);
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
