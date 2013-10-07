#ifndef GAMEDATA_H
#define GAMEDATA_H
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class GameData : public CCObject
{
public:
	static bool music;
	static bool checked;  //全局的变量记录是否已检测
	static bool payForGame;  //已付费

	static GameData* getInstance();
	static bool initInstance(bool force = false);
	~GameData();
	static int getLevel();
	static	void addLevel();
	static void setLevel(int);

	static int getMaxLevel(){return instance->max;}

	static void setGold(int g){instance->gold = g;}
	//static int getGold(){return instance->gold;}
	static void addGold(int change){
		instance->score += change;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("score", instance->score);
		CCUserDefault::sharedUserDefault()->flush();	
	}

	//static void setGold(int g){instance->gold = g;}
	static int getGold(){return instance->score;}
	//static void addGold(int change){instance->gold += change;}

	//购买时存储数据
	static void addSate(int type,int state,int v){
		instance->data[type].push_back(v);
		char key[50];
		memset(key,0,50);
		sprintf(key,"type%d%dbuy",type, v);
		CCUserDefault::sharedUserDefault()->setBoolForKey(key,true);
		CCUserDefault::sharedUserDefault()->flush();
	}

	static void replaceSate(int type,int v){
		instance->data[type][0] = v;
		char key[50];
		sprintf(key,"type%duse",type);
		CCUserDefault::sharedUserDefault()->setIntegerForKey(key,v);
		CCUserDefault::sharedUserDefault()->flush();
	}

	static bool bought(int type,int id);
	static vector<int> getState(int type){return instance->data[type];}

	static void addScore(int s){instance->score += s;}
	static int getScore(){return instance->score;}

	static int getDistance(){return instance->distance;}
	static void addDistance(int);
	static int getBest(){return instance->best;}
	static void reset(bool all);

	static void addLoop(){instance->loopCount++;}
	static long getLoop(){return instance->loopCount;}
	static void pay();
	static bool isPay();
	void clearData(float dt);
	void callPay(float dt);
private:
	GameData(void);
	int level;
	int max;
	int gold;
	int score;
	int distance;
	int best;
	vector<int> data[4];
	long loopCount;  //循环计数
	static GameData* instance;
	
};
#endif                                                    