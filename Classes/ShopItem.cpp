#include "ShopItem.h"
#include "GameData.h"
#include "tool.h"

ShopItem::ShopItem(int x,int y,const char* name,int v,int type, int id)
{
	layer = CCLayer::create();
	SETANCHPOS(layer,x,y,0,0);

	CCSprite* bg = CCSprite::createWithSpriteFrameName("item_bg.png");
	SETANCHPOS(bg,0,0,0,0);
	layer->addChild(bg);

	layer->setContentSize(bg->getContentSize());

	CCSprite* item = CCSprite::create(name);
	//SETANCHPOS(item,x + bg->getContentSize().width / 2,0,0.5,0);
	SETANCHPOS(item,bg->getContentSize().width / 2 ,bg->getContentSize().height / 2,0.5,0);
	layer->addChild(item);

	value = v;
	this->type = type;
	this->id = id;

	if(type == 0 ){
		CCLog("weapon id is %d", id);
		char* nameStr[] = {"木棍","铁棍","铜棍", "水晶棍", "月牙杖", "九齿耙", "金箍棒"};
		CCLabelTTF* weaponName = CCLabelTTF::create(conv(nameStr[id]),"Aria",24);
		SETANCHPOS(weaponName, bg->getContentSize().width / 2, 80, 0.5, 0.5);
		weaponName->setColor(ccc3(255,170,34));
		layer->addChild(weaponName);
	}

	int state = -1;   //未购买 的状态
	vector<int> temp =  GameData::getState(type);
	if(temp.at(0) == id){
		state = 0;  //正在使用
	}else{
		for(int i = 1;i < temp.size();i++){
			if(temp.at(i) == id){
				state = 1;  //已购买
				break;
			}
		}
	}
	if (state == -1){ //此物品还未购买
		char str[20];
		CCNode* valueText = NULL;
		if(v > 1000){
			sprintf(str,"%d",v);
			valueText = CCLabelAtlas::create(str,"num/num_yellow.png",28,40,'0');
		}else{
			sprintf(str,"%d%s",v,conv("元"));
			valueText = CCLabelTTF::create(str,"arial",35);
			((CCLabelTTF*)valueText)->setColor(ccc3(102,17,17));
		}
		SETANCHPOS(valueText,bg->getContentSize().width / 2,20,0.5,0);
		layer->addChild(valueText);
	}else{  //此物品已购买
		char* str = conv("已购买");
		ccColor3B color = ccc3(255,0,0);
		if(state == 0){ // 物品正在使用
			CCSprite* use = CCSprite::createWithSpriteFrameName("use.png");
			SETANCHPOS(use,bg->getContentSize().width / 2, bg->getContentSize().height / 2,0.5,0);
			layer->addChild(use);
			str = conv("已使用");
			color = ccc3(102, 17, 17);
		}
		CCLabelTTF* text = CCLabelTTF::create(str,"Arial",30);
		text->setColor(color);
		SETANCHPOS(text,bg->getContentSize().width / 2 ,25,0.5,0);
		layer->addChild(text);
	}

	sFlag = CCSprite::create("select.png");
	SETANCHPOS(sFlag,20,80,0,0);
	layer->addChild(sFlag);
	sFlag->setVisible(false);

}


ShopItem::~ShopItem(void)
{
}

bool ShopItem::isTouch(float x, float y){
	if(CCRectMake(layer->getPositionX(),layer->getPositionY(),
		layer->getContentSize().width,layer->getContentSize().height).containsPoint(ccp(x,y))){
			return true;
	}
	return false;
}

int ShopItem::touchAction(){
	if(!GameData::bought(type,id)){
		return -1;  //不可操作
		CCLog("this equip has not bought");
	}else{
		CCLog("this equip can be use");
		GameData::replaceSate(type,id); //点击则更新为当前装备
		return 0;  //可以装备
	}
}
