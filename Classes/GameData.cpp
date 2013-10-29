#include "GameData.h"
#include "tool.h"
#include "StartScene.h"

GameData* GameData::instance = NULL;
bool GameData::checked = false;
bool GameData::payForGame = false;
bool GameData::music = true;

GameData::GameData(void)
{
	level = 1;
	//最大解锁关
	if(CCUserDefault::sharedUserDefault()->getIntegerForKey("max") == 0){
		max = 1;
	}else{
		max = CCUserDefault::sharedUserDefault()->getIntegerForKey("max");
	}

	//最好成绩
	if(CCUserDefault::sharedUserDefault()->getIntegerForKey("best") == 0){
		best = 0;
	}else{
		best = CCUserDefault::sharedUserDefault()->getIntegerForKey("best");
	}
	
	//当前货币 
	if(CCUserDefault::sharedUserDefault()->getIntegerForKey("score") == 0){
		score = 0;
	}else{
		score = CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
	}
	int nums[] = {7,1,3,4};

	gold = 0;
	distance = 0;
	loopCount = 0;
	//memset(itemState,0,sizeof(itemState));
	for(int i = 0;i < 4;i++){
		data[i] = vector<int>();
		for(int j = 0;j < nums[i] + 1;j++){ //第一个是当前装备的，因此循环要多一次
			char key[50];
			memset(key, 0, 50);
			if(j == 0){
				sprintf(key, "type%duse", i);
				data[i].push_back(CCUserDefault::sharedUserDefault()->getIntegerForKey(key));
			}else if (j == 1){
				data[i].push_back(0);
			}else{
				sprintf(key, "type%d%dbuy",i, j - 1);
				if(CCUserDefault::sharedUserDefault()->getBoolForKey(key)){
					data[i].push_back(j - 1);
				}
			}
		}
	}
	//if(!checked){
	//	//CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameData::callPay),this,0,0,180,false);
	//	checked = true;
	//}
}

GameData::~GameData(void)
{
	instance = NULL;
	checked = false;
	payForGame = false;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("best", 0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("score",0);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("max",0);
	CCUserDefault::sharedUserDefault()->flush();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
	CCDirector::sharedDirector()->replaceScene(StartScene::scene());
}

void GameData::clearData(float dt){
	checked = false;
	payForGame = false;

	delete instance;
}

GameData* GameData::getInstance(){
	if(instance == NULL){
		instance = new GameData();
	}
	return instance;
}

bool GameData::initInstance(bool force){

	if(instance == NULL || force){
		instance = new GameData();
		return true;
	}
	return false;
}

void GameData::addLevel(){
	instance->level++;
	if(instance->level > 4){
		instance->level = 1;
	}
}

void GameData::setLevel(int l){
	instance->level = l;
}

int GameData::getLevel(){
	return instance->level;
}

void GameData::reset(bool all){
	if(all){
		instance->best = 0;
		instance->score = 0;
	}else{
		instance->best = instance->best > instance->distance ? instance->best : instance->distance;		
		CCUserDefault::sharedUserDefault()->setIntegerForKey("best", instance->best);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("score", instance->score);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("max", instance->max);
		CCUserDefault::sharedUserDefault()->flush();
	}
	instance->loopCount = 0;
	instance->distance = 0;
	
}
                                                                                                                                                                                                                                                                                                                                                                                                              
bool GameData::bought(int type,int id){
	for(int i = 1;i < instance->data[type].size();i++){
		if(instance->data[type].at(i) == id){
			return true;
		}
	}
	return false;
}


void GameData::addDistance(int d){
	instance->distance += d;
	if(GameData::isPay()){
		if(instance->distance > 7500){
			instance ->max = 4;
		}else if(instance->distance > 5500){
			instance ->max = 3;
		}/*else if(instance->distance > 2700){
			instance ->max = 2;*/
		/*	if(!checked){
				instance->callPay(0);
				checked = true;
			}*/
		//}
	}
}

void GameData::pay(){
	CCUserDefault::sharedUserDefault()->setBoolForKey("pay", true);
	CCUserDefault::sharedUserDefault()->flush();
}

bool GameData::isPay(){
	return CCUserDefault::sharedUserDefault()->getBoolForKey("pay");
}

void GameData::callPay(float dt){
	CCLog("come here to check is pay");
	callCharge(1);
	CCDirector::sharedDirector()->pause();
}