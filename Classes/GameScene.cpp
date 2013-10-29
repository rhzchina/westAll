#include "GameScene.h"
#include "StartScene.h"
#include "GameData.h"
#include <math.h>
#include "Prop.h"
GameScene::GameScene(void)
{
	scrollBg1 = NULL;
	scrollBg = NULL;
	totalTime = 0;
	count = false;
	over = false;
	bgX = 0;
	speed = 2;
	changeIndex = 0;
	speedChange = 0;
	map = NULL;
	hero = NULL;
	overText = NULL;
	items = NULL;
	pauseLayer = NULL;
	propIndex = 0;
	props[0] = CCArray::create();
	props[1] = CCArray::create();
	props[0]->retain();
	props[1]->retain();
}


GameScene::~GameScene(void)
{


}

bool GameScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayer::init());
		setTouchEnabled(true);

		initBgItems(GameData::getLevel());

		//scrollBg1 = CCSprite::createWithTexture(scrollBg->getTexture());

		overText = CCLabelTTF::create("Game OVer!!!\nTouch screen to replay,Let's go","黑体",30);
		overText->setColor(ccc3(255,0,0));
		overText->setPosition(ccp(425,240));
		overText->setVisible(false);

		//CC_BREAK_IF(!scrollBg1);

		addChild(overText,1);
		//addChild(scrollBg1);
		//scrollBg1->setAnchorPoint(ccp(0,0));

		//scrollBg1->setPosition(ccp(scrollBg->getContentSize().width,0));
		//初始化地图
		map = new Map(1,this);
		//生成主角
		hero = new Role(this);

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game.plist","game.png");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("prop_effect.plist","prop_effect.png");
		initProps(200);

		CCSprite* scorePeach = CCSprite::createWithSpriteFrameName("score_peach.png");
		SETANCHPOS(scorePeach,600,420,0,0);
		addChild(scorePeach,10);


		CCSprite* distance = CCSprite::createWithSpriteFrameName("distance.png");
		SETANCHPOS(distance,0,430,0,0);
		addChild(distance,10);

		CCSprite* best = CCSprite::createWithSpriteFrameName("best.png");
		SETANCHPOS(best,20,390,0,0);
		addChild(best,10);

		scoreValue = CCLabelAtlas::create("0","num/num_green.png",28,40,'0');
		SETANCHPOS(scoreValue,680,420,0,0);
		addChild(scoreValue,10);

		distanceValue = CCLabelAtlas::create("0","num/num_yellow.png",28,40,'0');
		SETANCHPOS(distanceValue,170,430,0,0);
		addChild(distanceValue,10);

		char b[20];
		sprintf(b,"%d",GameData::getBest());
		bestValue = CCLabelAtlas::create(b,"num/num_red.png",28,40,'0');
		SETANCHPOS(bestValue,140,390,0,0);
		addChild(bestValue,10);

		progressBg = CCSprite::createWithSpriteFrameName("progress.png");
		SETANCHPOS(progressBg,200,0,0,0);
		addChild(progressBg,14);

		progressLeaf = CCSprite::createWithSpriteFrameName("thumb_leaf.png");
		SETANCHPOS(progressLeaf,200,0,0,0);
		addChild(progressLeaf,14);

		CCMenu* menu = CCMenu::create();
		SETANCHPOS(menu,20,0,0,0);
		menu->setTag(99);
		addChild(menu,12);

		CCMenuItemSprite* pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pause.png"),
			CCSprite::createWithSpriteFrameName("pause.png"),this,menu_selector(GameScene::btnCallback));
		SETANCHPOS(pause,0,0,0,0);
		pause->setTag(1);
		menu->addChild(pause);


		schedule(schedule_selector(GameScene::bgMove));
		bRet = true;
	}while(0);
	return bRet;
}

