#include "CModelObj.h"
#include <string.h>

CMaterialObj::CMaterialObj() : m_pTex(NULL){
}
void CMaterialObj::Map() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

	if(m_pTex) {
		m_pTex->MapTexture();
	}
}

void CMaterialObj::Unmap() {
	if(m_pTex) {
		m_pTex->UnmapTexture();
	}
}

void CMaterialObj::Release() {
	if(m_pTex) {m_pTex->Release();m_pTex=NULL;}
}

bool CModelObj::LoadMaterial(char *path) {
	char key[32];
	FILE* fp = fopen(path,"r");
	//�t�@�C�����J���Ȃ������ꍇ�̓G���[��\����false��Ԃ�
	if(fp==NULL) {
		printf("�t�@�C��������܂���%s\n",path);
		return false;
	}
	m_material.m_pTex = NULL;
	fscanf(fp, "%s", key);
	while (!feof(fp)) {
		if(strcmp(key,"Ka")==0) {
			fscanf(fp,"%f %f %f",&m_material.m_ambient[0],&m_material.m_ambient[1],&m_material.m_ambient[2]);
			m_material.m_ambient[3] = 1.0f;
		} else 
		if(strcmp(key,"Kd")==0) {
			fscanf(fp,"%f %f %f",&m_material.m_diffuse[0],&m_material.m_diffuse[1],&m_material.m_diffuse[2]);
			m_material.m_diffuse[3] = 1.0f;
		} else 
		if(strcmp(key,"Ks")==0) {
			fscanf(fp,"%f %f %f",&m_material.m_specular[0],&m_material.m_specular[1],&m_material.m_specular[2]);
			m_material.m_specular[3] = 1.0f;
		}else
		if(strcmp(key,"Ns")==0) {
			fscanf(fp,"%f",&m_material.m_shininess);
		} else 
		if(strcmp(key,"d")==0) {
			float aplha;
			fscanf(fp,"%f",&aplha);
			m_material.m_ambient[3] = aplha;
			m_material.m_diffuse[3] = aplha;
			m_material.m_specular[3] = aplha;
		} else
		if(strcmp(key,"map_Kd")==0) {
			char str[128];
			fscanf(fp,"%s\n",str);
			m_material.m_pTex = new CTexture();
			m_material.m_pTex->Load(str);
		}
		fscanf(fp, "%s", key);
	}
	return true;
}

