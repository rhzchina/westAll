#include "Role.h"
#include "tool.h"
#include "GameData.h"
#include "Map.h"

Role::Role(CCLayer* parent)
{

	change = "";
	int index = GameData::getState(CLOTHES).at(0);
	char plist[20];
	char name[20];
	sprintf(plist,"hero%d.plist",index);
	sprintf(name,"hero%d.png",index);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist,name);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("effect.plist","effect.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("change.plist", "change.png");

	hero = CCSprite::createWithSpriteFrameName("run_1.png");
	hero->setPosition(ccp(POSX,POSY));

	index = GameData::getState(WEAPON).at(0);
	sprintf(name,"weapon/weapon%d.png",index);
	weapon = Weapon::create(name,hero);
	parent->addChild(this);   //英雄对象也加入管理
	parent->addChild(weapon);
	parent->addChild(hero,11);
	state = 0;
	protecting = false;
	die = false;
	blink = false;

	effect = CCSprite::createWithSpriteFrameName("earth1.png");
	effect->setPosition(ccp(POSX,POSY - hero->getContentSize().height / 2));
	parent->addChild(effect,15);

	changeState(NORMAL);

	for(int i = 0;i < 3;i ++){
		cur[i] = 0;
	}
}


Role::~Role(void)
{
	delete hero;
	hero = NULL;
}

void Role::setProtect(bool b){
	protecting = b;
}

void Role::nextLevel(){
	protecting = false;
	changeState(JUMP);
	hero->runAction(CCMoveTo::create(1,ccp(POSX,0)));
}

void Role::jump(){
	if(hero){
		if(state == NORMAL){
			changeState(JUMP);
			hero->runAction(CCSequence::create(CCJumpTo::create(1,ccp(hero->getPositionX(),0),200,1),
				CCCallFunc::create(this,callfunc_selector(Role::actionCallback))
				,NULL));
		}else if(state == JUMP){
			weapon->shoot();
			state = WAIT;
		}
	}
}

void Role::hold(){
	if(state == ATTACK){
		//hero->setPositionY(hero->getPositionY() + 10);
		changeState(HOLD);
		int height = 350  - hero->getPositionY() > 0 ? 350 - hero->getPositionY() : 0;
		//float time = height / 200.0f; 
		float time = POINT_INSTANCE(ccp(POSX,POSY),ccp(hero->getPositionX(),hero->getPositionY())) / 430.0f;
		hero->runAction(CCSequence::create(
			CCJumpTo::create(time < 0.5 ? 0.5 : time,ccp(POSX, 0),height,1),
			CCCallFunc::create(this,callfunc_selector(Role::actionCallback))
			,NULL));
	}
}

void Role::resetWeapon(){
	weapon->reset();
}

void Role::actionCallback(){
	if(state == JUMP || state == WAIT){
		if(state == JUMP){
			CCLog("GAME Over");
		}
		changeState(NORMAL);
	}else if(state == ATTACK){
		hold();
	}else if(state == HOLD){
		changeState(NORMAL);
	}else if(state == OVER){
		changeState(FLY);
	}else if(state == FALL){
		die = true;
		hero->stopAllActions();
	}
}

void Role::midCallback(){
	if(state == ATTACK){
		resetWeapon();
	}

}

void Role::protectedEnd(){
	protecting = false;
	blink = false;
}

//当武器射出状态有效，改变状态
void Role::weaponDone(Map* map,float frameTime){
	state = ATTACK;
	weapon->stopAllActions();
	hero->stopAllActions();
	weapon->done(map);
	swing();
}

void Role::fall(){
	if(!protecting && state != FALL){
		state = FALL;
		effect->setVisible(false);
		hero->runAction(CCSequence::create(CCMoveTo::create(0.2f,ccp(hero->getPositionX(),0)),CCCallFunc::create(this,callfunc_selector(Role::actionCallback)),NULL));
	}
}

//荡秋千的效果
void Role::swing(){
	ccBezierConfig config;
	float sx = hero->getPositionX();
	float sy = hero->getPositionY();
	float ex = hero->getPositionX() + 100;
	float ey = 350;

	config.controlPoint_1 = ccp(sx,sy);
	config.controlPoint_2 = ccp(sx + (ex - sx) * 0.5, sy + (ey - sy) * 0.5 - 300);
	config.endPosition = ccp(ex,ey);
	hero->runAction(CCSequence::create(
		CCMoveTo::create(0.1,ccp(sx - 50,sy + 10)),
		CCEaseSineOut::create(CCBezierTo::create(0.5,config)),
		CCCallFunc::create(this,callfunc_selector(Role::midCallback)),
		CCJumpTo::create(0.4,ccp(ex,ey + 20),50,1),
		CCCallFunc::create(this,callfunc_selector(Role::actionCallback)),NULL));
}

