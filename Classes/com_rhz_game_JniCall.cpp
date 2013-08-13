#include "com_rhz_game_JniCall.h"
#include "WelcomeScene.h"
#include "tool.h"

JNIEXPORT void JNICALL Java_com_rhz_game_JniCall_callCMethod
	(JNIEnv *env, jclass clazz, jint type){
	int result = type;
	int tab = 0;
	if(type > 1 && type < 5){
		tab = WEAPON;
	}else if(type == 5){
		tab = CLOTHES;
	}
	switch(result){
	case 0:
		break;
	case 1:
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		CCLog("++++++++++++++++++++%d%d",tab,type);
		WelcomeScene::instance->createItems(tab,0);
		break;
	}
	CCLog("@@@@@@@@@@@@@@@@@@@@@@@@@@@here%d",result);
}