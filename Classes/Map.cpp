#include "Map.h"
#include "GameData.h"
#include "GameScene.h"

Map::Map(int level,GameScene* parent)
{
	/*map = NULL;
	mapUp = false;                                                          
	mapData = vector<int>();
	createData(level);
	initMap(1);
	curLevel = 1;*/
	speed = 5.5f;
	speedChange = 0;
	map = CCArray::create();
	map->retain();
	resetMap(level,parent);
}

Map::~Map(void)
{
}

void Map::resetMap(int level,GameScene* parent, bool end){
	curLevel = level;
	distance = 0;
	countDistance = false;
	startCur = false;
	curDis = 0;
	mapUp = false;
	if(&mapData == NULL){
		mapData = vector<int>();
	}else{
		mapData.clear();
	}
	createData(level);
	initMap(GameData::getLevel(), end);
	addMap(parent);
}

void Map::createData(int l){
	if(l > 1){
		for(int i = 0; i < 13; i++){
			mapData.push_back(0);
		}
	}else{
		startCur = true;
	}
	int level = rand() % 9 + 1;
	CCLog("%d",level);
	if(level == 1){
		int array[] = {1, 2, 6, 2, 3, 0, 4, 0	, 0, 6, 0, 0, 0, 1, 2,
			3, 0, 5, 0,7,8,9};
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 2){
		int array[] =  { 1, 2, 6, 2, 3, 0, 5, 0, 5, 0, 4, 0, 0, 6, 0, 0, 0, 1, 2, 3,
			0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0,7,8,9 };
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 3){
		int array[] =  { 1, 2, 6, 2, 6, 2, 3, 0, 4, 0, 0, 6, 0, 0, 0, 1, 3, 0, 5, 0,
			1, 3, 0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0,7,8,9 };
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 4){
		int array[] =  {  1, 2, 6, 2, 3, 0, 4, 0, 0, 6, 0, 0, 0, 1, 2, 3, 0, 0, 6, 0, 0, 0, 1, 3, 0, 0, 6, 0, 0,
			0, 1, 3, 0, 5, 0, 0, 6, 0, 0, 0, 1, 3, 0, 5, 0, 1, 2, 6, 2, 3, 0, 5, 0, 4, 0 ,7,8,9};
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 5){
		int array[] =  { 1, 2, 6, 2, 3, 0, 4, 0, 0, 6, 0, 0, 0, 1, 2, 3, 0, 5, 0, 5,
			0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0 ,7,8,9};
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 6){
	int array[] =  { 1, 2, 6, 2, 3, 0, 4, 0, 0, 6, 0, 0, 0, 1, 3, 0, 5, 0, 1, 3,
			0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0, 1, 3, 0, 5, 0 ,7,8,9};
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 7){
		int array[] =  {1, 2, 6, 2, 3, 0, 4, 0, 0, 6, 0, 0, 0, 1, 2, 3, 0, 5, 0, 5,
			0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0 ,7,8,9};
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 8){
		int array[] =  {1, 2, 6, 2, 3, 0, 4, 0, 0, 6, 0, 0, 0, 1, 2, 3, 0, 0, 6, 0,
			0, 0, 1, 3, 0, 5, 0, 0, 6, 0, 0, 0, 1, 2, 3, 1, 2, 6, 2, 3, 0, 5, 0, 4, 0 ,7,8,9};
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 9){
		int array[] =  { 1, 2, 6, 2, 3, 0, 4, 0, 0, 6, 0, 0, 0, 1, 2, 3, 0, 0, 6, 0,
			0, 0, 1, 3, 0, 5, 0, 0, 6, 0, 0, 0, 1, 2, 3,0, 1, 2, 6, 2, 3, 0, 5, 0, 4, 0,7,8,9 };
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}else if(level == 10){
		int array[] = {1,3, 0,7,8,9};
		for(int i = 0;i < sizeof(array)/sizeof(array[0]);i++){
			mapData.push_back(array[i]);
		}
	}
}

