//
//  InputController.cpp
//  ShipDemo
//
//  Created by Walker White on 7/25/14.
//

// WARNING: I had serious problems with the accelerometer on my iPad when I first
// ran this code.  This is because Cocos2D buffers all the accelerometer calls
// and so it begins to lag behind the true accelerometer.  Why the Cocos2D
// designers did not provide a polling API for the accelerometer is a mystery.
// To fix the problem, you must modify CCDevice-ios.mm, as described here:
// http://discuss.cocos2d-x.org/t/accelerometer-data-lagging/16105/2

#include "InputController.h"
#include "SongDecomposition.h"

// Macros to eliminate magic numbers
#define INPUT_MAXIMUM_FORCE     1000.0f  // Historical choice from Marmalade
#define X_ADJUST_FACTOR          500.0f
#define Y_ADJUST_FACTOR          -50.0f
#define ACCELEROM_FACTOR         200.0f

#define KEYBOARD_INITIAL_FORCE    10.0f
#define KEYBOARD_FORCE_INCREMENT  10.0f
#define KEYBOARD_ACCELERATION      1.2f
#define KEYBOARD_DAMPEN_AMOUNT     0.75f

#define RANGE_CLAMP(x,y,z)  (x < y ? y : (x > z ? z : x))

/**
 * Creates a new input controller for the given dispatcher.
 *
 * @param dispatcher Event handler for callbacks.
 */
InputController::InputController(cocos2d::EventDispatcher* dispatcher) {
    this->dispatcher = dispatcher;
	clickProcessed = true;
    //inputThrust.set(0.0f,0.0f);
    //keybdThrust.set(0.0f,0.0f);
    inputThrust.set(0.0f,0.0f);
	inputPos.set(0.0f, 0.0f);
    keybdThrust.set(0.0f,0.0f);
    active = false;
    
    // Set up keyboard state
    keyCount = 0;
    forceLeft  = 0.0f;
    forceRight = 0.0f;
    forceUp    = 0.0f;
    forceDown  = 0.0f;
    
    keyLeft  = false;
    keyRight = false;
    keyUp    = false;
    keyDown  = false;

	clicked = false;
    
    
    // Create the touch listener. This is an autorelease object.
    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = [=](Touch* t, Event* event) {
                                        return this->touchBeganCB(t,event);
                                    };
	touchListener->onTouchMoved = [=](Touch* t, Event* event) {
										this->touchContinueCB(t, event);
									};
    touchListener->onTouchEnded = [=](Touch* t, Event* event) {
                                        this->touchEndedCB(t,event);
                                    };
    
    // Create the accelerometer listener. This is an autorelease object.
    Device::setAccelerometerEnabled(true);  // Comment out to remove accelerometer.
    accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(InputController::accleromCB, this));
	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = [=](Event* event){
		this->mousePressedCB(event);
	};
	mouseListener->onMouseMove = [=](Event* event){
		this->mouseContinuedCB(event);
	};
	mouseListener->onMouseUp = [=](Event* event){
		this->mouseReleasedCB(event);
	};


    // Register the listeners
	dispatcher->addEventListenerWithFixedPriority(mouseListener, 2);
    //dispatcher->addEventListenerWithFixedPriority(keybdListener,1);
    dispatcher->addEventListenerWithFixedPriority(touchListener, 1);
    dispatcher->addEventListenerWithFixedPriority(accelListener,3);
}

/**
 * Disposes of this input controller, releasing all listeners.
 */
InputController::~InputController() {
    dispatcher->removeEventListener(touchListener);
    //dispatcher->removeEventListener(accelListener);
    //dispatcher->removeEventListener(keybdListener);
	dispatcher->removeEventListener(mouseListener);
    touchListener = NULL; // Reference counting handles deletion.
    accelListener = NULL;
    //keybdListener = NULL;
	mouseListener = NULL;
    dispatcher = NULL;
}

/**
 * Start the input processing for this input controller.
 *
 * This will enable the dispatcher if it is not already enabled.
 */
void InputController::startInput() {
    active = true;
    if (!dispatcher->isEnabled()) {
        dispatcher->setEnabled(true);
    }
}

/**
 * Stop the input processing for this input controller.
 *
 * This will disable the dispatcher if it is not already disabled.
 */
