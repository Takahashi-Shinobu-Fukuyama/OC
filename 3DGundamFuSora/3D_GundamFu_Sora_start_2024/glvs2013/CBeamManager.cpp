#include "CBeamManager.h"

CBeamManager* CBeamManager::inst = NULL;

CBeamManager* CBeamManager::GetInstance() {
	if (inst == NULL) {
		inst = new CBeamManager();
		inst->array.clear();
	}
	return inst;
}

CBeam* CBeamManager::Collision(CEnemy* enemy) {
//	std::vector<CBeam*>::iterator itr;
	std::list<CBeam*>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		if (enemy->Collision(*itr))
			return *itr;
	}
	return NULL;
}
