#ifndef LOGOSCENE_H
#define LOGOSCENE_H
#include "cocos2d.h"
#include "tool.h"

USING_NS_CC;
class LogoScene : public CCLayer
{
public:
	LogoScene(void);
	~LogoScene(void);

	CREATE_FUNC(LogoScene);
	bool init();
	void ccTouchesBegan(CCSet*, CCEvent*);
	void next(float);
	static CCScene* scene();
};

#endif