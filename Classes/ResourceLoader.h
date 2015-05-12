//
//  ResourceLoader.h
//  ShipDemo
//
//  Created by Walker White on 1/18/15.
//
#ifndef __TD_RESOURCELOADER_H__
#define __TD_RESOURCELOADER_H__

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

/**
 * Singleton class to support resource loading.
 *
 * Resources should always be managed by a central loader.  Cocos2D appears to have these
 * things all over the place. This will particularly be a problem when we want to support
 * asynchronous asset loading (later in the semester).  Right now, this is synchronous
 * loader, but it is designed in such a way that we could change it later.
 *
 * As this course is a singleton, you should not create new instances of this object
 * (and the constructor is protected for that very reason).  Instead, you should always
 * access it through the static methods.
 */
class ResourceLoader {
private:
    /** Hash map storing the textures */
    unordered_map<string, Texture2D*> textures;
    /** Hash map storing the fonts */
    unordered_map<string, TTFConfig*> fontdata;
	/** The number of textures that we expect to load eventually */
	int texturesExpected;
	/** The number of textures that we have loaded so far */
	int texturesLoaded;

	void textureCB(Texture2D* texture, string key);

protected:
    /** Reference to the singleton object */
    static ResourceLoader* gLoader;
    
    /**
     * Constructs a new resource loader.
     *
     * Because this is not asynchronous, this loads all assets on creation.
     */
    ResourceLoader();

    /**
     * Deletes the resource loader and releases all assets.
     */
    ~ResourceLoader();
    
public:
    /**
     * Static method to get the single resource loader.
     *
     * @return The resource loader
     */
    static ResourceLoader* getInstance();
    
    /**
     * Static method to construct resource loader and load content.
     */
    static void loadContent();
        
    /**
     * Static method to dispose of resource loader and unload content.
     */
    static void unloadContent();
    
    /**
     * Returns true if the key maps to a loaded texture.
     *
     * This method is useful in case a texture fails to load.
     *
     * @return True if the key maps to a texture
     */
    bool hasTexture(string key)         { return textures.find(key) != textures.end(); }

    /**
     * Returns the texture for the given key.
     *
     * If the key is valid, the texture is guaranteed not to be null.
     *
     * @return the texture for the given key
     */
    Texture2D* getTexture(string key)   { return textures[key]; }

    /**
     * Returns true if the key maps to a loaded font.
     *
     * This method is useful in case a font fails to load.
     *
     * @return True if the key maps to a font
     */
    bool hasFont(string key)            { return fontdata.find(key) != fontdata.end(); }

    /**
     * Returns the font information for the given key.
     *
     * If the key is valid, the font is guaranteed not to be null.
     *
     * @return the font information for the given key
     */
    TTFConfig* getFont(string key)      { return fontdata[key]; }

	/**
	* Adds a new texture file to the loading queue
	*
	* The file will be loaded asynchronously.  When it is finished loading, it will be
	* added to this asset manager, and accessible under the given key.
	*
	* This method will mark the loading process as not complete, even if it was
	* completed previously.
	*
	* @param filename The pathname to a texture file
	* @param key      The key to access the texture after loading
	*/
	void queueTexture(const std::string& filename, const std::string& key);

	/**
	* Returns true if the assert manager has finished loading
	*
	* @return true if the assert manager has finished loading
	*/
	bool isComplete() const { return texturesExpected == texturesLoaded; }

	float getPercentage() { return float(texturesLoaded) / texturesExpected; }
};


#endif /* defined(__TD_RESOURCELOADER_H__) */