void InputController::stopInput() {
    active = false;
    if (dispatcher->isEnabled()) {
        dispatcher->setEnabled(false);
    }
}

/**
 * Processes the currently cached inputs.
 *
 * This method is used to to poll the current input state.  For input
 * without an associated event (like holding a key down) this method is
 * a necessity.
 */
void InputController::update() {
    // Right now, we only use this to process the keyboard.
}

/**
 * Callback for the beginning of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 *
 * @return True if the touch was processed; false otherwise.
 */
bool InputController::touchBeganCB(Touch* t, Event* event) {
	startTouch = t->getLocation();
	Vec2 touch = t->getLocationInView();
	lastClick.set(touch.x, touch.y);
	clickProcessed = false;
	clickTime = time(0);
	clicked = true;
	startTouch.x = RANGE_CLAMP(startTouch.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
	startTouch.y = RANGE_CLAMP(startTouch.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
	return true;
}

bool InputController::touchContinueCB(Touch* t, Event* event) {
	startTouch = t->getLocation();
	Vec2 touch = t->getLocationInView();
	lastClick.set(touch.x, touch.y);
	startTouch.x = RANGE_CLAMP(startTouch.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
	startTouch.y = RANGE_CLAMP(startTouch.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
	return true;
}


/**
* Callback for the beginning of a touch event
*
* @param t     The touch information
* @param event The associated event
*/

void InputController::touchEndedCB(Touch* t, Event* event) {
	clicked = false;
	inputThrust.x = 0.0f / X_ADJUST_FACTOR;
	inputThrust.y = 0.0f / Y_ADJUST_FACTOR;
}

/**
 * Callback for a change in accelerometer state
 *
 * @param acc   The accelerometer information
 * @param event The associated event
 */
void InputController::accleromCB(Acceleration *acc, Event *event) {
    // Apply to thrust directly.
   // inputThrust.x =  acc->x*ACCELEROM_FACTOR;
   // inputThrust.y = -acc->y*ACCELEROM_FACTOR;
}


void InputController::mousePressedCB(Event* event) {
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	Vec2 mouseTouch = mouseEvent->getLocationInView();
	//mouseTouch.x = RANGE_CLAMP(mouseTouch.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
	//mouseTouch.y = RANGE_CLAMP(mouseTouch.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);

	lastClick.set(mouseTouch.x, -mouseTouch.y);
	clickProcessed = false;
	clickTime = time(0);
	// Go ahead and apply to thrust now.
	//inputThrust.x = mouseTouch.x / X_ADJUST_FACTOR;
	//inputThrust.y = mouseTouch.y / Y_ADJUST_FACTOR;
	clicked = true;
	inputPos = mouseEvent->getLocation();
	inputPos.x = RANGE_CLAMP(inputPos.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
	inputPos.y = RANGE_CLAMP(inputPos.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);

	// Go ahead and apply to thrust now.
	inputThrust.x = inputPos.x / X_ADJUST_FACTOR;
	inputThrust.y = inputPos.y / Y_ADJUST_FACTOR;

}

void InputController::mouseContinuedCB(Event* event) {
	if (clicked){
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
		Vec2 mouseTouch = mouseEvent->getLocationInView();
		//mouseTouch.x = RANGE_CLAMP(mouseTouch.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
		//mouseTouch.y = RANGE_CLAMP(mouseTouch.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);

		lastClick.set(mouseTouch.x, -mouseTouch.y);
		inputPos = mouseEvent->getLocation();
		inputPos.x = RANGE_CLAMP(inputPos.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
		inputPos.y = RANGE_CLAMP(inputPos.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);

		// Go ahead and apply to thrust now.
		inputThrust.x = inputPos.x / X_ADJUST_FACTOR;
		inputThrust.y = inputPos.y / Y_ADJUST_FACTOR;
	}

}

void InputController::mouseReleasedCB(Event* event) {
	//inputThrust.x = 0.0f / X_ADJUST_FACTOR;
	//inputThrust.y = 0.0f / Y_ADJUST_FACTOR;
	clicked = false;
	inputThrust.x = 0.0f / X_ADJUST_FACTOR;
	inputThrust.y = 0.0f / Y_ADJUST_FACTOR;
}