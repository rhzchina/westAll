#ifndef TOOL_H 
#define TOOL_H
#include <math.h>
#include <string>
#include "cocos2d.h"
USING_NS_CC;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	#include "platform/android/jni/JniHelper.h"  
#include "libiconv/include/iconv.h"
#else
	#include <JniHelper.h>
	#include "iconv\iconv.h"
#endif

extern "C"{
  char* conv(const char* str);
  CCAnimate* createAni(const char* preName, int count,float unit,bool zero = true);
  void callCharge(int);
  bool checkPay(int);
}


#define POINT_INSTANCE(_p1,_p2) \
	abs(sqrt((_p1.x - _p2.x) * \
	(_p1.x - _p2.x) + \
	(_p1.y - _p2.y) * \
	(_p1.y - _p2.y)))

#define SETANCHPOS(target,x,y,anchX,anchY) \
	target->setPosition(ccp(x,y)); \
	target->setAnchorPoint(ccp(anchX,anchY));
#endif

#define WEAPON  0
#define TREASURE  1
#define TIP  2
#define CLOTHES  3