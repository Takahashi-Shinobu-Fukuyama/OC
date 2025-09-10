#include "CEffectManager.h"

CEffectManager* CEffectManager::inst = NULL;

CEffectManager* CEffectManager::GetInstance() {
	if (inst == NULL) {
		inst = new CEffectManager();
		inst->array.clear();
	}
	return inst;
}