CCScene* GameScene::scene(){
	CCScene* scene = CCScene::create();
	GameScene* layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

void GameScene::ccTouchesBegan(CCSet* touches,CCEvent* event){
	if(!CCDirector::sharedDirector()->isPaused()){
		CCTouch* touch = (CCTouch*)touches->anyObject();
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		hero->jump();

	}
}

void GameScene::ccTouchesMoved(CCSet* touches,CCEvent* event){

}

void GameScene::ccTouchesEnded(CCSet* touches,CCEvent* event){
	/*if(!CCDirector::sharedDirector()->isPaused()){
	hero->hold();
	}*/
}

void GameScene::tempCallback(CCNode* node){
	removeChild(node, true);
	hero->changeRole(changeIndex);
	if(changeIndex == 4){
		map->tempChange(SPEEDUP);
		tempChange(SPEEDUP);
	}else if(changeIndex == 5){
		map->tempChange(-SPEEDUP / 2.0f);
		tempChange(-SPEEDUP / 2.0f);
	}
}

void GameScene::bgMove(float dt){
	//游戏数据存储，根据移动的速度计算得分和距离
	if(over){
		return;
	}

	srand(time(0));
	GameData::addLoop();
	if(GameData::getLoop() % 5 == 0){
		GameData::addDistance(getSpeed() * 2);
		//GameData::addScore(1);
	}
	char v[20];
	sprintf(v,"%d",GameData::getDistance());
	distanceValue->setString(v);
	sprintf(v,"%d",GameData::getScore());
	scoreValue->setString(v);

	//地图移动
	map->mapMove(this,hero);

	//判断是否碰到道具
	for(int j = 0; j < 2; j++){
		for(int i = 0;i < props[j]->count(); i++){
			Prop* temp =  (Prop*)props[j]->objectAtIndex(i);
			temp->move(map->getSpeed());
			if(temp->collision(hero->getSprite())){
				if(temp->getType() == Prop::PROP){
					changeIndex = rand() % 3 + 4;
					hero->hideSprite();
					CCSprite* temp = CCSprite::create();
					SETANCHPOS(temp, hero->getSprite()->getPositionX(),hero->getSprite()->getPositionY(),0.5,0.5);
					addChild(temp,20);
					temp->runAction(CCSequence::create(createAni("change",5,0.05f),CCCallFuncN::create(this,callfuncN_selector(GameScene::tempCallback)),NULL));

				}else{
					GameData::addScore(temp->getScore());
				}
				temp->setCollision(this);
			}
		}
	}

	if(((Prop*)(props[propIndex]->lastObject()))->getPosX() < 0){
		for(int i = 0;i < props[propIndex]->count();i++){
			props[propIndex]->objectAtIndex(i)->release();
		}

		props[propIndex]->removeAllObjects();
		if(++propIndex > 1){
			propIndex = 0;
		}

		if(map->getPercent() > 0.7){
			initProps(2200);
		}else{
			initProps(854);
		}

	}

	if(hero->getState() != Role::ATTACK){
		/*	map->clearChange();
		clearChange();*/

		if(hero->getSprite()->getPositionY() < 130){
			if(map->onLand(hero->getSprite())){
				if(hero->getState() != Role::FALL && hero->getState() != Role::ATTACK){
					hero->changeState(Role::NORMAL);
				}else if(hero->getState() == Role::ATTACK){
					hero->hold();
				}
			}else{
				if(!hero->isProtected() && hero->getState() != Role::ATTACK && hero->getState() != Role::HOLD){

					hero->fall();
				}else if(hero->isProtected()){
					hero->changeState(Role::NORMAL);
				}
			}
		}else if(hero->getSprite()->getPositionY() == 130){
			if(!map->onLand(hero->getSprite()) && !hero->isProtected()) {
				hero->fall();
			}else if(hero->getState() == Role::FALL && hero->isProtected()){
				hero->changeState(Role::NORMAL);
			}
		}
	}else{
		/*	map->tempChange(SPEEDUP);
		tempChange(SPEEDUP);*/
	}

	//武器是否否使用
	if(map->weaponOn(hero)){
		hero->weaponDone(map,dt);
	}

	if(hero->isDie()){
		over = true;	
		hero->setDie(this);
		CCSprite* temp = hero->getSprite();
		temp->runAction(CCSequence::create(CCJumpTo::create(0.8, ccp(temp->getPositionX(),-200),300,1),CCCallFunc::create(this, callfunc_selector(GameScene::gameOver)),NULL));
		//gameOver();
	}else if(strcmp(hero->getChange(), "") != 0){  //变身状态
		totalTime += dt;
		if(totalTime > 8){
			hero->resumeNormal();
			totalTime = 0;
			map->clearChange();
			clearChange();
		}
	}

	if(items->getPositionX() > -items->getContentSize().width){
		items->setPositionX(items->getPositionX() - getSpeed() / 2);
	}else{
		if(rand() % 10 == 5){
			items->setPositionX(854);
		}
	}
}

void GameScene::initBgItems(int level){
	char dir[20];
	sprintf(dir,"land%d/",level);
	string name(dir);
	scrollBg = CCSprite::create(name.append("bg.jpg").c_str());
	SETANCHPOS(scrollBg,0,0,0,0);
	addChild(scrollBg,-2);

	char tempName[20];
	CCSprite* temp = NULL;
	items = CCLayer::create();
	int x = 0;
	switch(level){
	case 1:
		name.replace(name.begin(),name.end(),dir);

		temp = CCSprite::create(name.append("rainbow.png").c_str());
		SETANCHPOS(temp,200, 200, 0.5, 0);
		items->addChild(temp);

		name.replace(name.begin(),name.end(),dir);
		temp = CCSprite::create(name.append("small_hill.png").c_str());
		SETANCHPOS(temp,300, 0, 0.5, 0);
		items->addChild(temp);

		name.replace(name.begin(),name.end(),dir);
		temp = CCSprite::create(name.append("hill.png").c_str());
		SETANCHPOS(temp,600, 0, 0.5, 0);
		items->addChild(temp);

		for(int i = 0;i < 8; i++){
			sprintf(tempName,"land%d/bloud%d.png",level,i);
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,150 + rand() % 100,0,0);
			items->addChild(temp);
			x += temp->getContentSize().width;
		}


		break;
	case 2:

		for(int i = 1;i < 3; i++){
			sprintf(tempName,"land%d/frog%d.png",level,i);
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,300 + rand() % 100,0,0);
			items->addChild(temp);
			x += temp->getContentSize().width;
		}

		name.replace(name.begin(),name.end(),dir);
		temp = CCSprite::create(name.append("hill1.png").c_str());
		SETANCHPOS(temp,300, 0, 0.5, 0);
		items->addChild(temp);

		name.replace(name.begin(),name.end(),dir);
		temp = CCSprite::create(name.append("hill2.png").c_str());
		SETANCHPOS(temp,600, 0, 0.5, 0);
		items->addChild(temp);

		name.replace(name.begin(),name.end(),dir);
		temp = CCSprite::create(name.append("water.png").c_str());
		SETANCHPOS(temp,500, 0, 0.5, 0);
		items->addChild(temp);

		for(int i = 1;i < 3; i++){
			sprintf(tempName,"land%d/frog%d.png",level,i);
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,150 + rand() % 100,0,0);
			items->addChild(temp);
			x += temp->getContentSize().width;
		}
		break;
	case 3:
		items->setContentSize(CCSizeMake(1380,480));
		//水晶宫
		name.replace(name.begin(),name.end(),dir);
		temp = CCSprite::create(name.append("sea_house.png").c_str());
		SETANCHPOS(temp,400,0,0.5,0);
		items->addChild(temp);
		//海中的山
		for(int i = 0; i < 3;i++){
			sprintf(tempName,"land%d/hill%d.png",level,(i+1));
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,0,0,0);
			items->addChild(temp,-1);
			x += temp->getContentSize().width;
		}
		//水晶宫前门牌
		temp = CCSprite::create(name.replace(name.begin(),name.end(),dir).append("plate.png").c_str());
		SETANCHPOS(temp,700,0,0,0);
		items->addChild(temp);
		//珊瑚和石头hk:海葵
		x =100;
		for(int i = 0;i < 5; i++){
			srand(time(0) + i);
			sprintf(tempName,"land%d/stone%d.png",level,(rand() % 5 + 1));
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,0,0,0);
			items->addChild(temp,3);
			int height = temp->getContentSize().height;
			srand(time(0) - i);
			sprintf(tempName,"land%d/plant%d.png",level,(rand() % 5 + 1));
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,height / 3,0,0);
			items->addChild(temp,2);
			int space = temp->getContentSize().width;

			if(rand() % 5 == i){
				sprintf(tempName,"land%d/hk.png",level);
				temp = CCSprite::create(tempName);
				SETANCHPOS(temp,x,0,0,0);
				items->addChild(temp,3);
			}
			srand(time(0) + (i + 1) * 2);
			x += space * (rand() % 3 + 2);
		}
		break;
	case 4:
		//石人碉像
		name.replace(name.begin(),name.end(),dir);
		temp = CCSprite::create(name.append("stone.png").c_str());
		SETANCHPOS(temp,854,0,1,0);
		items->addChild(temp);
		//背后的山	
		for(int i = 0; i < 2;i++){
			sprintf(tempName,"land%d/hill%d.png",level,(i+1));
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,0,0,0);
			items->addChild(temp,-1);
			x += temp->getContentSize().width;
		}
		//前方的草地
		x = 0;
		for(int i = 0; i < 8;i++){
			sprintf(tempName,"land%d/grass%d.png",level,rand() % 2 + 1);
			temp = CCSprite::create(tempName);
			SETANCHPOS(temp,x,rand() % 30,0,0);
			items->addChild(temp,1);
			x += temp->getContentSize().width * 3 / (rand() % 2 + 2);
		}
		break;
	}
	SETANCHPOS(items,0,0,0,0);
	addChild(items);
}

