#include "CParticleManager.h"

CParticleManager* CParticleManager::inst = NULL;

CParticleManager* CParticleManager::GetInstance() {
	if (inst == NULL) {
		inst = new CParticleManager();
		inst->array.clear();
	}
	return inst;
}
