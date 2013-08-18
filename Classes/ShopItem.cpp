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
		char* nameStr[] = {"ľ��","����","ͭ��", "ˮ����", "������", "�ųݰ�", "�𹿰�"};
		CCLabelTTF* weaponName = CCLabelTTF::create(conv(nameStr[id]),"Aria",24);
		SETANCHPOS(weaponName, bg->getContentSize().width / 2, 80, 0.5, 0.5);
		weaponName->setColor(ccc3(255,170,34));
		layer->addChild(weaponName);
	}

	int state = -1;   //δ���� ��״̬
	vector<int> temp =  GameData::getState(type);
	if(temp.at(0) == id){
		state = 0;  //����ʹ��
	}else{
		for(int i = 1;i < temp.size();i++){
			if(temp.at(i) == id){
				state = 1;  //�ѹ���
				break;
			}
		}
	}
	if (state == -1){ //����Ʒ��δ����
		char str[20];
		CCNode* valueText = NULL;
		if(v > 1000){
			sprintf(str,"%d",v);
			valueText = CCLabelAtlas::create(str,"num/num_yellow.png",28,40,'0');
		}else{
			sprintf(str,"%d%s",v,conv("Ԫ"));
			valueText = CCLabelTTF::create(str,"arial",35);
			((CCLabelTTF*)valueText)->setColor(ccc3(102,17,17));
		}
		SETANCHPOS(valueText,bg->getContentSize().width / 2,20,0.5,0);
		layer->addChild(valueText);
	}else{  //����Ʒ�ѹ���
		char* str = conv("�ѹ���");
		ccColor3B color = ccc3(255,0,0);
		if(state == 0){ // ��Ʒ����ʹ��
			CCSprite* use = CCSprite::createWithSpriteFrameName("use.png");
			SETANCHPOS(use,bg->getContentSize().width / 2, bg->getContentSize().height / 2,0.5,0);
			layer->addChild(use);
			str = conv("��ʹ��");
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
		return -1;  //���ɲ���
		CCLog("this equip has not bought");
	}else{
		CCLog("this equip can be use");
		GameData::replaceSate(type,id); //��������Ϊ��ǰװ��
		return 0;  //����װ��
	}
}
