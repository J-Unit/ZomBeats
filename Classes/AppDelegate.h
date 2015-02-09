//
//  AppDelegate.cpp
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
 * Class that represents cocos2d Application.
 *
 * This is implemented as private inheritance is to hide some interface calls by the Director.
 */
class  AppDelegate : private cocos2d::Application {

public:
    /**
     * Constructs a new AppDelegate (defers to the parent class)
     */
    AppDelegate();
    
    /**
     * Disposes a new AppDelegate (an extra memory goes here)
     */
    virtual ~AppDelegate();

    /**
     * Initialize the OpenGL settings. If you want a different context,
     * just modify the value of glContextAttrs. It will takes effect
     * on all platforms
     */
    virtual void initGLContextAttrs();

    /**
     * Invoked when the application has finished bootstrap loading.
     *
     * Your application startup code goes here.  However, you should
     * not need to make too many modifications to this method.  Just
     * create a new game layer.
     *
     * @return true if initialization succesful; otherwise false
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * Function called when the App in inactive.
     *
     * This happens when you return to the home screen or take a phone call.
     * You should add page-out code to your game root and invoke it here.
     *
     * If your app uses A LOT of memory, you might want to release all that
     * memory here.  We will have examples of this in later demos.
     */
    virtual void applicationDidEnterBackground();

    /**
     * Function called when the App is active again.
     *
     * This is the inverse of the function applicationDidEnterBackground().
     * You should add page-in code to your game root and invoke it here.
     *
     * If you released memory, you will need to restore all that state here.
     * We will have examples of this in later demos.
     */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_

