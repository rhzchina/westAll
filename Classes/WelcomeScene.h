#ifndef WELCOME_H
#define WELCOME_H
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ShopItem.h"
USING_NS_CC;
USING_NS_CC_EXT;

class WelcomeScene : public CCLayer
{
public:
	WelcomeScene(void);
	~WelcomeScene(void);

	static WelcomeScene* instance;

	bool init();
	static CCScene* scene();
	CREATE_FUNC(WelcomeScene);

	void ccTouchesBegan(CCSet*,CCEvent*);
	void ccTouchesMoved(CCSet*,CCEvent*);
	void ccTouchesEnded(CCSet*,CCEvent*);

	void btnCallback(CCObject*);
	void refresh(float dt);
	void createItems(int type,float off = 0);
	void reset(){tipped = false;}

private:
	CCScrollView* scroll;
	CCLayer* itemsLayer;
	CCLabelAtlas* goldNum;
	CCArray* itemsArray;
	int touchId;
	bool tipped;
	CCPoint lastPt;
	CCLabelTTF* tipText;
	ShopItem* temp;
};

#endif