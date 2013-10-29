#include "StartScene.h"
#include "GameScene.h"
#include "tool.h"
#include "GameData.h"
#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"

StartScene::StartScene(void)
{
	tempLayer = NULL;
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

		setTouchEnabled(true);

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("start.plist","start.png");
		//游戏菜单
		CCMenu* menu = CCMenu::create();
		CC_BREAK_IF(!menu);
		SETANCHPOS(menu,580,140,0,0);
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
		menu->setContentSize(CCSizeMake(shop->getContentSize().width, 480));
		CCLog("menu size %f%f", menu->getContentSize().width, menu->getContentSize().height);


		CCMenu* spMenu = CCMenu::create();
		CC_BREAK_IF(!spMenu);
		SETANCHPOS(spMenu, 740, 140, 0, 0);
		addChild(spMenu);

		CCMenuItemSprite* help = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("help.png"),
			CCSprite::createWithSpriteFrameName("help.png"),this,menu_selector(StartScene::btnCallback));
		help->setTag(5);
		spMenu->addChild(help);


		CCMenuItemSprite* about = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("about.png"),
			CCSprite::createWithSpriteFrameName("about.png"),this,menu_selector(StartScene::btnCallback));
		about->setTag(6);
		spMenu->addChild(about);


		CCMenuItemSprite* exit = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("exit.png"),
			CCSprite::createWithSpriteFrameName("exit.png"),this,menu_selector(StartScene::btnCallback));
		exit->setTag(7);
		spMenu->addChild(exit);
		spMenu->alignItemsVertically();
		spMenu->setContentSize(CCSizeMake(exit->getContentSize().width, 480));

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
	if(tempLayer){
		removeChild(tempLayer, true);
		tempLayer = NULL;
		return;
	}
	CCLog("print%d",((CCNode*)sender)->getTag());
	switch(((CCNode*)sender)->getTag()){
		//startBtn
	case 1:
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		CCDirector::sharedDirector()->replaceScene(SelectScene::scene());
		break;
		//shopBtn
	case 2:
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		CCDirector::sharedDirector()->replaceScene(WelcomeScene::scene());
		break;
	case 3:
		callCharge(-2);
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
	case 5:   //游戏帮助
		createTip(5);
		break;
	case 6:  //游戏关于
		createTip(6);
		break;
	case 7:
		callCharge(-1);
		break;
	}
}

void StartScene::createTip(int type){
	if(tempLayer != NULL){
		removeChild(tempLayer, true);
	}
	tempLayer = CCLayer::create();


	CCSprite* tip = CCSprite::create(type == 5 ? "help.png" : "dianxin.png");
	SETANCHPOS(tip,425, 240, 0.5, 0.5);
	tempLayer->addChild(tip);

	addChild(tempLayer);
}

void StartScene::ccTouchesBegan(CCSet* touches, CCEvent* event){
	if(tempLayer){
		removeChild(tempLayer, true);
		tempLayer = NULL;
	}
}