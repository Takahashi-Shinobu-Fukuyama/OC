#include "CUIBar.h"
#include "CCamera.h"

CUIBar::CUIBar() : m_vPos(0,0),m_vCenter(0,0),m_vSize(0,0),srcRect(0,0,0,0),m_ang(0),m_flipH(false),m_color(1.0f,1.0f,1.0f,1.0f){
}

CUIBar::~CUIBar(){
}

bool CUIBar::Load(char* path){
	m_pTexture = new CTexture();
	return m_pTexture->Load(path);

}

void CUIBar::Release() {
	delete m_pTexture;
}

void CUIBar::Draw() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//スクリーン左上を原点(0,0)にした正射投影の行列を作成
	CMatrix	mProj = CMatrix(
		2.0f/CCamera::getCurrent()->getWhidth() , 0.0f, 0.0f, -1.0f,
         0.0f, -2.0f/CCamera::getCurrent()->getHeight() , 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
     );
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadMatrixf(mProj.f);	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			CMatrix mT,mR;
			mT.Transelate(m_vPos.x,m_vPos.y,0.0f);
			
			mR.RotationZ(m_ang);

			glLoadMatrixf((mT * mR).f);

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (float*)&m_color);
//			m_pTexture->MapTexture();

			//画像を張り付けるためのポリゴンの作成
			struct SVertex {
			//	float tu;
			//	float tv;
				float vx;
				float vy;
				float vz;
			};
			/*
			SVertex vertices[] = {
				{ srcRect.m_left / m_pTexture->m_width, srcRect.m_top / m_pTexture->m_height, 0 - m_vCenter.x, 0 - m_vCenter.y, 0.0f },		//左上
				{ srcRect.m_right / m_pTexture->m_width, srcRect.m_top / m_pTexture->m_height, m_vSize.x - m_vCenter.x, 0 - m_vCenter.y, 0.0f },		//右上
				{ srcRect.m_left / m_pTexture->m_width, srcRect.m_bottom / m_pTexture->m_height, 0 - m_vCenter.x, m_vSize.y - m_vCenter.y, 0.0f },		//左下
				{ srcRect.m_right / m_pTexture->m_width, srcRect.m_bottom / m_pTexture->m_height, m_vSize.x - m_vCenter.x, m_vSize.y - m_vCenter.y, 0.0f }		//右下
			};
			*/
			SVertex vertices[] = {
				{  0 - m_vCenter.x, 0 - m_vCenter.y, 0.0f },		//左上
				{  m_vSize.x - m_vCenter.x, 0 - m_vCenter.y, 0.0f },		//右上
				{  0 - m_vCenter.x, m_vSize.y - m_vCenter.y, 0.0f },		//左下
				{  m_vSize.x - m_vCenter.x, m_vSize.y - m_vCenter.y, 0.0f }		//右下
			};
			if (m_flipH) {
			//	float t = vertices[0].tu;
			//	vertices[0].tu = vertices[2].tu = vertices[1].tu;
			//	vertices[1].tu = vertices[3].tu = t;
			}

//			glColor3f(1,1,1);
			glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);
			glInterleavedArrays(GL_V3F, 0, vertices);
			glDrawArrays(GL_TRIANGLE_STRIP,0,4);

//			m_pTexture->UnmapTexture();
			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	
}