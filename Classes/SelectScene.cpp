#include "SelectScene.h"
#include "StartScene.h"
#include "tool.h"

SelectScene::SelectScene(void)
{
	scroll = NULL;
	level = NULL;
	tipText = NULL;
	index = 0;
}


SelectScene::~SelectScene(void)
{
	if(level){
		level->release();
		level = NULL;
	}
}

bool SelectScene::init(){
	bool success = false;
	do{
		CCLayer::init();

		setTouchEnabled(true);

		CCSprite* bg = CCSprite::create("shopBg.jpg");
		CC_BREAK_IF(!bg);
		SETANCHPOS(bg,0,0,0,0);
		addChild(bg);

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("choose.plist","choose.png");

		CCLayer* container = CCLayer::create();
		CC_BREAK_IF(!container);
		SETANCHPOS(container,0,50,0,0);

		int x = 0;
		level = CCArray::createWithCapacity(MAXLEVEL);
		level->retain();
		for(int i = 1;i <= MAXLEVEL;i++){
			//选关背景
			CCSprite* itemBg = CCSprite::create("choose_bg.png");
			SETANCHPOS(itemBg,x + 930 / 2,240,0.5,0.5);
			container->addChild(itemBg);

			//选关名称背景
			CCSprite* nameBg = CCSprite::createWithSpriteFrameName("name_bg.png");
			SETANCHPOS(nameBg,x + 854 / 2 - itemBg->getContentSize().width / 2,240,0.5,0.5);
			container->addChild(nameBg);

			char name[20];
			//关卡名称
			sprintf(name,"level%d_name.png",i);
			CCSprite* levelName = CCSprite::createWithSpriteFrameName(name);
			SETANCHPOS(levelName,x + 854 / 2 - itemBg->getContentSize().width / 2,240,0.5,0.5);
			container->addChild(levelName);

			if(i <= GameData::getMaxLevel()){
				sprintf(name,"level%d.png",i);
			}else{
				sprintf(name,"level%d_lock.png",i);
			}
			CCSprite* item = CCSprite::create(name);
			SETANCHPOS(item,x + 930 / 2,255,0.5,0.5);
			CC_BREAK_IF(!item);
			container->addChild(item);
			level->addObject(item);
			x += 854;
		}
		container->setContentSize(CCSizeMake(854 * 4,480));

		scroll = CCScrollView::create();
		scroll->setDelegate(this);
		scroll->setBounceable(false);
		//scroll->setBounceable(false);

		CC_BREAK_IF(!scroll);
		SETANCHPOS(scroll,0,0,0,0);
		scroll->setViewSize(CCSizeMake(854,480));
		scroll->setContentSize(CCSizeMake(854 * 4,480));
		scroll->setContainer(container);
		scroll->setDirection(kCCScrollViewDirectionHorizontal);

		addChild(scroll);


		CCMenu* menu = CCMenu::create();
		SETANCHPOS(menu,0,0,0,0);
		addChild(menu);

		CCMenuItemImage* back = CCMenuItemImage::create("back.png","back.png",this,menu_selector(SelectScene::btnCallback));
		SETANCHPOS(back,740,380,0,0);
		back->setTag(1);
		menu->addChild(back);

		//描述背景
		CCSprite* desBg = CCSprite::create("dlg_long.png");
		SETANCHPOS(desBg,425,0,0.5,0);
		addChild(desBg);

		CCSprite* next = CCSprite::createWithSpriteFrameName("next.png");
		SETANCHPOS(next,854,300,1,0.5);
		addChild(next);

		CCSprite* prev = CCSprite::createWithSpriteFrameName("next.png");
		SETANCHPOS(prev,0,300,0,0.5);
		prev->setFlipX(true);
		addChild(prev);

		tipText = CCSprite::createWithSpriteFrameName("level1_des.png");
		tipText->setScale(1.3);
		SETANCHPOS(tipText,525,60,0.5,0.5);
		addChild(tipText);

		success = true;
	}while(0);
	return success;
}

CCScene* SelectScene::scene(){
	CCScene* scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		SelectScene* layer = SelectScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);
	}while(0);

	return scene;
}

void SelectScene::ccTouchesBegan(CCSet* touches,CCEvent* event){
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	int num = touchedLevel(location);
	if(num != -1){
		touched = num;
	}
	lastX = location.x;

}

void SelectScene::ccTouchesMoved(CCSet* touches,CCEvent* event){
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	if(abs(lastX - location.x) > 50){
		touched = -1;
	}
}

void SelectScene::ccTouchesEnded(CCSet* touches,CCEvent* event){
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	int num = touchedLevel(location);
	if (num != -1 && num == touched){
		if(num == 1 || GameData::isPay()){
			GameData::setLevel(num);
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
			CCDirector::sharedDirector()->replaceScene(GameScene::scene());
		}else{
			callCharge(1);
		}
	}else{
		touched = -1;
	}

	//有30象素的差认为是滑动
	if(abs(location.x - lastX) > 30){
		if(location.x > lastX){
			if(index > 0){
				index--;
			}
		}else{
			if(index < 3){
				index++;
			}
		}
	}
}

int SelectScene::touchedLevel(CCPoint pos){
	int num = -1;
	for(int i = 0;i < level->count();i++)	{
		CCSprite* t = (CCSprite*)level->objectAtIndex(i);
		if(CCRectMake(scroll->getContentOffset().x + t->getPositionX() - t->getContentSize().width / 2,t->getPositionY() - t->getContentSize().height / 2,
			t->getContentSize().width,t->getContentSize().height).containsPoint(pos)){
				if(i + 1 <= GameData::getMaxLevel()){
					num = i + 1;
				}else{
					int unlock = 0;
					switch(i + 1){
					case 2:
						unlock = 2700;
						break;
					case 3:
						unlock = 4000;
						break;
					case 4:
						unlock = 5500;
						break;
					}
					/*char str[100];
					sprintf(str,conv("解锁本关需要挑战%dM,加油哦！"),unlock);
					tipText->setString(str);*/
				}
		}
	}
	return num;

}

void SelectScene::btnCallback(CCObject* sender){
	switch(((CCNode*)sender)->getTag()){
	case 1:
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		CCDirector::sharedDirector()->replaceScene(StartScene::scene());
		break;
	case 2:
		break;
	}
}

void SelectScene::scrollViewDidScroll(CCScrollView* view){
	SETANCHPOS(scroll->getContainer(),view->getContentOffset().x,view->getContentOffset().y + 50, 0, 0);
	if(view->isTouchMoved()){
	}else{
		removeChild(tipText,true);
		char n[50];
		CCLog("%d是多少",index);
		sprintf(n, "level%d_des.png", (index + 1));
		CCLog(n);
		tipText = CCSprite::createWithSpriteFrameName(n);
		tipText->setScale(1.3f);
		SETANCHPOS(tipText,505,60,0.5,0.5);
		addChild(tipText);
		scroll->getContainer()->runAction(CCMoveTo::create(0.3,ccp(-index * 854, view->getContentOffset().y)));
	}
}

void SelectScene::scrollViewDidZoom(CCScrollView* view){

}