#include "GrooveMeter.h"
#include <algorithm>

GrooveMeter::GrooveMeter()
{
	detectionRadius = INITIAL_DETECTION_RADIUS;
}

void GrooveMeter::increaseRadius(){
	detectionRadius = std::min(detectionRadius + DETECTION_RADIUS_INCREASE, MAX_DETECTION_RADIUS);
}

void GrooveMeter::decreaseRadius(){
	detectionRadius = std::max(detectionRadius - DETECTION_RADIUS_DECREASE, MIN_DETECTION_RADIUS);
}

void GrooveMeter::drain(){
	detectionRadius = std::min(detectionRadius + 0.06f, MAX_DETECTION_RADIUS);
}

std::string GrooveMeter::getGroovinessDisplay(float v){
	if (v < 0.2) return "Pitiful!";
	else if (v < 0.4) return "Awkward!";
	else if (v < 0.6) return "Average!";
	else if (v < 0.85) return "Groovy!";
	else return "Incredible!";
}

GrooveMeter::~GrooveMeter()
{
}