void GameScene::initProps(float xPos){
	int randProp;
	if(strcmp(hero->getChange(),"") == 0 ){
		randProp = 2;
	}else{
		randProp = 0;
	}

	int p1[][10] = {
		{0,0,0,1,1,1,1,0,0,0},
		{0,0,1,0,0,0,0,1,0,0},
		{0,1,0,0,0,0,0,0,1,0},
		{1,0,0,0,randProp,0,0,0,0,1}};

		int p2[][10] = { 
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,randProp,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1}};

			int p3[][10] = {{1,0,0,randProp,0,0,1,0,0,0},
			{0,1,0,0,0,1,0,1,0,0},
			{0,0,1,0,1,0,0,0,1,0},
			{0,0,0,1,0,0,0,0,0,1}};

			int **p = new int*[4];
			for(int i = 0;i < 4; i++){
				p[i] = new int[10];
			}

			switch(rand() % 3){
			case 0:
				for(int i = 0;i < 4;i++){
					for(int j = 0;j < 10; j++){
						p[i][j] = p1[i][j];
					}
				}	
				break;
			case 1:
				for(int i = 0;i < 4;i++){
					for(int j = 0;j < 10; j++){
						p[i][j] = p2[i][j];
					}
				}	
				break;
			case 2:
				for(int i = 0;i < 4;i++){
					for(int j = 0;j < 10; j++){
						p[i][j] = p3[i][j];
					}
				}
				break;
			}

			float x = xPos, y = 300;
			for(int i = 0;i < 4; i++){
				for(int j = 0; j < 10; j++){
					if(p[i][j] != 0){
						props[propIndex]->addObject(new Prop(p[i][j],this,x,y));	
					}
					x += 60;
				}
				x = xPos;
				y -= 50;
			} 
}

