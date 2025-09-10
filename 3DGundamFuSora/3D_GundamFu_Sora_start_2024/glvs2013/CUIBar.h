/*
*	2D�摜�\���N���X
*/
#pragma once

#include "CRect.h"
#include "CMatrix.h"
#include "CShader.h"
#include "CTexture.h"

class CUIBar{
private:

	CTexture* m_pTexture;	//�e�N�X�`���[
	CVector2D m_vPos;		//�ʒu(pixel)
	CVector2D m_vCenter;	//���S�ʒu(pixel)
	CVector2D m_vSize;		//�T�C�Y(pixel)
	CRect srcRect;			//�\������`
	float m_ang;			//��]�l
	bool m_flipH;			//���E���]
	CColorRGBA m_color;		//�F
	

public:
	CUIBar();
	~CUIBar();

	CUIBar(float x, float y, float w, float h, float r, float g, float b, float a):srcRect(0,0,0,0),m_ang(0),m_pTexture(0) {
		setPos(x, y);
		setSize(w, h);
		setColor(r, g, b, a);
		setCenter(0.0f, 0.0f);
	};

	/*
	*	�t�@�C������摜��ǂݍ���
	*/
	bool Load(char* path);
	/*
	*	�J������
	*/
	void Release();
	/*
	*	�\���ʒu�̐ݒ�
	*/
	void setPos(float x,float y) {
		m_vPos.x = x; m_vPos.y = y;
	}
	/*
	*	���S�ʒu�̐ݒ�
	*/
	void setCenter(float x,float y) {
		m_vCenter.x = x; m_vCenter.y = y;
	}
	/*
	*	�\���T�C�Y��ݒ�
	*/
	void setSize(float w,float h) {
		m_vSize.x = w; m_vSize.y = h;
	}
	/*
	*	�\������`��ݒ�
	*/
	void setRect(int left,int top,int right,int bottom){
		srcRect.m_left = left;srcRect.m_right = right;
		srcRect.m_top = top;srcRect.m_bottom = bottom;
	}
	/*
	*	��]�l��ݒ�
	*/
	void setAng(float ang){
		m_ang = ang;
	}
	/*
	*	���E���]�t���O
	*/
	void setFlipH(bool f) {
		m_flipH = f;
	}
	void setColor(float r, float g, float b,float a){
		m_color.r = r;
		m_color.g = g;
		m_color.b = b;
		m_color.a = a;
	}
	/*
	*	�`��
	*/
	void Draw();

	float getPoint() {
		return m_vSize.x;
	}

	void  setPoint(float p) {
		m_vSize.x = p;
	}

};