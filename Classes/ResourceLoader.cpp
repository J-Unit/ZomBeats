//
//  ResourceLoader.cpp
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#include "ResourceLoader.h"
#include "2d/CCFontAtlasCache.h"  // Not sure why not part of cocos2d.h

/** Initialization of static reference */
ResourceLoader* ResourceLoader::gLoader = NULL;

/**
 * Constructs a new resource loader.
 *
 * Because this is not asynchronous, this loads all assets on creation.
 */
ResourceLoader::ResourceLoader() {
    // Load the font.
    TTFConfig* font = new TTFConfig("../Resources/fonts/Marker Felt.ttf",24,GlyphCollection::DYNAMIC);
    if (!FontAtlasCache::getFontAtlasTTF(*font)) {
        // Failed to load font.
        delete font;
    } else {
        fontdata["MarkerFelt"] = font;
    }

    // Load the textures (Autorelease objects)
    Texture2D* text;
    text = Director::getInstance()->getTextureCache()->addImage("../Resources/textures/wood.png");
    if (text) {
        text->retain();
        textures["space"] = text;
    }
    text = Director::getInstance()->getTextureCache()->addImage("../Resources/textures/ships.png");
    if (text) {
        text->retain();
        textures["ship"] = text;
    }
    text = Director::getInstance()->getTextureCache()->addImage("../Resources/textures/zombie.png");
    if (text) {
        text->retain();
        textures["planet1"] = text;
    }
    text = Director::getInstance()->getTextureCache()->addImage("../Resources/textures/zombie.png");
    if (text) {
        text->retain();
        textures["planet2"] = text;
    }
}

/**
 * Deletes the resource loader and releases all assets.
 */
ResourceLoader::~ResourceLoader() {
    // Clear the allocated fonts
    for (auto it = fontdata.begin(); it != fontdata.end(); ++it ) {
        delete it->second;
        it->second = NULL;
    }
    fontdata.clear();
    FontAtlasCache::purgeCachedData();
    
    // Release the allocated textures
    for (auto it = textures.begin(); it != textures.end(); ++it ) {
        it->second->release();
    }
    textures.clear();
    Director::getInstance()->getTextureCache()->removeAllTextures();
}

/**
 * Static method to get the single resource loader.
 *
 * @return The resource loader
 */
ResourceLoader* ResourceLoader::getInstance() {
    return gLoader;
}

/**
 * Static method to construct resource loader and load content.
 */
void ResourceLoader::loadContent() {
    gLoader = new ResourceLoader();
}

/**
 * Static method to dispose of resource loader and unload content.
 */
void ResourceLoader::unloadContent() {
    if (gLoader != NULL) {
        delete gLoader;
        gLoader = NULL;
    }
}