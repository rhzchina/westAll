#include "com_rhz_game_JniCall.h"
#include "WelcomeScene.h"
#include "GameData.h"
#include "StartScene.h"

#include "tool.h"

JNIEXPORT void JNICALL Java_com_rhz_game_JniCall_callCMethod
	(JNIEnv *env, jclass clazz, jint type){
	int result = type;
	if(type > 1){
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WelcomeScene::refresh),WelcomeScene::instance,0,false,0,0);
	}else if(type == 1){  //正版 激活
		GameData::pay();
		CCDirector::sharedDirector()->resume();
	}else if (type == -1){//取消
		WelcomeScene::instance->reset();
	}else if (type == -2){
		//CCDirector::sharedDirector()->end();
		//GameData::clearData();
		CCDirector::sharedDirector()->resume();
		//CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameData::clearData),GameData::getInstance(),0,false,0,0);
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameData::clearData),GameData::getInstance(),0,false,0,0);

	}
}