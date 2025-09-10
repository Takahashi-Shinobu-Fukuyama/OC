#include "CParticleSystem.h"
#include "CCamera.h"

CParticle::CParticle() :m_life(0), m_moveDirection(PARTICLE_MOVE_SPEED, 0.0f, 0.0f){
		//	m_billboard.Load("particle.obj");
}

void CParticle::Init() {
	float distance = (float)(rand() % 100) / 100.0 * m_rangeRadius;
	m_pos = CVector3D(1.0f, 0.0f, 0.0f);
	float xrad = (float)(rand() % 628) / 100.0f;
	float yrad = (float)(rand() % 628) / 100.0f;
//	float zrad = (float)(rand() % 628) / 100.0f;
	CMatrix roty, rotx, rotz;
	roty.SetIndentity();
	rotx.SetIndentity();
	roty.RotationY(yrad);
	rotx.RotationX(xrad);
//	rotz.SetIndentity();
//	rotz.RotationZ(zrad);
	m_moveDirection = rotx * roty * CVector3D(1.0f, 0.0f, 0.0f) * m_moveSpeed;
	m_pos = rotx * roty * m_pos * distance;
}

void CParticle::Update() {
	if (m_life < m_maxLife) {
		m_life++;
		m_pos += m_moveDirection;
	}
}

void CParticle::Render() {
	if (0 < m_life  && m_life < m_maxLife) {
		UpdateMatrix();
		m_pModelObj->RenderM(m_matrix);
//		CModelObj::Render();
	}
}


CParticleSystem::CParticleSystem() :m_pos(0.0f, 0.0f, 0.0f), m_particleNum(0), m_pParticleAry(NULL), m_loop(false), m_alive(false), m_pModelObj(NULL){
}

CParticleSystem::CParticleSystem(const CParticleSystem& obj){
	*this = obj;
	m_pParticleAry = new CParticle[m_particleNum];
	memcpy(m_pParticleAry, obj.m_pParticleAry, sizeof(CParticleSystem)*m_particleNum);
}

void CParticleSystem::Copy(const CParticleSystem &obj) {
	if (m_pParticleAry) {
		delete[] m_pParticleAry;
		m_pParticleAry = NULL;
	}
	Init(obj.m_pos, obj.m_particleNum, obj.m_pModelObj, obj.m_scale, obj.m_moveSpeed, obj.m_maxlife,obj.m_rangeRadius);
}


CParticleSystem::~CParticleSystem() {
	if (m_pParticleAry != NULL) {
		delete[] m_pParticleAry;
		m_pParticleAry = 0;
	}
	/*
	if (m_pVertexAry != NULL) {
		delete[] m_pVertexAry;
		m_pVertexAry = 0;
	}
	if (m_pNormalAry != NULL) {
		delete[] m_pNormalAry;
		m_pNormalAry = 0;
	}
	if (m_pTexCoordAry != NULL) {
		delete[] m_pTexCoordAry;
		m_pTexCoordAry = 0;
	}
	if (m_pZIndex != NULL) {
		delete[] m_pZIndex;
		m_pZIndex = 0;
	}
	*/
}

void CParticleSystem::Init(CVector3D particleSystemPosition, int particleNum, CModelObj* pModelObj, float scale, float moveSpeed, int maxlife) {
	Init(particleSystemPosition, particleNum, pModelObj, scale, moveSpeed, maxlife, 1.0f);
}

void CParticleSystem::Init(CVector3D particleSystemPosition, int particleNum, CModelObj* pModelObj, float scale, float moveSpeed, int maxlife, float rangeRadius) {
	m_rangeRadius = rangeRadius;
	m_pos = particleSystemPosition;
	m_particleNum = particleNum;
	m_maxlife = maxlife;
	m_scale = scale;
	m_moveSpeed = moveSpeed;
	m_pParticleAry = new CParticle[m_particleNum];
	m_pModelObj = pModelObj;
	m_pParticleAry[0].m_pModelObj = pModelObj;
	m_pParticleAry[0].m_pos = m_pos;
	m_pParticleAry[0].m_scale = CVector3D(scale, scale, scale);
	m_pParticleAry[0].m_moveSpeed = moveSpeed;
	m_pParticleAry[0].m_moveDirection.x = 1.0f;
	m_pParticleAry[0].m_rangeRadius = rangeRadius;
	m_pParticleAry[0].m_maxLife = maxlife;
	m_pParticleAry[0].m_life = 0;
//	m_pParticleAry[0].Init(m_radius);
//	m_pZIndex = new int[m_particleNum];
//	m_pZIndex[0] = 0;
//	m_vertexNum = m_pParticleAry[0].m_billboard.m_vertexNum;
	for (int i = 1; i < m_particleNum; i++) {
		m_pParticleAry[i] = m_pParticleAry[0];
//		m_pParticleAry[i].m_life = maxlife * i / particleNum;
//		m_pParticleAry[i].Init(m_radius);
//		m_vertexNum += m_pParticleAry[0].m_billboard.m_vertexNum;

//		m_pZIndex[i] = i;
	}
	/*
	m_pVertexAry = new CVector3D[m_vertexNum];
	m_pNormalAry = new CVector3D[m_vertexNum];
	m_pTexCoordAry = new CVector2D[m_vertexNum];
	*/
}

void CParticleSystem::Update() {
	CMatrix m;
	m.SetIndentity();
	Update(m);
}

