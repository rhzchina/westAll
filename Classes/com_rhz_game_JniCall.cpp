#include "com_rhz_game_JniCall.h"
#include "WelcomeScene.h"
#include "tool.h"

JNIEXPORT void JNICALL Java_com_rhz_game_JniCall_callCMethod
	(JNIEnv *env, jclass clazz, jint type){
	int result = type;
	CCLog("------------------");
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WelcomeScene::refresh),WelcomeScene::instance,0,false,0,0);
	CCLog("------------------+++++++++++++++++");
}