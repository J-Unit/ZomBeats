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
    TTFConfig* font = new TTFConfig("fonts/Marker Felt.ttf",24,GlyphCollection::DYNAMIC);
    if (!FontAtlasCache::getFontAtlasTTF(*font)) {
        // Failed to load font.
        delete font;
    } else {
        fontdata["MarkerFelt"] = font;
    }

    // Load the textures (Autorelease objects)
    Texture2D* text;
    text = Director::getInstance()->getTextureCache()->addImage("textures/tile_new.png");
    if (text) {
        text->retain();
        textures["tile1"] = text;
    }

	text = Director::getInstance()->getTextureCache()->addImage("textures/zombie_new.png");
	if (text) {
		text->retain();
		textures["zombie_single"] = text;
	}

    //text = Director::getInstance()->getTextureCache()->addImage("textures/ships.png");
	//text = Director::getInstance()->getTextureCache()->addImage("textures/chuck.png");
	text = Director::getInstance()->getTextureCache()->addImage("textures/spritebasic_withsword.png");
    if (text) {
        text->retain();
        textures["ricky"] = text;
    }
	
	text = Director::getInstance()->getTextureCache()->addImage("textures/ricky_lawnmower.png.png");
	if (text) {
		text->retain();
		textures["ricky_mower"] = text;
	}
    text = Director::getInstance()->getTextureCache()->addImage("textures/low_res_sprite_sheet_zombie.png");
    if (text) {
        text->retain();
        textures["zombie"] = text;
    }
	text = text = Director::getInstance()->getTextureCache()->addImage("textures/spritebasic_withshotty.png");
	if (text) {
		text->retain();
		textures["ricky_gun"] = text;
	}

	//add the wall textures here
	text = Director::getInstance()->getTextureCache()->addImage("textures/wall.png");
	if (text) {
		text->retain();
		textures["pyramid"] = text;
	}
	text = Director::getInstance()->getTextureCache()->addImage("textures/fence.png");
	if (text) {
		text->retain();
		textures["fence"] = text;
	}
	text = Director::getInstance()->getTextureCache()->addImage("textures/house.png");
	if (text) {
		text->retain();
		textures["house"] = text;
	}
	text = Director::getInstance()->getTextureCache()->addImage("textures/rock.png");
	if (text) {
		text->retain();
		textures["rock"] = text;
	}
	text = Director::getInstance()->getTextureCache()->addImage("textures/tree.png");
	if (text) {
		text->retain();
		textures["tree"] = text;
	}
	//add the weapon textures here
	text = Director::getInstance()->getTextureCache()->addImage("textures/sword.png");
	if (text) {
		text->retain();
		textures["sword"] = text;
	}
	text = Director::getInstance()->getTextureCache()->addImage("textures/pistol.png");
	if (text) {
		text->retain();
		textures["pistol"] = text;
	}
	text = Director::getInstance()->getTextureCache()->addImage("textures/shotgun.png");
	if (text) {
		text->retain();
		textures["shotgun"] = text;
	}

	//add the fog of war
	text = Director::getInstance()->getTextureCache()->addImage("textures/fog_new.png");
	if (text) {
		text->retain();
		textures["fog"] = text;
	}

	//add the weapon textures here
	text = Director::getInstance()->getTextureCache()->addImage("textures/mower.png");
	if (text) {
		text->retain();
		textures["mower"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/pause_button.png");
	if (text) {
		text->retain();
		textures["pause_button"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/pause_button_clicked.png");
	if (text) {
		text->retain();
		textures["pause_button_clicked"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/restart_button.png");
	if (text) {
		text->retain();
		textures["restart_button"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/resume_button.png");
	if (text) {
		text->retain();
		textures["resume_button"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/restart_button_clicked.png");
	if (text) {
		text->retain();
		textures["restart_button_clicked"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/resume_button_clicked.png");
	if (text) {
		text->retain();
		textures["resume_button_clicked"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/menu_background.png");
	if (text) {
		text->retain();
		textures["blank_pause_menu"] = text;
	}

	text = Director::getInstance()->getTextureCache()->addImage("textures/musicNote.png");
	if (text) {
		text->retain();
		textures["music_note"] = text;
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
