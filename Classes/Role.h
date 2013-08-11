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
	static const int WAIT = 4;  //����Ͷ������������������ʱ���ﲻ�ɲ���״̬
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
	void weaponDone(Map*,float);  //��������
	void setProtect(bool);
	bool isProtected(){return protecting;}
	void nextLevel();
	bool isDie();
	void changeRole(int);     // ��ɫ����
	char* getChange(){return change;}
	void setDie(CCLayer*);
	void hideSprite(){hero->setVisible(false);}
private:
	bool protecting;   //�޵�״̬
	bool blink;       //�޵й���
	int state;
	char* change;          //����״̬
	bool checkOnLand;
	CCSprite* hero;
	CCSprite* effect;
	Weapon* weapon;
	bool die;  
	int cur[3];  //��ǰ�Ѽ���״̬
};
#endif