void GameScene::btnCallback(CCObject* sender){
	switch(((CCNode*)sender)->getTag()){
	case 1:
		CCDirector::sharedDirector()->pause();	
		showPauseLayer(true);
		break;
	case 2:
		callCharge(1);
		showPauseLayer(false);
		CCDirector::sharedDirector()->resume();
		break;
	case 3: //重试
		GameData::reset(false);
		CCDirector::sharedDirector()->resume();
		CCDirector::sharedDirector()->replaceScene(GameScene::scene());
		break;
	case 4:
		GameData::reset(false); //返回
		CCDirector::sharedDirector()->resume();
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		CCDirector::sharedDirector()->replaceScene(SelectScene::scene());
		break;
	case 5:
		GameData::reset(false);
		CCDirector::sharedDirector()->resume();
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		CCDirector::sharedDirector()->replaceScene(StartScene::scene());
		break;
	}
}
void GameScene::gameOver(){
	removeChild(getChildByTag(99),true);
	unschedule(schedule_selector(GameScene::bgMove));
	CCLayerColor* ol = CCLayerColor::create(ccc4(0,0,0,150));
	addChild(ol,20);

	CCSprite* overBg = CCSprite::create("over_bg.png");
	SETANCHPOS(overBg,425,240,0.5,0.5);
	addChild(overBg,21);

	CCMenu* menu = CCMenu::create();
	SETANCHPOS(menu,425,240,0.5,0.5);
	addChild(menu,21);

	CCMenuItemSprite* retry = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("retry.png"),
		CCSprite::createWithSpriteFrameName("retry.png"),this,menu_selector(GameScene::btnCallback));
	SETANCHPOS(retry,-180,-110,0,0);
	retry->setTag(3);
	menu->addChild(retry);

	CCMenuItemSprite* play = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("play.png"),
		CCSprite::createWithSpriteFrameName("play.png"),this,menu_selector(GameScene::btnCallback));
	SETANCHPOS(play,80,-110,0,0);
	play->setTag(4);
	menu->addChild(play);

	CCSprite* dis = CCSprite::createWithSpriteFrameName("distance.png");
	SETANCHPOS(dis,350,300,0.5,0.5);
	addChild(dis,21);

	char v[20];
	sprintf(v,"%d",GameData::getDistance());
	CCLabelAtlas* disText =CCLabelAtlas::create(v,"num/num_yellow.png",28,40,'0');
	SETANCHPOS(disText,430,280,0,0);
	addChild(disText,21);

	CCSprite* best = CCSprite::createWithSpriteFrameName("best.png");
	SETANCHPOS(best,350,250,0.5,0.5);
	addChild(best,21);

	sprintf(v,"%d",GameData::getBest() > GameData::getDistance() ? GameData::getBest() : GameData::getDistance());
	CCLabelAtlas* bestText =CCLabelAtlas::create(v,"num/num_red.png",28,40,'0');
	SETANCHPOS(bestText,410,230,0,0);
	addChild(bestText,21);

}


