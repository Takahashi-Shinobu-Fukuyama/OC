#ifndef OBJ_H
#define OBJ_H

#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include "glut.h"

#include "CTexture.h"
#include "CVector3.h"
#include "CMatrix44.h"
#include "CTriangle.h"
#include <vector>

#include "CMaterial.h"
#include "CVertex.h"


class CModelObj {
private:
	std::vector<CTriangle> mTriangle; //�O�p�`
	std::vector<CMaterial*> mMaterials;	//�}�e���A��
	CMatrix44	mMatrix;//�s��
	CVertex		*mpVertex;	//���_�z��
	int			m_vertexNum;//���_��
	bool LoadMaterial(char *path);
	bool LoadMaterial(char *path, char *newmtl);
	//�}�e���A�����̖ʐ�
	std::vector<int> mMaterialVertexCount;

public:
	CModelObj();
	~CModelObj();

	/*�t�@�C���ǂݍ���*/
	bool	Load(const char *path);
	/*�`�揈��*/
	void	Render();
	/*�e�N�X�`���J��*/
	void	Release();
	//����̃I�[�o�[���C�h
	CModelObj& operator=(const CModelObj &c) {
		m_vertexNum = c.m_vertexNum;

		if (mpVertex) {
			delete[] mpVertex;
		}
		//�`��p���_�z����쐬
		mpVertex = new CVertex[c.m_vertexNum];
		memcpy(mpVertex, c.mpVertex, sizeof(CVertex) * c.m_vertexNum);

		return (*this);
	}

	/*�O�p�`�����蔻��ǉ�*/
	void CModelObj::AddCollisionManager();
};

#endif