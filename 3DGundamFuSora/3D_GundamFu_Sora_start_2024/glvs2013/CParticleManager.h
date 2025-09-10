#pragma once
#include "CManager.h"
#include "CParticleSystem.h"

#include <vector>

class CParticleManager : public CManager<CParticleSystem*> {
private:
	static CParticleManager* inst;

public:
	static CParticleManager* GetInstance();

};
