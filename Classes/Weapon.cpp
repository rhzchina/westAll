#include "Weapon.h"
#include "tool.h"
#include "Map.h"
Weapon::Weapon(void)
{
	map = NULL;
}


Weapon::~Weapon(void)
{
	map = NULL;
}

void Weapon::actionCallback(){
	//shootDone = true;
	reset();
}
void Weapon::shoot(){
	setPosition(owner->getPosition());
	setVisible(true);
	float time = POINT_INSTANCE(ccp(400,480),getPosition()) / 1400.0f;
	runAction(CCSequence::create(CCMoveTo::create(time,ccp(400,480 + getContentSize().height/2)),
		CCCallFunc::create(this,callfunc_selector(Weapon::actionCallback)),
		NULL));
}

void Weapon::reset(){
	shootDone = false;
	map = NULL;
	stopAllActions(); 
	setVisible(false);
}

Weapon* Weapon::create(const char *pszFileName,CCSprite* o){
	Weapon* weapon = new Weapon();
	if(weapon && weapon->initWithFile(pszFileName)){
		weapon->autorelease();
		weapon->reset();
		SETANCHPOS(weapon,0,0,0,0);
		weapon->owner = o;
		return weapon;
	}
	CC_SAFE_DELETE(weapon);
	return NULL;
}

void Weapon::draw(){
   // glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
	ccDrawLine(ccp(owner->getPositionX() + owner->getContentSize().width / 2 - getPositionX() - getContentSize().width / 2,
		owner->getPositionY() + owner->getContentSize().height / 2 - getPositionY() - getContentSize().height / 2 - 10)
		,ccp(getContentSize().width / 2,getContentSize().height / 2));
	CCSprite::draw();
	if(shootDone){
		this->setPositionX(getPositionX() - map->getSpeed());
	}
}
