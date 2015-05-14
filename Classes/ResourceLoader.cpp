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
	texturesLoaded = texturesExpected = 0;

    // Load the font.
    TTFConfig* font = new TTFConfig("fonts/Marker Felt.ttf",24,GlyphCollection::DYNAMIC);
    if (!FontAtlasCache::getFontAtlasTTF(*font)) {
        // Failed to load font.
        delete font;
    } else {
        fontdata["MarkerFelt"] = font;
    }
	
	font = new TTFConfig("fonts/NewFont.ttf", 24, GlyphCollection::DYNAMIC);
	if (!FontAtlasCache::getFontAtlasTTF(*font)) {
		// Failed to load font.
		delete font;
	}
	else {
		fontdata["NewFont"] = font;
	}

	queueTexture("textures/tile_new.png", "tile1");
	queueTexture("textures/blood1.png", "blood");
	queueTexture("textures/zombie_new.png", "zombie_single");
	queueTexture("textures/dust.png", "dust");
	queueTexture("textures/grass1.png", "grass1");
	queueTexture("textures/grass2.png", "grass2");
	queueTexture("textures/grass3.png", "grass3");
	queueTexture("textures/gun2.png", "shrapnel");
	queueTexture("textures/glass.png", "glass");
	queueTexture("textures/triangle1.png", "triangle1");
	queueTexture("textures/triangle2.png", "triangle2");
	queueTexture("textures/spritebasic_withsword.png", "ricky");
	queueTexture("textures/ricky_lawnmower.png", "ricky_mower");
	queueTexture("textures/low_res_sprite_sheet_zombie.png", "zombie");
	queueTexture("textures/spritebasic.png", "ricky_gun");
	queueTexture("textures/wall.png", "pyramid");
	queueTexture("textures/fence.png", "fence");
	queueTexture("textures/house.png", "house");
	queueTexture("textures/rock.png", "rock");
	queueTexture("textures/trash.png", "trash");
	queueTexture("textures/trashcan.png", "trashcan");
	queueTexture("textures/tree.png", "tree");
	queueTexture("textures/sword.png", "sword");
	queueTexture("textures/pistol.png", "pistol");
	queueTexture("textures/shotgun.png", "shotgun");
	queueTexture("textures/fog_new.png", "fog");
	queueTexture("textures/mower.png", "mower");
	queueTexture("textures/pause_button.png", "pause_button");
	queueTexture("textures/pause_button_clicked.png", "pause_button_clicked");
	queueTexture("textures/restart_button.png", "restart_button");
	queueTexture("textures/restart_button_clicked.png", "restart_button_clicked");
	queueTexture("textures/resume_button.png", "resume_button");
	queueTexture("textures/resume_button_clicked.png", "resume_button_clicked");
	queueTexture("textures/menu_background.png", "blank_pause_menu");
	queueTexture("textures/musicNote.png", "music_note");
	queueTexture("textures/dialogue_box.png", "dialogue_popup");
	queueTexture("textures/ok_button.png", "ok_button");
	queueTexture("textures/countdown.png", "count_down");
	queueTexture("textures/record.png", "record");
	queueTexture("textures/bullet.png", "bullet");
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

void ResourceLoader::queueTexture(const std::string& filename, const std::string& key) {
	TextureCache* cache = Director::getInstance()->getTextureCache();
	// C++11 closures are great
	cache->addImageAsync(filename, [=](Texture2D* texture) { this->textureCB(texture, key); });
	texturesExpected++;
}

void ResourceLoader::textureCB(Texture2D* texture, string key) {
	texture->retain();
	textures[key] = texture;
	texturesLoaded++;
}
