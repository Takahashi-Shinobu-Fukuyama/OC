#pragma once
#include "CManager.h"
#include "CEffect.h"

#include <vector>

class CEffectManager : public CManager<CEffect*> {
private:
	static CEffectManager* inst;

public:
	static CEffectManager* GetInstance();

};
