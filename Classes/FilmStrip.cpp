//
//  FilmStrip.cpp
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#include "FilmStrip.h"

/**
 * Creates a new filmstrip from the given texture.
 *
 * @param texture The texture image to use
 * @param rows The number of rows in the filmstrip
 * @param cols The number of columns in the filmstrip
 *
 * @return The allocated filmstrip as an autorelease object
 */
FilmStrip* FilmStrip::create(Texture2D* texture, int rows, int cols) {
    FilmStrip *filmStrip = new (std::nothrow) FilmStrip();
    if (filmStrip && filmStrip->init(texture, rows, cols, rows*cols)) {
        filmStrip->autorelease();
        return filmStrip;
    }
    CC_SAFE_DELETE(filmStrip);
    return nullptr;
}

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
FilmStrip* FilmStrip::create(Texture2D* texture, int rows, int cols, int size) {
    FilmStrip *filmStrip = new (std::nothrow) FilmStrip();
    if (filmStrip && filmStrip->init(texture, rows, cols, size)) {
        filmStrip->autorelease();
        return filmStrip;
    }
    CC_SAFE_DELETE(filmStrip);
    return nullptr;
}

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
bool FilmStrip::init(Texture2D* texture, int rows, int cols, int size) {
    if (size > rows*cols) {
        log("ERROR: Invalid strip size %d > %d", size, rows*cols);
        return false;
    }
    this->cols = cols;
    this->size = size;

    rect.size.setSize(texture->getPixelsWide()/cols, texture->getPixelsHigh()/rows);
    return this->initWithTexture(texture, rect);
}

/**
 * Constructs a new empty filmstrip
 *
 * You should never call this constructor.  Call create() instead.
 */
FilmStrip::FilmStrip() {
    cols = 0;
    size = 0;
    frame = 0;
    rect.setRect(0,0,0,0);
}

/**
 * Disposes of a filmstrip and its contents
 */
FilmStrip::~FilmStrip() {
    // Deallocation happens in parent.
}

/**
 * Sets the active frame as the given index.
 *
 * If the frame index is invalid, an error is raised.
 *
 * @param frame the index to make the active frame
 */
void FilmStrip::setFrame(int frame) {
    if (frame < 0 || frame >= size) {
        log("ERROR: Invalid animation frame %d", frame);
        return;
    }
    this->frame = frame;
    int x = (frame % cols)*rect.size.width;
    int y = (frame / cols)*rect.size.height;
    rect.origin.set(x,y);
    setTextureRect(rect);
}