//over参数指定是否是一关结束的飞行
void Role::fly(bool over){
	if(state != OVER && state != FLY){
		if(over){
			changeState(JUMP);
			state = OVER;
		}
		hero->runAction(CCSequence::create(CCJumpTo::create(1,ccp(POSX,300),100,1),CCCallFunc::create(this,callfunc_selector(Role::actionCallback)),NULL));
	}
}

bool Role::isDie(){
	if(effect != NULL && strcmp(change,"6/") == 0){
		SETANCHPOS(effect,hero->getPositionX(),hero->getPositionY(), 0.5,0.5);	
	}
	return die;	
}
void Role::resumeNormal(){
	int blink = strcmp(change, "6/");
	change = "";
	changeState(state, true);
	if(blink == 0 ){
		this->blink = true;
		hero->runAction(CCSequence::create(CCBlink::create(1,10),CCCallFunc::create(this,callfunc_selector(Role::protectedEnd)),NULL));
	}else{
		protecting = false;
	}
}

void Role::changeRole(int index){
	bool add = true;
	for(int i = 0;i < 3; i++){
		if(cur[i] == index){
			add = false;
			break;
		}
	}
	if(add){
		for(int i = 0;i < 3; i++){
			if(cur[i] == 0){
				cur[i] = index;
				break;
			}
		}
	}
	switch(index){
	case 4:
		if(add){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hero4.plist", "hero4.png");	
		}
		change = "4/";
		break;
	case 5:	
		if(add){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hero5.plist", "hero5.png");	
		}
		change = "5/";
		break;
	case 6:
		if(add){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hero6.plist", "hero6.png");	
		}
		change = "6/";
		protecting = true;
		break;
	}
	changeState(state,true);
}

void Role::changeState(int s, bool first){
	if(state == ATTACK){
		resetWeapon();
	}
	state = s > HOLD  ? NORMAL : s;

	if(first){
		hero->stopActionByTag(999);
	}else{
		if(!blink){
			hero->stopAllActions();
		}
	}

	if(effect->isVisible()){		
		effect->stopAllActions();
		effect->setVisible(false);
	}
	int count = 0;
	string name(change);

	switch(state){
	case NORMAL:
		if(strcmp(change, "4/") == 0){
			count = 4;
			name.append("speedup_");
		}else{
			count = 5;
			name.append("run_");
		}
		hero->setPositionY(130);
		if(strcmp(change, "4/") == 0){
			effect->runAction(CCRepeatForever::create(createAni("speed",3,0.1f)));
			effect->setPosition(hero->getPosition());
			effect->setVisible(true);
		}else if(strcmp(change, "5/") == 0){
			effect->runAction(CCRepeatForever::create(createAni("heart",3,0.1f)));
			effect->setPosition(ccp(hero->getPositionX() - 20,hero->getPositionY()));
			effect->setVisible(true);
		}else if(strcmp(change,"6/") != 0 && strcmp(change,"4/") != 0 ){
			effect->runAction(CCRepeatForever::create(createAni("earth",3,0.1f)));
			effect->setVisible(true);
			effect->setPosition(ccp(hero->getPositionX() - 15,hero->getPositionY() - 35));
		}
		break;
	case SPEEDUP:
		count = 4;
		name.append("speedup_");
		break;
	case FLY:
		count = 3;
		name.append("fly_");
		effect->runAction(CCRepeatForever::create(createAni("speed",3,0.1f)));
		effect->setPosition(hero->getPosition());
		effect->setVisible(true);
		break;
	case JUMP:
		count = 1;
		name.append("jump_");
		break;
	case HOLD:
		count = 3;
		name.append("hold_");
		break;
	}
	CCRepeatForever* repeat = CCRepeatForever::create(createAni(name.c_str(),count,0.1f,false));
	repeat->setTag(999);
	hero->setVisible(true);
	hero->runAction(repeat);

	if(strcmp(change,"6/") == 0 ){
		effect->runAction(CCRepeatForever::create(createAni("gold_circle",3,0.1f)));
		effect->setVisible(true);
		effect->setPosition(ccp(hero->getPositionX(),hero->getPositionY()));
	}
}

CCRect Role::getWeaponRange(){
	return CCRectMake(weapon->getPositionX() + weapon->getContentSize().width / 2,weapon->getPositionY(),
		weapon->getContentSize().width / 2,weapon->getContentSize().height / 2);
}

void Role::setDie(CCLayer* parent){
	CCSprite* temp = CCSprite::create("die.png");
	SETANCHPOS(temp,hero->getPositionX(), hero->getPositionY(), hero->getAnchorPoint().x, hero->getAnchorPoint().y);
	parent->removeChild(hero, true);
	hero = temp;
	parent->addChild(hero,15);

}
