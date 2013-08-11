#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "cocos2d.h"
#include "Map.h"
#include "Role.h"

USING_NS_CC;
class GameScene : public CCLayer 
{
public:
	static const int SPEEDUP = 3;  //速度的临时改变量 
	GameScene(void);
	~GameScene(void);

	bool init();
	void initBgItems(int);
	void bgMove(float);
	void ccTouchesBegan(CCSet* ,CCEvent*);
	void ccTouchesMoved(CCSet*,CCEvent*);
	void ccTouchesEnded(CCSet*,CCEvent*);

	void btnCallback(CCObject*);
	void showPauseLayer(bool);
	void addSpeed(float add){speed += add;}
	void tempChange(float temp){speedChange = temp;} //速度的临时改变量
	void clearChange(){speedChange = 0;}
	float getSpeed(){return speed + speedChange;}
	void gameOver();
	void setProgress(float percent){progressLeaf->setPositionX(progressBg->getPositionX() + percent *  progressBg->getContentSize().width) ;}
	void tempCallback(CCNode*);
	void initProps(float xPos);

	static CCScene* scene();
	CREATE_FUNC(GameScene);

private:
	CCSprite* scrollBg;
	CCSprite* scrollBg1;
	CCSprite* sky;
	CCSprite* progressBg;
	CCSprite* progressLeaf;
	Map* map;
	Role* hero;
	float bgX;
	float speed;
	float speedChange; 
	CCLayer* pauseLayer;
	CCLayer* items;
	CCLabelTTF* overText;
	CCLabelAtlas* scoreValue;
	CCLabelAtlas* distanceValue;
	CCLabelAtlas* bestValue;
	CCArray*  props[2];	
	int propIndex;
	float totalTime;
	bool count;
	bool over;
	int changeIndex;
	
};
#endif