CModelObj::CModelObj() : 
m_pVertexAry(NULL),m_pNormalAry(NULL),m_pTexCoordAry(NULL),m_vertexNum(0){
}
CModelObj::~CModelObj() {
	Release();
}
bool CModelObj::Load(const char *path) {
	//�t�@�C���|�C���^(fp)�̍쐬���J�����t�@�C���̃n���h����n���B
	FILE* fp = fopen(path,"r");
	//�t�@�C�����J���Ȃ������ꍇ�̓G���[��\����false��Ԃ�
	if(fp==NULL) {
		printf("�t�@�C��������܂���%s\n",path);
		return false;
	}
	//���W�f�[�^�p�J�E���^�[
	int	VCnt=0;
	//�@���f�[�^�p�J�E���^�[
	int	NCnt=0;
	//�e�N�X�`���[�R�[�h�p�J�E���^�[
	int	TCnt=0;
	//�ʃf�[�^�p�J�E���^�[
	int	FCnt=0;
	//�L�[���[�h�i�[�p
	char key[32];
	//�t�@�C���̖����łȂ���ΌJ��Ԃ�
	while(!feof(fp)) {
		//key���N���A
		key[0]=0;
		//1�P��@�������ǂݍ���
		fscanf(fp,"%s",key);
		//�ǂݍ��񂾃L�[���[�h���uv�v�Ȃ���W�f�[�^�J�E���^�[���J�E���g�A�b�v
		if(strcmp(key,"v")==0) {
			VCnt++;
		} else 
		//�ǂݍ��񂾃L�[���[�h���uf�v�Ȃ�ʃf�[�^�J�E���^�[���J�E���g�A�b�v
		if(strcmp(key,"f")==0) {
			FCnt++;
		} else 
		//�ǂݍ��񂾃L�[���[�h���uvn�v�Ȃ�@���f�[�^�J�E���^�[���J�E���g�A�b�v
		if(strcmp(key,"vn")==0) {
			NCnt++;
		} else 
		//�ǂݍ��񂾃L�[���[�h���uvt�v�Ȃ�e�N�X�`���[�R�[�h�f�[�^�J�E���^�[���J�E���g�A�b�v
		if(strcmp(key,"vt")==0) {
			TCnt++;
		} else 
		//�ǂݍ��񂾃L�[���[�h���umtllib�v�Ȃ�}�e���A���t�@�C���̓ǂݍ���
		if(strcmp(key,"mtllib")==0) {
			char str[32];
			fscanf(fp,"%s",str);
			LoadMaterial(str);
		}
	}
	printf("VCnt %d\n",VCnt);

	//���W�f�[�^�̐������z����쐬
	CVector3D *pVertex = new CVector3D[VCnt];
	//�@���f�[�^�̐������z����쐬
	CVector3D *pNormal = new CVector3D[NCnt];
	//�e�N�X�`���[�R�[�h�f�[�^�̐������z����쐬
	CVector2D *pTexCoord = new CVector2D[TCnt];
	//�`��p�̒��_�f�[�^�̒��_�����v�Z�@3*�ʂ̐�
	m_vertexNum = 3*FCnt;
	//�`��p���_�z����쐬
	m_pVertexAry = new CVector3D[m_vertexNum];
	//�`��p�@���z����쐬
	m_pNormalAry = new CVector3D[m_vertexNum];
	//�`��p�e�N�X�`���[�R�[�h�z����쐬
	m_pTexCoordAry = new CVector2D[m_vertexNum];
	//�ǂݍ��񂾍��W�f�[�^��0���珇�ԂɊi�[���邽�߁A�J�E���^�[��0�ɖ߂�
	VCnt=0;
	NCnt=0;
	TCnt=0;
	//�ǂݏ����̈ʒu���t�@�C���̐擪�ɖ߂�
	fseek(fp,0,SEEK_SET);
	//�`��p���_�z��ւ̍��W�f�[�^�i�[��ԍ�
	int idx = 0;
	//�t�@�C���̖����łȂ���ΌJ��Ԃ�
	while(!feof(fp)) {
		//key���N���A
		key[0]=0;
		//�L�[���[�h�̓ǂݍ���
		fscanf(fp,"%s",key);
		//�u���v��ǂݍ��񂾂�A���W�f�[�^��ǂݍ���
		if(strcmp(key,"v")==0) {
			fscanf(fp,"%f %f %f",&pVertex[VCnt].x,&pVertex[VCnt].y,&pVertex[VCnt].z);
			//�ǂݍ��ݐ�����̃f�[�^��
			VCnt++;
		}else//�uvn�v��ǂݍ��񂾂�A�@���f�[�^��ǂݍ���
		if(strcmp(key,"vn")==0) {
			fscanf(fp,"%f %f %f",&pNormal[NCnt].x,&pNormal[NCnt].y,&pNormal[NCnt].z);
			//�ǂݍ��ݐ�����̃f�[�^��
			NCnt++;
		}else//�uvt�v��ǂݍ��񂾂�A�e�N�X�`���[�R�[�h�f�[�^��ǂݍ���
		if(strcmp(key,"vt")==0) {
			fscanf(fp,"%f %f",&pTexCoord[TCnt].x,&pTexCoord[TCnt].y);
			pTexCoord[TCnt].y = 1.0f - pTexCoord[TCnt].y;
			//�ǂݍ��ݐ�����̃f�[�^��
			TCnt++;
		}else
		if(strcmp(key,"f")==0) {
			int v1,v2,v3;
			int n1,n2,n3;
			int t1,t2,t3;
			if (TCnt > 0) {
				fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
				m_pVertexAry[idx] = pVertex[v1 - 1];			//���_�P�̍��W�f�[�^
				m_pVertexAry[idx + 1] = pVertex[v2 - 1];		//���_�Q�̍��W�f�[�^
				m_pVertexAry[idx + 2] = pVertex[v3 - 1];

				m_pNormalAry[idx] = pNormal[n1 - 1];			//���_�P�̖@���f�[�^
				m_pNormalAry[idx + 1] = pNormal[n2 - 1];		//���_�Q�̖@���f�[�^
				m_pNormalAry[idx + 2] = pNormal[n3 - 1];

				m_pTexCoordAry[idx] = pTexCoord[t1 - 1];			//���_�P�̃e�N�X�`���[�R�[�h�f�[�^
				m_pTexCoordAry[idx + 1] = pTexCoord[t2 - 1];		//���_�Q�̃e�N�X�`���[�R�[�h�f�[�^
				m_pTexCoordAry[idx + 2] = pTexCoord[t3 - 1];
			}
			else {
				fscanf(fp, "%d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
				m_pVertexAry[idx] = pVertex[v1 - 1];			//���_�P�̍��W�f�[�^
				m_pVertexAry[idx + 1] = pVertex[v2 - 1];		//���_�Q�̍��W�f�[�^
				m_pVertexAry[idx + 2] = pVertex[v3 - 1];

				m_pNormalAry[idx] = pNormal[n1 - 1];			//���_�P�̖@���f�[�^
				m_pNormalAry[idx + 1] = pNormal[n2 - 1];		//���_�Q�̖@���f�[�^
				m_pNormalAry[idx + 2] = pNormal[n3 - 1];

			}
			
			//�f�[�^�̊i�[���3�ړ�����
			idx+=3;
		}
	}

	//�ꎞ�f�[�^���
	delete[] pVertex;
	delete[] pNormal;
	delete[] pTexCoord;
	return true;
}

void CModelObj::RenderM(CMatrix &m) {
//	UpdateMatrix();

	glPushMatrix();
//	glMultMatrixf(m_matrix.f);
	glMultMatrixf(m.f);

	m_material.Map();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3,GL_FLOAT,0,m_pVertexAry);
	glNormalPointer(GL_FLOAT,0,m_pNormalAry);

	if(m_pTexCoordAry != NULL) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,0,m_pTexCoordAry);
	}

	glDrawArrays(GL_TRIANGLES,0,m_vertexNum);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_material.Unmap();

	glPopMatrix();

}

void CModelObj::Release() {
	if(m_pVertexAry)  {delete[] m_pVertexAry; m_pVertexAry=NULL;}
	if(m_pNormalAry)  {delete[] m_pNormalAry; m_pNormalAry=NULL;}
	if(m_pTexCoordAry)  {delete[] m_pTexCoordAry; m_pTexCoordAry=NULL;}
}