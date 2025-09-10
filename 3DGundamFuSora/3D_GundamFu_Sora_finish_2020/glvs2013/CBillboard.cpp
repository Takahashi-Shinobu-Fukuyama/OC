#include "CBillboard.h"
#include "CCamera.h"

//void CBillboard::GetBillboardMatrix(CMatrix *out, const CVector3D &pos, const CVector3D &scale);

void CBillboard::Update() {
	/*
//�r���{�[�h�̉�]�s��v�Z
	//�J�����̃r���[�s��擾
	m_rotMtx = CCamera::getCamera()->getMatrix();
	//�ړ��ʂ��[���ɂ���
	m_rotMtx.m03 = m_rotMtx.m13 = m_rotMtx.m23 = 0.0f;
	//�t�s����擾
	m_rotMtx = m_rotMtx.getInverse();
//�s������߂�
	m_transMtx.Transelate(m_pos.x, m_pos.y, m_pos.z);
	m_scaleMtx.Scale(m_scale.x, m_scale.y, m_scale.z);
	m_matrix = m_transMtx * m_rotMtx * m_scaleMtx;
	*/
	//GetBillboardMatrix(&m_matrix, m_pos, m_scale);
	UpdateMatrix();
}

void CBillboard::GetBillboardMatrix(CMatrix *out, const CVector3D &pos, const CVector3D &scale) {
	//�r���{�[�h�̉�]�s��v�Z
	//�J�����̃r���[�s��擾
	CMatrix rotMtx = CCamera::getCamera()->getMatrix();
	//�ړ��ʂ��[���ɂ���
	rotMtx.m03 = rotMtx.m13 = rotMtx.m23 = 0.0f;
	//�t�s����擾
	rotMtx = rotMtx.getInverse();
	//�s������߂�
	CMatrix transMtx;
	transMtx.Transelate(pos.x, pos.y, pos.z);
	CMatrix scaleMtx;
	scaleMtx.Scale(scale.x, scale.y, scale.z);
	*out = transMtx * rotMtx * scaleMtx;
}

void CBillboard::Render() {
	RenderM(m_matrix);
}
/*
int CBillboard::VertexCopy(CVector3D vertex[], CVector3D* normal, CVector2D* texcoord) {
	for (int i = 0; i < m_vertexNum; i++) {
		CVector4D vec;
//		vec = m_matrix * m_pVertexAry[i];
		vec.x = m_pVertexAry[i].x;
		vec.y = m_pVertexAry[i].y;
		vec.z = m_pVertexAry[i].z;
		vec.w = 1.0f;
		vec = vec * m_matrix;
		vertex[i].x = vec.x;
		vertex[i].y = vec.y;
		vertex[i].z = vec.z;
//		vec = m_pNormalAry[i] * m_matrix;
		vec.x = m_pNormalAry[i].x;
		vec.y = m_pNormalAry[i].y;
		vec.z = m_pNormalAry[i].z;
		vec.w = 1.0f;
		vec = vec * m_matrix;
		normal[i].x = vec.x;
		normal[i].y = vec.y;
		normal[i].z = vec.z;
		texcoord[i] = m_pTexCoordAry[i];
	}
	return m_vertexNum;
}
*/