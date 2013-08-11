#ifndef STARSCENE_H
#define STARTSCENE_H
#include "cocos2d.h"
#include "tool.h"
#include "SelectScene.h"

USING_NS_CC;
class StartScene : public CCLayer
{
public:
	StartScene(void);
	~StartScene(void);

	CREATE_FUNC(StartScene);
	bool init();
	void btnCallback(CCObject*);
	static CCScene* scene();

private:
	CCMenuItemSprite* soundOn;
	CCMenuItemSprite* soundOff;
};

#endif