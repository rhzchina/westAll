#include "com_rhz_game_JniCall.h"
#include "WelcomeScene.h"
#include "GameData.h"
#include "StartScene.h"

#include "tool.h"

JNIEXPORT void JNICALL Java_com_rhz_game_JniCall_callCMethod
	(JNIEnv *env, jclass clazz, jint type){
	int result = type;
	if(type > 1){
		int kind = 0;
		int id = 0;
		switch(type){
		case 2:
			id = 4;
			break;
		case 3:
			id = 5;
			break;
		case 4:
			id = 6;
			break;
		case 5:
			kind = 3;
			id = 3;
			break;
		}
		GameData::addSate(kind,0,id);
		GameData::replaceSate(kind,id);
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WelcomeScene::refresh),WelcomeScene::instance,0,false,0,0);
	}else if(type == 1){  //���� ����
		GameData::pay();
		CCDirector::sharedDirector()->resume();
	}else if (type == -1){//ȡ��
		WelcomeScene::instance->reset();
	}else if (type == -2){
		//CCDirector::sharedDirector()->end();
		//GameData::clearData();
		//CCDirector::sharedDirector()->replaceScene(StartScene::scene());
		CCDirector::sharedDirector()->resume();
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameData::clearData),GameData::getInstance(),0,false,0,0);

	}
}