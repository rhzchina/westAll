#include "WelcomeScene.h"
#include "tool.h"
#include "StartScene.h"


WelcomeScene::WelcomeScene(void)
{
	itemsLayer = NULL;
	itemsArray = NULL;
	temp = NULL;
}


WelcomeScene::~WelcomeScene(void)
{
}

bool WelcomeScene::init(){
	bool success = false;
	do{
		CCLayer::init()	;
		setTouchEnabled(true);

		CCSprite* bg = CCSprite::create("shopBg.jpg");
		bg->setAnchorPoint(ccp(0,0));
		addChild(bg);

		createItems(CLOTHES);

		CCMenu* menu = CCMenu::create();
		CC_BREAK_IF(!menu);
		SETANCHPOS(menu,480,240,0.5,0.5);
		addChild(menu);

		CCMenuItemSprite* close = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("close.png"),
			CCSprite::createWithSpriteFrameName("close.png"),this,menu_selector(WelcomeScene::btnCallback));
		SETANCHPOS(close,260,170,0,0);
		close->setTag(-2);
		menu->addChild(close);

		CCMenuItemSprite* buy = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("buy.png"),
			CCSprite::createWithSpriteFrameName("buy.png"),this,menu_selector(WelcomeScene::btnCallback));
		SETANCHPOS(buy,240,-220,0,0);
		buy->setTag(-1);
		menu->addChild(buy);

		CCMenuItemSprite* weapon = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("weapon.png"),
			CCSprite::createWithSpriteFrameName("weapon.png"),this,menu_selector(WelcomeScene::btnCallback));
		//SETANCHPOS(weapon,-450,120,0,0);
		SETANCHPOS(weapon,-450,110,0,0);
		weapon->setTag(WEAPON);
		menu->addChild(weapon);

	/*	CCMenuItemSprite* treasure = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("treasure.png"),
			CCSprite::createWithSpriteFrameName("treasure.png"),this,menu_selector(WelcomeScene::btnCallback));
		SETANCHPOS(treasure,-450,50,0,0);
		treasure->setTag(TREASURE);
		menu->addChild(treasure);*/

		CCMenuItemSprite* tip = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("tip.png"),
			CCSprite::createWithSpriteFrameName("tip.png"),this,menu_selector(WelcomeScene::btnCallback));
		//SETANCHPOS(tip,-450,-20,0,0);
		SETANCHPOS(tip,-450,10,0,0);
		tip->setTag(TIP);
		menu->addChild(tip);

		CCMenuItemSprite* clothes = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("clothes.png"),
			CCSprite::createWithSpriteFrameName("clothes.png"),this,menu_selector(WelcomeScene::btnCallback));
		//SETANCHPOS(clothes,-450,-100,0,0);
		SETANCHPOS(clothes,-450,-90,0,0);
		clothes->setTag(CLOTHES);
		menu->addChild(clothes);
		//商城提示
		CCSprite* dlg = CCSprite::create("dlg_short.png");
		SETANCHPOS(dlg,0,0,0,0);
		addChild(dlg);

		tipText = CCLabelTTF::create(conv("欢迎使用商城功能，点击类型按钮切换标签"),"arial",40);
		tipText->setDimensions(CCSizeMake(500,100));
		tipText->setColor(ccc3(102, 17, 17));
		tipText->setHorizontalAlignment(kCCTextAlignmentLeft);
		SETANCHPOS(tipText,200,20,0,0);
		addChild(tipText);

		//用户数据信息显示
		CCSprite* gold = CCSprite::createWithSpriteFrameName("gold.png");
		SETANCHPOS(gold,322,420,0,0);
		addChild(gold);

	/*	CCSprite* god = CCSprite::createWithSpriteFrameName("god.png");
		SETANCHPOS(god,350,420,0,0);
		addChild(god);

		CCSprite* star = CCSprite::createWithSpriteFrameName("star.png");
		SETANCHPOS(star,550,415,0,0);
		addChild(star);*/
		char account[20];
		sprintf(account,"%d",GameData::getGold());
		goldNum = CCLabelAtlas::create(account,"num/num_yellow.png",28,40,'0');
		SETANCHPOS(goldNum,410,425,0,0);
		addChild(goldNum);
		success = true;
	}while(0);

	return success;
}

CCScene* WelcomeScene::scene(){
	CCScene* scene = NULL;

	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		WelcomeScene* layer = WelcomeScene::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

	}while(0);


	return scene;
}

