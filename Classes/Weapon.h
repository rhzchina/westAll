#ifndef WEAPON_H
#define WEAPON_H
#include "cocos2d.h"

USING_NS_CC;
class Map;
class Weapon : public CCSprite
{

public:
	static Weapon* create(const char *,CCSprite*);

	Weapon(void);
	~Weapon(void);
	void shoot();
	void draw();
	void reset();
	void actionCallback();
	void done(Map* m){shootDone = true;map = m;}
	bool isShootDone(){return shootDone;}
private:
	CCSprite* owner;
	bool shootDone;  //���и�ʯ
	Map* map;  //��ͼ�����ã�����ȡ��ǰ��ͼ�ٶ�

};
#endif