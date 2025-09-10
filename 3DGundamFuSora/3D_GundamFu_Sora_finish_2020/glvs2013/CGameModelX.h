#pragma once
#include "CModelX.h"
#include "CBeam.h"

class CGameModelX : public CModelX {
public:
	bool Collision(const CBeam* beam);

};
