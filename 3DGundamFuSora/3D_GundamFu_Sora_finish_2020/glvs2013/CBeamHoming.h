#pragma once

#include "CBeam.h"
#include "CEnemy.h"

class CBeamHoming : public CBeam {
public:
	float m_df;	//C³ˆÚ“®—Ê
	CEnemy* m_pEnemy;	//’Ç‚¢‚©‚¯‚é“G
	CVector3D m_start_target_pos;
	CBeamHoming(CBeam::SBT s) : CBeam(s),m_df(0.25f),m_pEnemy(0) {};
	void Start(const CVector3D& pos, const CVector3D& target);
	void Update();
};