void Map::initMap(int level, bool end){
	float x = 0;
	float y = 0;

		for(int i = 0;i <mapData.size();i++){
			if(mapData.at(i)!=0){    //陆地
				countDistance = true;  //第一个不是0的位置起开始计算总长
				char buf[30];
				sprintf(buf,"land%d/land%d.png",level,mapData[i]);
				CCSprite* land = CCSprite::create(buf);
				land->setAnchorPoint(ccp(0,0));
				if(mapData.at(i)==6){   //顶部陆地
					y = 480 - land->getContentSize().height; 
					land->setPosition(ccp(x,y));

				}else{  
					CCSprite* startLand = NULL;
					if(map->count() >= 1){
						startLand = (CCSprite*)map->objectAtIndex(0);
					}
					

					if(mapData.at(i)==7){
						y = -(land->getContentSize().height - startLand->getContentSize().height + 55);
						land->setTag(7);
					}else if(mapData.at(i)==8){
						x-=195;
						y = -(land->getContentSize().height - startLand->getContentSize().height + 30 + 50);
						land->setTag(8);
					}else if(mapData.at(i) == 9){
						x-=135;
						y = -(land->getContentSize().height - startLand->getContentSize().height+30+35 + 115);
						land->setTag(9);

						if(end){
							CCLog("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
							CCSprite* end = CCSprite::createWithSpriteFrameName("end.png");
							end->setTag(10);
							end->setPosition(ccp(x + end->getContentSize().width + 60, y + 140));
							end->setAnchorPoint(ccp(0,0));
							map->addObject(end);

						}
					}else{
						y = -55;
					}
					land->setPosition(ccp(x,y));
					x += land->getContentSize().width;
					distance += land->getContentSize().width;
				}
				map->addObject(land);
			}else{   //空地
				x += 140;
				if(countDistance){
					distance += 140;
				}
			}
		}
		

}

void Map::addMap(GameScene* layer){
	if(map != NULL){
		for(int i = 0;i < map->count();i++){
			int zOrder = 10;
			CCSprite* temp = (CCSprite*)map->objectAtIndex(i);
			if(temp->getTag() == 10){
					zOrder = 11;
			}else{
				zOrder = 10;
			}
			layer->addChild(temp,zOrder);
		}
	}

}

bool Map::onLand(CCSprite* hero){
	bool onLand = false;
	for(int i = 0;i < map->count();i++){
		CCSprite* temp = (CCSprite*)map->objectAtIndex(i);
		if(temp->getPosition().x < hero->getPositionX() + hero->getContentSize().width / 2){
			if(CCRectMake(temp->getPositionX(),temp->getPositionY(),temp->getContentSize().width,temp->getContentSize().height).
				intersectsRect(CCRectMake(hero->getPositionX(),hero->getPositionY() - hero->getContentSize().height / 2,
				hero->getContentSize().width / 2,hero->getContentSize().height))){
					onLand = true;
			}
		}
	}
	return onLand;
}

bool Map::weaponOn(Role* hero){
	bool shootOn = false;
	if(hero->getState() == Role::WAIT ){
		for(int i = 0; i < map->count();i++){
			CCSprite* temp = (CCSprite*)map->objectAtIndex(i);
			if(temp->getPositionY() > 240){
				if(hero->getWeaponRange().intersectsRect(CCRectMake(temp->getPositionX(),temp->getPositionY(),
					temp->getContentSize().width,temp->getContentSize().height))){
						shootOn = true;
						break;
				}
			}
		}
	} 
	return shootOn;
}

void Map::mapMove(GameScene* parent,Role* role){
	for(int i = 0;i < map->count();i++){
		CCSprite* m = (CCSprite*)map->objectAtIndex(i);

		if(i == 0){ //第二大关卡开始，结束玩家飞行状态
			if(role->getState() == Role::FLY){
				if(m->getPositionX() > 0 && m->getTag() < 7 && m->getPositionX() <= Role::POSX){
					role->nextLevel();
					speed += 0.5;  //每一关增加0.5速 度
					parent->addSpeed(0.5);
					startCur = true;
				}
			}
		}

		if(m->getPositionX() > 854){
			m->setVisible(false);
		}
		if(m->getPositionX()+m->getContentSize().width>= 0){
			float x = m->getPositionX();
			float y = m->getPositionY();
			if(mapUp){
				if(m->getTag() >= 7 ){
					switch(m->getTag()){
					case 7:
						if(y<-55){
							y+=getSpeed() / 1.5f;
						}
						break;
					case 8:
						if(y < -75){
							y+=getSpeed() / 1.5f;
						}
						break;
					case 9:
						if(y < -70){
							y+=getSpeed() / 1.5f;
						}
						break;
					case 10:
						if(y < 70){
							y += getSpeed() / 1.5f;
						}
						break;
					}
				}else{
					if( y < 0){
						y += getSpeed();
					}
				}
				x -= getSpeed() / 1.5f;
			}else{
				x -= getSpeed();
			}
			//一关结束的地图向上移动
			if(!mapUp && i >= map->count() - 3){
				if(m->getPositionX() + 100 < 200){
					mapUp = true;
					role->setProtect(true);
				}
			}

			m->setPosition(ccp(x,y));
			if(m->getPositionX() < 854 && m->getPositionX() + m->getContentSize().width > 0){
				m->setVisible(true);
				if(startCur && m->getTag() == 9 && m->getPositionX()< role->getPositionX() - role->getContentSize().width){
					role->fly(true);
					startCur = false;
				}
			}
		}else{
		}
	}

	CCSprite* last = (CCSprite*)map->lastObject();
	if(last->getPositionX() + last->getContentSize().width <= 0 ){
		for(int i = 0;i < map->count();i++){
			parent->removeChild((CCSprite*)map->objectAtIndex(i),true);
		}
		map->removeAllObjects();
		
		if(GameData::getLevel() == 1 && curLevel >= 2){
			CCLog("到达这里可以结束一关");
			if(GameData::isPay()){
				GameData::setMax(2);
				parent->gameOver();
			}else{
				parent->gameOver();
				GameData::getInstance()->callPay(0);
			}
		}else{
			CCLog("现在的地图等级是%d， 关卡等级是%d", GameData::getLevel(), curLevel);
			bool end = false;
			if(GameData::getLevel() == 1 && curLevel >= 1){
				end = true;
			}
			resetMap(curLevel + 1,parent, end);
		}

	}

	if(startCur){
		parent->setProgress(getPercent());
		curDis += getSpeed();
	}
}