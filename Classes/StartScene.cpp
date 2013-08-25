#include "StartScene.h"
#include "GameScene.h"
#include "tool.h"
#include "GameData.h"
#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"

StartScene::StartScene(void)
{
}


StartScene::~StartScene(void)
{
}

bool StartScene::init(){
	bool success = false;
	do{
		CCSprite* bg = CCSprite::create("startBg.jpg");
		CC_BREAK_IF(!bg);

		SETANCHPOS(bg,0,0,0,0);
		addChild(bg);


		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("start.plist","start.png");
		//游戏菜单
		CCMenu* menu = CCMenu::create();
		CC_BREAK_IF(!menu);
		SETANCHPOS(menu,650,130,0,0);
		addChild(menu);

		CCMenuItemSprite* start = CCMenuItemSprite::create(
			CCSprite::createWithSpriteFrameName("start.png"),
			CCSprite::createWithSpriteFrameName("start.png"),
			this,menu_selector(StartScene::btnCallback));	
		CC_BREAK_IF(!start);
		start->setTag(1);
		menu->addChild(start);

		CCMenuItemSprite* shop  = CCMenuItemSprite::create(
			CCSprite::createWithSpriteFrameName("shop.png"),
			CCSprite::createWithSpriteFrameName("shop.png"),
			this,menu_selector(StartScene::btnCallback));
		CC_BREAK_IF(!shop);
		shop->setTag(2);
		menu->addChild(shop);

		CCMenuItemSprite* more = CCMenuItemSprite::create(
			CCSprite::createWithSpriteFrameName("more.png"),
			CCSprite::createWithSpriteFrameName("more.png"),
			this,menu_selector(StartScene::btnCallback));
		CC_BREAK_IF(!more);
		more->setTag(3);
		menu->addChild(more);

		menu->alignItemsVertically();

		//标题
		CCSprite* titleBg = CCSprite::createWithSpriteFrameName("name_bg.png");
		CC_BREAK_IF(!titleBg);
		SETANCHPOS(titleBg,500,250,0,0);
		addChild(titleBg);

		CCSprite* title = CCSprite::createWithSpriteFrameName("name.png");
		CC_BREAK_IF(!title);
		SETANCHPOS(title,500,250,0,0);
		addChild(title);

		//开关按钮
		CCMenu* sound = CCMenu::create();
		CC_BREAK_IF(!sound);
		SETANCHPOS(sound,50,50,0,0);
		addChild(sound);

		soundOn = CCMenuItemSprite::create(
			CCSprite::createWithSpriteFrameName("sound_off.png"),
			CCSprite::createWithSpriteFrameName("sound_off.png"),
			this,NULL);

		soundOff = CCMenuItemSprite::create(
			CCSprite::createWithSpriteFrameName("sound_on.png"),
			CCSprite::createWithSpriteFrameName("sound_on.png"),
			this,NULL);

		CCMenuItemToggle* toggle = CCMenuItemToggle::createWithTarget(this,menu_selector(StartScene::btnCallback),soundOff,soundOn,NULL);
		toggle->setTag(4);

		sound->addChild(toggle);

		//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("game.ogg"));
		//初始化用户数据
		if(GameData::initInstance(!GameData::payForGame)){
			if(checkPay(-1)){  //开启音乐
				GameData::music = true;
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game.ogg",true);
			}else{
				GameData::music = false;
				toggle->setSelectedIndex(1);
			}
		}else{
			if(!GameData::music){
				toggle->setSelectedIndex(1);
			}
		}

		success = true;
	}while(false);
	return success; 
}

CCScene* StartScene::scene(){
	CCScene* scene = NULL;

	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		StartScene* layer = StartScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(false);

	return scene;
}

void StartScene::btnCallback(CCObject* sender){
	switch(((CCNode*)sender)->getTag()){
		//startBtn
	case 1:
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		CCDirector::sharedDirector()->replaceScene(SelectScene::scene());
		break;
		//shopBtn
	case 2:
		CCDirector::sharedDirector()->replaceScene(WelcomeScene::scene());
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		break;
	case 3:
		callCharge(-1);
		break;
	case 4:
		if(((CCMenuItemToggle*)sender)->selectedItem() == soundOn){
			GameData::music = false;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}else{
			GameData::music = true;

			if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
				CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
			}else{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game.ogg",true);
			}
		}
		break;
	}
}