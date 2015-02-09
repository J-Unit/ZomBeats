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
    inputThrust.set(0.0f,0.0f);
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
    
    
    // Create the touch listener. This is an autorelease object.
    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = [=](Touch* t, Event* event) {
                                        return this->touchBeganCB(t,event);
                                    };
    touchListener->onTouchEnded = [=](Touch* t, Event* event) {
                                        this->touchEndedCB(t,event);
                                    };
    
    // Create the accelerometer listener. This is an autorelease object.
    Device::setAccelerometerEnabled(true);  // Comment out to remove accelerometer.
    accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(InputController::accleromCB, this));
    
    // Create the keyboard listener. This is an autorelease object.
    keybdListener = EventListenerKeyboard::create();
    keybdListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
                                        this->keyPressedCB(keyCode,event);
                                    };
    keybdListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
                                        this->keyReleasedCB(keyCode,event);
                                    };
    
    // Register the listeners
    dispatcher->addEventListenerWithFixedPriority(keybdListener,1);
    dispatcher->addEventListenerWithFixedPriority(touchListener,2);
    dispatcher->addEventListenerWithFixedPriority(accelListener,3);
}

/**
 * Disposes of this input controller, releasing all listeners.
 */
InputController::~InputController() {
    dispatcher->removeEventListener(touchListener);
    dispatcher->removeEventListener(accelListener);
    dispatcher->removeEventListener(keybdListener);
    touchListener = NULL; // Reference counting handles deletion.
    accelListener = NULL;
    keybdListener = NULL;
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
    if (active && keyCount > 0) {
        // Forces increase the longer you hold a key.
        forceLeft  += (keyLeft  ? KEYBOARD_FORCE_INCREMENT : 0.0f);
        forceRight += (keyRight ? KEYBOARD_FORCE_INCREMENT : 0.0f);
        forceDown  += (keyDown  ? KEYBOARD_FORCE_INCREMENT : 0.0f);
        forceUp    += (keyUp    ? KEYBOARD_FORCE_INCREMENT : 0.0f);
        
        // Clamp everything so it does not fly off to infinity.
        forceLeft  = (forceLeft  > INPUT_MAXIMUM_FORCE ? INPUT_MAXIMUM_FORCE : forceLeft);
        forceRight = (forceRight > INPUT_MAXIMUM_FORCE ? INPUT_MAXIMUM_FORCE : forceRight);
        forceDown  = (forceDown  > INPUT_MAXIMUM_FORCE ? INPUT_MAXIMUM_FORCE : forceDown);
        forceUp    = (forceUp    > INPUT_MAXIMUM_FORCE ? INPUT_MAXIMUM_FORCE : forceUp);
    
        // Update the keyboard thrust.  Result is cumulative.
        keybdThrust.x += forceRight;
        keybdThrust.x -= forceLeft;
        keybdThrust.y += forceUp;
        keybdThrust.y -= forceDown;
        keybdThrust.x = RANGE_CLAMP(keybdThrust.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
        keybdThrust.y = RANGE_CLAMP(keybdThrust.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
        
        // Transfer to main thrust. This keeps us from "adding" to accelerometer or touch.
        inputThrust.x = keybdThrust.x/X_ADJUST_FACTOR;
        inputThrust.y = keybdThrust.y/Y_ADJUST_FACTOR;
    }
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
    return true;
}

/**
 * Callback for the beginning of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::touchEndedCB(Touch* t, Event* event) {
    Vec2 finishTouch = t->getLocation()-startTouch;
    finishTouch.x = RANGE_CLAMP(finishTouch.x, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
    finishTouch.y = RANGE_CLAMP(finishTouch.y, -INPUT_MAXIMUM_FORCE, INPUT_MAXIMUM_FORCE);
    
    // Go ahead and apply to thrust now.
    inputThrust.x = finishTouch.x/X_ADJUST_FACTOR;
    inputThrust.y = finishTouch.y/Y_ADJUST_FACTOR;
}

/**
 * Callback for a change in accelerometer state
 *
 * @param acc   The accelerometer information
 * @param event The associated event
 */
void InputController::accleromCB(Acceleration *acc, Event *event) {
    // Apply to thrust directly.
    inputThrust.x =  acc->x*ACCELEROM_FACTOR;
    inputThrust.y = -acc->y*ACCELEROM_FACTOR;
}

/**
 * Callback for a key press.
 *
 * @param keyCode   The key pressed
 * @param event     The associated event
 */
void InputController::keyPressedCB(EventKeyboard::KeyCode keyCode, Event* event) {
    switch(keyCode) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            keyCount++;
            keyLeft = true;
            forceLeft = KEYBOARD_INITIAL_FORCE;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            keyCount++;
            keyRight = true;
            forceRight = KEYBOARD_INITIAL_FORCE;
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            keyCount++;
            keyUp = true;
            forceUp = KEYBOARD_INITIAL_FORCE;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            keyCount++;
            keyDown = true;
            forceDown = KEYBOARD_INITIAL_FORCE;
            break;
        default:
            break;
    }
}

/**
 * Callback for a key release.
 *
 * @param keyCode   The key released
 * @param event     The associated event
 */
void InputController::keyReleasedCB(EventKeyboard::KeyCode keyCode, Event* event) {
    switch(keyCode) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            keyCount--;
            keyLeft = false;
			keybdThrust.x = 0.0f;
            forceLeft = 0.0f;
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            keyCount--;
            keyRight = false;
			keybdThrust.x = 0.0f;
            forceRight = 0.0f;
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            keyCount--;
            keyUp = false;
			keybdThrust.y = 0.0f;
            forceUp = 0.0f;
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            keyCount--;
            keyDown = false;
			keybdThrust.y = 0.0f;
            forceDown = 0.0f;
            break;
        default:
            break;
    }
	inputThrust.x = keybdThrust.x / X_ADJUST_FACTOR;
	inputThrust.y = keybdThrust.y / Y_ADJUST_FACTOR;
}
