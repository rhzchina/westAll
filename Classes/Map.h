#ifndef MAP_H
#define MAP_H
#include "cocos2d.h"
#include <vector>
#include "Role.h"
using namespace std;
USING_NS_CC;
class GameScene;

class Map
{
public:
	Map(int,GameScene*);
	~Map(void);
	void initMap(int, bool = false);
	void addMap(GameScene*);
	void mapMove(GameScene*,Role* );
	bool onLand(CCSprite*);
	bool weaponOn(Role*);
	int getSpeed(){return speed + speedChange;}
	void tempChange(float change){speedChange = change;}
	void clearChange(){speedChange = 0;}
	void createData(int);
	void resetMap(int,GameScene*, bool = false);
	int getLevel(){return curLevel;}
	float getPercent(){return curDis / distance > 0.95 ? 0.95 : curDis / distance;}
private:
	CCArray* map;
	vector<int> mapData;
	bool mapUp;
	float speed; 
	float speedChange;
	int curLevel;
	float distance;  //地图总长
	float curDis;   //当前
	bool countDistance;  //开始计算总长
	bool startCur;  //开始计算当前
};

#endif