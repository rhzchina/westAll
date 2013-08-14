                                                                                                         #include "GameData.h"

GameData* GameData::instance = NULL;

GameData::GameData(void)
{
	level = 1;
	max = 1;
	gold = 0;
	score = 0;
	distance = 0;
	best = 0;
	loopCount = 0;
	//memset(itemState,0,sizeof(itemState));
	for(int i = 0;i < 4;i++){
		data[i] = vector<int>();
		for(int j = 0;j < 2;j++){
			data[i].push_back(0);  //初始化，index = 0为当前持有，index = 1,值为0，是默认拥有第一个
		}
	}
}

GameData::~GameData(void)
{
}

GameData* GameData::getInstance(){
	if(instance == NULL){
		instance = new GameData();
	}
	return instance;
}

void GameData::addLevel(){
	instance->level++;
	if(instance->level > 4){
		instance->level = 1;
	}
}

void GameData::setLevel(int l){
	instance->level = l;
}

int GameData::getLevel(){
	return instance->level;
}

void GameData::reset(bool all){
	if(all){
		instance->best = 0;
		instance->score = 0;
	}else{
		instance->best = instance->best > instance->distance ? instance->best : instance->distance;		
	}
	instance->loopCount = 0;

	instance->distance = 0;
	
}
                                                                                                                                                                                                                                                                                                                                                                                                              
bool GameData::bought(int type,int id){
	for(int i = 1;i < instance->data[type].size();i++){
		if(instance->data[type].at(i) == id){
			return true;
		}
	}
	return false;
}


void GameData::addDistance(int d){
	instance->distance += d;
	if(instance->distance > 5500){
		instance ->max = 4;
	}else if(instance->distance > 4000){
		instance ->max = 3;
	}else if(instance->distance > 2700){
		instance ->max = 2;
	}
}