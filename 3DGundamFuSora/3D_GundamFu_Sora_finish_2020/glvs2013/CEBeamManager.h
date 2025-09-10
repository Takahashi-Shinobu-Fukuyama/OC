#pragma once
#include "CManager.h"
#include "CBeam.h"
#include "CGameModelX.h"

#include <vector>

class CEBeamManager : public CManager<CBeam*> {
private:
	static CEBeamManager* inst;

public:
	static CEBeamManager* GetInstance();
	CBeam* Collision(CGameModelX* enemy);
};