void GameScene::showPauseLayer(bool show){
	if(show){
		if(!pauseLayer){
			pauseLayer = CCLayer::create();
			CCSprite* bg = CCSprite::create("dlg_long.png");
			SETANCHPOS(bg,0,0,0,0);
			pauseLayer->addChild(bg);

			CCMenu* menu = CCMenu::create();
			SETANCHPOS(menu,0,0,0,0);
			pauseLayer->addChild(menu);

			CCMenuItemSprite* play = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("play.png"),
				CCSprite::createWithSpriteFrameName("play.png"),this,menu_selector(GameScene::btnCallback));
			play->setTag(2);
			SETANCHPOS(play,200,25,0,0);
			menu->addChild(play);

			CCMenuItemSprite* retry = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("retry.png"),
				CCSprite::createWithSpriteFrameName("retry.png"),this,menu_selector(GameScene::btnCallback));
			SETANCHPOS(retry,350,25,0,0);
			retry->setTag(3);
			menu->addChild(retry);

			CCMenuItemSprite* back = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("back.png"),
				CCSprite::createWithSpriteFrameName("back.png"),this,menu_selector(GameScene::btnCallback));
			SETANCHPOS(back,500,25,0,0);
			back->setTag(4);
			menu->addChild(back);

			CCMenuItemSprite* main = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("menu.png"),
				CCSprite::createWithSpriteFrameName("menu.png"),this,menu_selector(GameScene::btnCallback));
			SETANCHPOS(main,650,25,0,0);
			main->setTag(5);
			menu->addChild(main);

			addChild(pauseLayer,15);
		}
	}else{
		removeChild(pauseLayer,true);
		pauseLayer = NULL;
	}
}