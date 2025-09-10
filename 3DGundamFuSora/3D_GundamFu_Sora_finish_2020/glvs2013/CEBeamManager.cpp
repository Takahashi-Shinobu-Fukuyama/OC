#include "CEBeamManager.h"

CEBeamManager* CEBeamManager::inst = NULL;

CEBeamManager* CEBeamManager::GetInstance() {
	if (inst == NULL) {
		inst = new CEBeamManager();
		inst->array.clear();
	}
	return inst;
}

CBeam* CEBeamManager::Collision(CGameModelX* gm) {
//	std::vector<CBeam*>::iterator itr;
	std::list<CBeam*>::iterator itr;
	for (itr = array.begin(); itr != array.end(); itr++) {
		if (gm->Collision(*itr))
			return *itr;
	}
	return NULL;
}
