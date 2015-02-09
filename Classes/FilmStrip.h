//
//  FilmStrip.h
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#ifndef __TD_FILMSTRIP_H__
#define __TD_FILMSTRIP_H__

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

/**
 * Class to support simple film strip animation
 *
 * Animation in Cocos2D is an excellent example of the poor engineering in
 * the engine.  Animation is always a fixed cycle that loops, and there are
 * several additional classes to set up this loop.  Sometimes you just want
 * set your own animation frame manually.  For that, bizarrely, we need
 * our own custom class.
 *
 * A filmstrip is just a sprite sheet.  As the class is a subclass of Sprite,
 * you just load it like you would load a normal sprite.  The only difference
 * is that you specify the rows and columns in the sprite sheet so that it
 * can break up the images for you.
 */
class FilmStrip : public Sprite {

private:
    /** The number of columns in this filmstrip */
    int cols;
    
    /** The number of frames in this filmstrip */
    int size;
    
    /** The active animation frame */
    int frame;

    /** The bounds (in the image file) of the active frame */
    Rect rect;

public:
    /**
     * Creates a new filmstrip from the given texture.
     *
     * @param texture The texture image to use
     * @param rows The number of rows in the filmstrip
     * @param cols The number of columns in the filmstrip
     *
     * @return The allocated filmstrip as an autorelease object
     */
    static FilmStrip* create(Texture2D* texture, int rows, int cols);
    
    /**
     * Creates a new filmstrip from the given texture.
     *
     * The parameter size is to indicate that there are unused frames in
     * the filmstrip.  The value size must be less than or equal to
     * rows*cols, or this constructor will raise an error.
     *
     * @param texture The texture image to use
     * @param rows The number of rows in the filmstrip
     * @param cols The number of columns in the filmstrip
     * @param size The number of frames in the filmstrip
     *
     * @return The allocated filmstrip as an autorelease object
     */
    static FilmStrip* create(Texture2D* texture, int rows, int cols, int size);
    
    /**
     * Returns the number of frames in this filmstrip.
     *
     * @return the number of frames in this filmstrip.
     */
    int getSize()   { return size; }
    
    /**
     * Returns the current active frame.
     *
     * @return the current active frame.
     */
    unsigned int getFrame()  { return frame; }
    
    /**
     * Sets the active frame as the given index.
     *
     * If the frame index is invalid, an error is raised.
     *
     * @param frame the index to make the active frame
     */
    void setFrame(int frame);

CC_CONSTRUCTOR_ACCESS:
    // WE FOLLOW THE TEMPLATE FOR SPRITE HERE
    /**
     * Constructs a new empty filmstrip 
     *
     * You should never call this constructor.  Call create() instead.
     */
    FilmStrip();
    
    /**
     * Disposes of a filmstrip and its contents
     */
    ~FilmStrip();

    /**
     * Initializes the film strip with the given texture.
     *
     * @param texture The texture image to use
     * @param rows The number of rows in the filmstrip
     * @param cols The number of columns in the filmstrip
     * @param size The number of frames in the filmstrip
     *
     * @return True if initialization was successful; false otherwise.
     */
    bool init(Texture2D* texture, int rows, int cols, int size);
};

#endif /* defined(__TD_FILMSTRIP_H__) */
