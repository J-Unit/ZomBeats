#include "CalibrationController.h"
#include "cocos2d.h"

CalibrationController::CalibrationController()
{

}


CalibrationController::~CalibrationController()
{
}

void CalibrationController::init(){
	clicks = 0;
	totalOffset = 0.0f;
	audioCalibration = true;
	acceptClicks = true;
}

void CalibrationController::update(float deltaTime){

}


float CalibrationController::audioDelay(){
	return totalOffset / clicks;
}

float CalibrationController::videoDelay(){
	return totalOffset / clicks;
}