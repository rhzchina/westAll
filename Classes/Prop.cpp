#include "Prop.h"
#include "tool.h"


Prop::Prop(int type, CCLayer* parent, float x, float y)
{
	colli = false;
	this->type = type;
	char name[50];
	switch(type){
	case SCORE:
		switch(rand() % 4){
		case 0:
			sprintf(name,"peach.png");
			score = 10;
			break;
		case 1:
			sprintf(name,"dragon_scale.png");
			score = 15;
			break;
		case 2:
			sprintf(name,"fire.png");
			score = 20;
			break;	
		case 3:
			sprintf(name,"bamboo.png");
			score = 30;
			break;
		}
		prop = CCSprite::createWithSpriteFrameName(name);
		break;
	case PROP:
		prop = CCSprite::createWithSpriteFrameName("random.png");
		score = 0;
		break;
	}
	SETANCHPOS(prop, x, y, 0, 0.5);
	parent->addChild(prop);
}


Prop::~Prop(void)
{
	prop->removeFromParentAndCleanup(true);

	prop = NULL;
}

bool Prop::move(float speed){
	bool remove = false;
	prop->setPositionX(prop->getPositionX() - speed);
	if(prop->getPositionX() + prop->getContentSize().width < 0){
		remove = true;
	}
	return remove;
}

bool Prop::collision(CCSprite* hero){
	if(colli){
		return false;
	}else{
		return CCRectMake(prop->getPositionX(),prop->getPositionY(),prop->getContentSize().width,prop->getContentSize().height).intersectsRect(CCRectMake(
			hero->getPositionX(),hero->getPositionY(),hero->getContentSize().width / 2,hero->getContentSize().height / 2));
	}
}

void Prop::setCollision(CCLayer* parent){
	colli = true;
	prop->runAction(CCSequence::create(createAni("prop_",4,0.1f),CCCallFuncN::create(prop,callfuncN_selector(Prop::callback)),NULL));

	if(type == SCORE){
		char num[50];
		sprintf(num,"%d",score);
		CCLabelAtlas* scoreAni = CCLabelAtlas::create(num,"num/num_red.png",28,40,'0');
		//SETANCHPOS(scoreAni,prop->getPositionX(), prop->getPositionY(), 0 ,0);
		SETANCHPOS(scoreAni,0, 0, 0 ,0);
		CCLayer* temp = CCLayer::create();
		SETANCHPOS(temp,prop->getPositionX(), prop->getPositionY(), 0 ,0);
		temp->addChild(scoreAni);
		
		CCSprite* add = CCSprite::createWithSpriteFrameName("add.png");
		SETANCHPOS(add, -add->getContentSize().width, 0, 0, 0);
		temp->addChild(add);
			
		parent->addChild(temp,10);
		temp->setTag(1);
		temp->runAction(CCSpawn::create(CCMoveTo::create(0.5,ccp(prop->getPositionX(),prop->getPositionY() + 100)),CCFadeOut::create(0.5),NULL));
		temp->runAction(CCSequence::create(CCEaseBounceOut::create(CCScaleTo::create(0.5,1.5,1.5)),CCCallFuncN::create(scoreAni,callfuncN_selector(Prop::callback)),NULL));
	}
}

void Prop::callback(CCNode* sender){
	sender->setVisible(false);
	if(sender->getTag() == 1){
		sender->removeAllChildrenWithCleanup(true);
	}
}