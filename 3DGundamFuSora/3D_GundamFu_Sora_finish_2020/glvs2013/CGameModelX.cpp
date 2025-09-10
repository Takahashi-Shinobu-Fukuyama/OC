#include "CGameModelX.h"
#include "CCollision.h"

bool CGameModelX::Collision(const CBeam* beam) {
	if (!beam->m_alive)
		return false;
	if (beam->m_trail > 0.0f)
		return false;
	return !CCollision::CollisionShpere(m_pos, 1.3f, beam->m_pos, beam->sx);
}

