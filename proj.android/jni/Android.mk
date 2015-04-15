LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/FilmStrip.cpp \
                   ../../Classes/GameController.cpp \
                   ../../Classes/Zombie.cpp \
                   ../../Classes/ResourceLoader.cpp \
                   ../../Classes/Wall.cpp \
                   ../../Classes/LevelMap.cpp \
                   ../../Classes/Sword.cpp \
                   ../../Classes/GameState.cpp \
                   ../../Classes/View.cpp \
                   ../../Classes/AIController.cpp \
                   ../../Classes/Ship.cpp \
                   ../../Classes/SongDecomposition.cpp \
                   ../../Classes/InputController.cpp \
                   ../../Classes/Weapon.cpp \
                   ../../Classes/EnvironmentWeapon.cpp \
                   ../../Classes/Lawnmower.cpp \
                   ../../Classes/LevelSerializer.cpp \
                   ../../Classes/GrooveMeter.cpp \
                   ../../Classes/AudioController.cpp \
                   ../../Classes/Shotgun.cpp \
                   ../../Classes/CalibrationController.cpp \
                   ../../Classes/MainMenuScene.cpp \
                   ../../Classes/LevelSelectScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