void WelcomeScene::btnCallback(CCObject* sender){
	char str[50];
	switch(((CCNode*)sender)->getTag()){
	case -2:
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		CCDirector::sharedDirector()->replaceScene(StartScene::scene());
		break;
	case -1:
		if(temp){
			if(GameData::getGold() >= temp->getValue()){
				GameData::addGold(-temp->getValue());
				sprintf(str,"%d",GameData::getGold());
				goldNum->setString(str);
				tipText->setString(conv("购买成功，已使用，谢谢"));
				GameData::addSate(temp->getType(),0,temp->getId());
				GameData::replaceSate(temp->getType(),temp->getId());
				createItems(temp->getType(),scroll->getContentOffset().x);
			}else{
				tipText->setString(conv( "对不起您的元宝不足，无法够买"));
			}
		}
		break;
	case WEAPON:
		createItems(WEAPON);
		tipText->setString(" ");
		break;
	case TREASURE:
		createItems(TREASURE);
		tipText->setString(" ");
		break;
	case TIP:
		createItems(TIP);
		tipText->setString(" ");
		break;
	case CLOTHES:
		createItems(CLOTHES);
		tipText->setString(" ");
		break;
	}
	temp = NULL;
}

void WelcomeScene::createItems(int type,float offset){
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shop.plist","shop.png");

	if(scroll){
		removeChild(scroll,true);
	}
	itemsLayer = CCLayer::create();

	int x = 0;
	int count = 0;
	int max = 0;
	bool next = true;
	int value = 0;
	if(!itemsArray){
		itemsArray = CCArray::create();
		itemsArray->retain();
	}else{
		itemsArray->removeAllObjects();
	}

	while(next){
		count++;
		//CCSprite* bg = CCSprite::createWithSpriteFrameName("item_bg.png");
		ShopItem* item = NULL;
		char name[20];
		switch(type){
		case WEAPON:
			max = 7;
			sprintf(name,"weapon/weapon%d.png",count - 1);
			value = 1000 * count;
			break;
		case TREASURE:
			max = 1;
			return;
			break;
		case TIP:
			max = 3;
			sprintf(name,"tip/tip%d.png",count * 3);
			value = 1500 * count;
			break;	
		case CLOTHES:
			max = 4;
			sprintf(name,"hero_%d.png",(count - 1));
			value = 800 * count;
			break;
		default:
			CCLog("type %d error",type);
			break;
		}
		item = new ShopItem(x,0,name,value,type,count - 1);
		itemsLayer->addChild(item->getLayer());
		itemsArray->addObject(item);
		x += item->getWidth() + 10;
		if(count >= max){
			next = false;
		}
	}
	itemsLayer->setContentSize(CCSizeMake(x,480 - 150));
	SETANCHPOS(itemsLayer,0,0,0,0);

	scroll = CCScrollView::create();
	SETANCHPOS(scroll,175,150,0,0);
	scroll->setDirection(kCCScrollViewDirectionHorizontal);
	scroll->setViewSize(CCSizeMake(max > 3 ? 854 - 150 : x,480 - 150));
	scroll->setContentSize(CCSizeMake(x,480 - 150));
	scroll->setContainer(itemsLayer);
	scroll->setContentOffset(ccp(offset,0));

	addChild(scroll);
}

void WelcomeScene::ccTouchesBegan(CCSet* touches,CCEvent* event){
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

	for(int i = 0;i < itemsArray->count();i++){
		ShopItem* item = (ShopItem*)itemsArray->objectAtIndex(i);
		if(item->isTouch(location.x - scroll->getPositionX() - scroll->getContentOffset().x,
			location.y - scroll->getPositionY())){
				/*if(item->touchAction() == 0){
				createItems(item->getType());
				}*/
				touchId = item->getId();
				lastPt = ccp(location.x,location.y);
				break;
		}
	}

}

void WelcomeScene::ccTouchesMoved(CCSet* touches,CCEvent* event){
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	if(touchId != -1){
		if(POINT_INSTANCE(location,(lastPt)) > 50){
			touchId = -1;
		}
	}
}

void WelcomeScene::ccTouchesEnded(CCSet* touches,CCEvent* event){
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	if(touchId != -1){
	for(int i = 0;i < itemsArray->count();i++){
		ShopItem* item = (ShopItem*)itemsArray->objectAtIndex(i);
		if(item->isTouch(location.x - scroll->getPositionX() - scroll->getContentOffset().x,
			location.y - scroll->getPositionY())){
				if(temp != NULL){
					temp->setSelected(false);
				}
				if(item->getId() == touchId){
					if(item->touchAction() == 0){
						createItems(item->getType(),scroll->getContentOffset().x);
						temp = NULL;
					}else{
						temp = item;
						char t[100];
						sprintf(t,"购买物品需要%d元宝，点击右侧的买入按钮即可购买",item->getValue());
						tipText->setString(conv(t));
						temp->setSelected(true);
					}
				}
				break;
		}
	}
	}
	touchId = -1;
}