#ifndef PROP_H
#define PROP_H
#include "cocos2d.h"
USING_NS_CC;

class Prop : public CCObject
{
public:
	static const int SCORE = 1;
	static const int PROP = 2;
	
	Prop(int type, CCLayer* parent,float x, float y);
	~Prop();

	bool move(float speed);
	bool collision(CCSprite*);
	float getPosX(){return prop->getPositionX();}
	void setCollision(CCLayer*);
	void callback(CCNode*);
	int getScore(){return score;}
	int getType(){return type;}
private:
	CCSprite* prop;
	bool colli;  
	int score;
	int type;
};

#endif