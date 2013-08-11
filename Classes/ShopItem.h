#ifndef SHOPITEM_H
#define SHOPITEM_H
#include "cocos2d.h"
USING_NS_CC;

class ShopItem : public CCObject
{
public:
	ShopItem(int x,int y,const char* name,int v,int type, int id);
	~ShopItem(void);

	CCLayer* getLayer(){return layer;}
	float getWidth(){return layer->getContentSize().width;}
	bool isTouch(float,float);

	int getValue(){return value;}
	int touchAction();

	int getType(){return type;}
	int getId(){return id;}

	void setSelected(bool s){selected = s;sFlag->setVisible(s);}
private:
	CCLayer* layer;
	CCSprite* sFlag;
	int value;
	int type;
	int id;
	bool selected;
};

#endif