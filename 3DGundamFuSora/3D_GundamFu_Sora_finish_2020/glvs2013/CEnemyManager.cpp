#include "CEnemyManager.h"

CEnemyManager* CEnemyManager::inst = NULL;

CEnemyManager* CEnemyManager::GetInstance() {
	if (inst == NULL) {
		inst = new CEnemyManager();
		inst->array.clear();
	}
	return inst;
}

//std::vector<CEnemy*>& CEnemyManager::GetArray() {
std::list<CEnemy*>& CEnemyManager::GetArray() {
		return array;
}