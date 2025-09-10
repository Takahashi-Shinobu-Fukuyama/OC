#pragma once
#include "CManager.h"
#include "CBeam.h"
#include "CEnemy.h"

#include <vector>

class CBeamManager : public CManager<CBeam*> {
private:
	static CBeamManager* inst;

public:
	static CBeamManager* GetInstance();
	CBeam* Collision(CEnemy* enemy);
};
