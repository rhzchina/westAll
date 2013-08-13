LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/Map.cpp \
                   ../../Classes/Role.cpp \
                   ../../Classes/Weapon.cpp \
				   ../../Classes/WelcomeScene.cpp\
				   ../../Classes/StartScene.cpp \
				   ../../Classes/SelectScene.cpp \
				   ../../Classes/GameData.cpp \
				   ../../Classes/ShopItem.cpp \
				   ../../Classes/tool.cpp \
				   ../../Classes/Prop.cpp \
				   ../../Classes/com_rhz_game_JniCall.cpp
                   
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static 
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)