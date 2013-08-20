#include "LogoScene.h"
#include "tool.h"
#include "StartScene.h"



LogoScene::LogoScene(void)
{
}


LogoScene::~LogoScene(void)
{
}

bool LogoScene::init(){
	bool success = false;
	do{
		CCLayerColor* layer = CCLayerColor::create(ccc4(255,255,255,255));
		CC_BREAK_IF(!layer);
		addChild(layer);
		
		CCSprite* bg = CCSprite::create("sp_logo.png");
		CC_BREAK_IF(!bg);

		SETANCHPOS(bg,425,240,0.5,0.5);
		addChild(bg);

		scheduleOnce(schedule_selector(LogoScene::next),2);
		success = true;
	}while(false);
	return success; 
}

void LogoScene::ccTouchesBegan(CCSet* set, CCEvent* event){

}

void LogoScene::next(float dt){
	CCScene* temp = CCTransitionProgressRadialCW::transitionWithDuration(1.2, StartScene::scene());
	CCDirector::sharedDirector()->replaceScene(temp);
}

CCScene* LogoScene::scene(){
	CCScene* scene = NULL;

	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		LogoScene* layer = LogoScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(false);

	return scene;
}
