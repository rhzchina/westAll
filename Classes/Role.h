#ifndef ROLE_H
#define ROLE_H
#include "cocos2d.h"
#include "Weapon.h"
USING_NS_CC;
class Map;
class Role : public CCNode 
{
public:
	static const int NORMAL = 0;
	static const int SPEEDUP = 1;
	static const int FLY = 2;
	static const int JUMP = 3;
	static const int WAIT = 4;  //跳起投出武器，当武器飞行时人物不可操作状态
	static const int ATTACK = 5;
	static const int HOLD = 6;
	static const int FALL = 7;
	static const int OVER = 8;

	static const int POSX = 200;
	static const int POSY = 130;
	Role(CCLayer*);
	~Role(void);
	void changeState(int, bool = false);
	void resumeNormal();
	int getState(){return state;}
	CCSprite* getSprite(){return hero;}
	void jump();
	void hold();
	void swing();
	void actionCallback();
	void midCallback();
	void protectedEnd();
	void fall();
	void fly(bool);
	void resetWeapon();
	CCRect getWeaponRange();
	void weaponDone(Map*,float);  //武器击中
	void setProtect(bool);
	bool isProtected(){return protecting;}
	void nextLevel();
	bool isDie();
	void changeRole(int);     // 角色变身
	char* getChange(){return change;}
	void setDie(CCLayer*);
	void hideSprite(){hero->setVisible(false);}
private:
	bool protecting;   //无敌状态
	bool blink;       //无敌过渡
	int state;
	char* change;          //变身状态
	bool checkOnLand;
	CCSprite* hero;
	CCSprite* effect;
	Weapon* weapon;
	bool die;  
	int cur[3];  //当前已加载状态
};
#endif