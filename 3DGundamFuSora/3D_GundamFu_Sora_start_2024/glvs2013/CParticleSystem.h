#pragma once

//#include "CBillboard.h"
#include "CModelObj.h"

#define PARTICLE_MOVE_SPEED 0.00125f
//#define PARTICLE_MOVE_SPEED 0.0f
#define PARTICLE_NUM 40
#define PARTICLE_SCALE 0.05f
#define PARTICLE_MAXLIFE 30

//class CParticle {
class CParticle : public CModelObj {
public:
//	CBillboard m_billboard;
	CModelObj* m_pModelObj;
	CVector3D m_moveDirection;
	float m_moveSpeed;
	float m_rangeRadius;
	int m_maxLife;
	int m_life;
	CParticle();

	void Init();
	void Update();
	void Render();
};

class CParticleSystem {
public:
	//�p�[�e�B�N���̔����ʒu
	CVector3D m_pos;
	//�p�[�e�B�N���̔�����
	int m_particleNum;
	//�p�[�e�B�N���̎���
	int m_maxlife;
	float m_scale;
	float m_moveSpeed;
	//���[�v�@true:���[�v����@false�F���[�v���Ȃ�(default)
	bool m_loop;
	//�p�[�e�B�N���L�� true:�L���@false:����
	bool m_alive;

	//�p�[�e�B�N����z�u����͈͂̔��a
	float m_rangeRadius;

	CModelObj* m_pModelObj; //�R�s�[�ŃG���[

	//�p�[�e�B�N���̔z��
	CParticle* m_pParticleAry;

//	int* m_pZIndex;

//	int m_vertexNum;
//	CVector3D* m_pVertexAry;
//	CVector3D* m_pNormalAry;
//	CVector2D* m_pTexCoordAry;

	CParticleSystem();
	CParticleSystem(const CParticleSystem& obj);
	~CParticleSystem();
	void Init(CVector3D particleSystemPosition, int particleNum, CModelObj* pModelObj, float scale, float moveSpeed, int maxlife);
	void Init(CVector3D particleSystemPosition, int particleNum, CModelObj* pModelObj, float scale, float moveSpeed, int maxlife, float rangeRadius);
	void Update();
	void Update(CMatrix);
	void Render();
	void Render(CMatrix);
	CMatrix CParticleSystem::GetBillboardRotationMatrix();
	void CParticleSystem::GetBillboardMatrix(CMatrix *out, const CVector3D &pos, const CVector3D &scale);

	void SetLoop(bool b) { m_loop = b; }
	void SetEnable(bool b) { m_alive = b; }
	void Start();
	void Start(bool loop);
	void Copy(const CParticleSystem &obj);

};
