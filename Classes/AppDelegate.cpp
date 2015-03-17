//
//  AppDelegate.cpp
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#include "AppDelegate.h"
#include "GameController.h"
#include "ResourceLoader.h"
#include "View.h"


USING_NS_CC;

/**
 * Constructs a new AppDelegate (defers to the parent class)
 */
AppDelegate::AppDelegate() {

}

/**
 * Disposes a new AppDelegate (an extra memory goes here)
 */
AppDelegate::~AppDelegate() {
}

/** 
 * Initialize the OpenGL settings. If you want a different context,
 * just modify the value of glContextAttrs. It will takes effect 
 * on all platforms
 */
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

/**
 * Invoked when the application has finished bootstrap loading.
 *
 * Your application startup code goes here.  However, you should
 * not need to make too many modifications to this method.  Just
 * create a new game layer.
 *
 * @return true if initialization succesful; otherwise false
 */
bool AppDelegate::applicationDidFinishLaunching() {
	FileUtils::getInstance()->addSearchPath("../../Resources/");
    // Initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("ZomBeats");
        director->setOpenGLView(glview);
    }
	//LevelMap m;
	//m.shortestPath(NULL, NULL);

    // Turn on display FPS
    //director->setDisplayStats(true);

    // Set FPS. The default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	GameController* gc = GameController::create();

    // Run the game until the app quits
    director->runWithScene(gc->view->scene);

    return true;
}

/**
 * Function called when the App in inactive.
 *
 * This happens when you return to the home screen or take a phone call.
 * You should add page-out code to your game root and invoke it here.
 *
 * If your app uses A LOT of memory, you might want to release all that
 * memory here.  We will have examples of this in later demos.
 */
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

/**
 * Function called when the App is active again.
 *
 * This is the inverse of the function applicationDidEnterBackground(). 
 * You should add page-in code to your game root and invoke it here.
 *
 * If you released memory, you will need to restore all that state here.
 * We will have examples of this in later demos.
 */
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}