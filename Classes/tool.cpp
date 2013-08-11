//#include "tool.h"ShopItem.cpp
#include "tool.h"
char* conv(const char* str){
#ifdef WIN32
	char* result = "error";
	iconv_t iconvH;
	iconvH = iconv_open("utf-8","gbk");
	if(iconvH != 0){
		const char* strChar = str;
		const char** pin = &strChar;

		size_t strLength = strlen(str);
		char* outbuf = (char*)malloc(strLength * 4);
		char* pBuff = outbuf;
		memset(outbuf,0,strLength * 4);
		size_t outLength = strLength * 4;
		if(iconv(iconvH,pin,&strLength,&outbuf,&outLength) == -1){
			iconv_close(iconvH);
		}else{
			result = pBuff;
			iconv_close(iconvH);
		}
	}
	return result;
#else
	char* r = (char*)malloc(strlen(str) + 1);
	memcpy(r,str,strlen(str));
	r[strlen(str)] = '\0';
	return r;

#endif
}

CCAnimate* createAni(const char* preName, int count,float unit,bool zero){
	CCArray* frames = CCArray::create();
	frames->retain();
	std::string format(preName);
	for(int i = 0;i < count; i++){
		char name[50];
		sprintf(name,(format + "%d.png").c_str(),zero ? i : i + 1);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
		frames->addObject(frame);
	}
	CCAnimation* ani = CCAnimation::createWithSpriteFrames(frames,unit);
	CCAnimate* animate = CCAnimate::create(ani);
	return animate;
}