void CParticleSystem::Update(CMatrix m) {
	if (!m_alive) return;
	/*
	m_billboardMatrix = GetBillboardRotationMatrix();

	//パーティクルシステムの場所（X,Y,Z座標）を計算する
	CMatrix matrix;
	CVector4D v4;
	v4.x = m_pos.x;
	v4.y = m_pos.y;
	v4.z = m_pos.z;
	v4.w = 1.0f;
	v4 = m * v4;
	*/

	int loopcnt = 0;
	for (int i = 0; i < m_particleNum; i++) {
		if ((m_loop && m_pParticleAry[i].m_life >= m_maxlife) || m_pParticleAry[i].m_life == 0) {
			m_pParticleAry[i].Init();
			m_pParticleAry[i].m_life = 1;
		}
		else {
			if (!m_loop && m_pParticleAry[i].m_life >= m_maxlife) loopcnt++;
			m_pParticleAry[i].Update();
		}

		//頂点、法線、UV座標をまとめる
//		num = m_pParticleAry[i].m_billboard.VertexCopy(v, n, t);
//		memcpy(&m_pVertexAry[vertex_num], v, sizeof(CVector3D) * 6);
//		memcpy(&m_pNormalAry[vertex_num], n, sizeof(CVector3D) * 6);
//		memcpy(&m_pTexCoordAry[vertex_num], t, sizeof(CVector2D) * 6);

//		num = m_pParticleAry[i].m_billboard.VertexCopy(&m_pVertexAry[vertex_num], &m_pNormalAry[vertex_num], &m_pTexCoordAry[vertex_num]);
//		vertex_num += num;
	}
	if (loopcnt >= m_particleNum && !m_loop) m_alive = false;
	/*
	for (int j = 0; j < m_particleNum - 1; j++) {
		for (int k = j + 1; k < m_particleNum; k++) {
			if (m_pParticleAry[m_pZIndex[j]].m_billboard.m_pos.z > m_pParticleAry[m_pZIndex[k]].m_billboard.m_pos.z) {
				int w = m_pZIndex[j];
				m_pZIndex[j] = m_pZIndex[k];
				m_pZIndex[k] = w;
			}
		}
	}
	*/
}

void CParticleSystem::Render() {
	CMatrix m;
	m.SetIndentity();
	Render(m);
}

void CParticleSystem::Render(CMatrix m) {
	if (!m_alive) return;
	//パーティクルシステムのワールド座標を計算する
	CMatrix matrix;
	CVector4D v4;
	v4.x = m_pos.x;
	v4.y = m_pos.y;
	v4.z = m_pos.z;
	v4.w = 1.0f;
	v4 = m * v4;
	CVector3D pos;
	pos.x = v4.x;
	pos.y = v4.y;
	pos.z = v4.z;

	//ビルボード用の行列を取得
	GetBillboardMatrix(&matrix, pos, CVector3D(1.0f, 1.0f, 1.0f));

	glPushMatrix();

	glMultMatrixf(matrix.f);

	//デプステストなし（Zバッファキャンセル）
	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	//各パーティクルの描画
	for (int i = 0; i < m_particleNum; i++) {
//		m_pParticleAry[m_pZIndex[i]].Render();
		m_pParticleAry[i].Render();
	}
	glDisable(GL_BLEND);
	//デプステストあり
	glDepthMask(GL_TRUE);

	/*
	m_pParticleAry[0].m_billboard.m_material.Map();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, m_pVertexAry);
	glNormalPointer(GL_FLOAT, 0, m_pNormalAry);

	if (m_pTexCoordAry != NULL) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, m_pTexCoordAry);
	}

	glDrawArrays(GL_TRIANGLES, 0, m_vertexNum);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_pParticleAry[0].m_billboard.m_material.Unmap();
	*/

	glPopMatrix();

}

//ビルボードの回転行列計算
CMatrix CParticleSystem::GetBillboardRotationMatrix() {
	//カメラのビュー行列取得
	CMatrix rotMtx = CCamera::getCamera()->getMatrix();
	//移動量をゼロにする
	rotMtx.m03 = rotMtx.m13 = rotMtx.m23 = 0.0f;
	//逆行列を取得
	return rotMtx.getInverse();
}

//ビルボードの回転行列計算
void CParticleSystem::GetBillboardMatrix(CMatrix *out, const CVector3D &pos, const CVector3D &scale) {
		//カメラのビュー行列取得
	CMatrix rotMtx = CCamera::getCamera()->getMatrix();
	//移動量をゼロにする
	rotMtx.m03 = rotMtx.m13 = rotMtx.m23 = 0.0f;
	//逆行列を取得
	rotMtx = rotMtx.getInverse();
	//合成行列を求める
	CMatrix transMtx;
	transMtx.Transelate(pos.x, pos.y, pos.z);
	CMatrix scaleMtx;
	scaleMtx.Scale(scale.x, scale.y, scale.z);
	*out = transMtx * rotMtx * scaleMtx;
}

void CParticleSystem::Start() {
	Start(false);
}
void CParticleSystem::Start(bool loop) {
	m_loop = loop;
	if (m_alive) return;
	m_alive = true;
	for (int i = 0; i < m_particleNum; i++) {
		m_pParticleAry[i].Init();
		m_pParticleAry[i].m_life = m_maxlife * i / m_particleNum;
	}
}