/*
*	2D画像表示クラス
*/
#pragma once

#include "CRect.h"
#include "CMatrix.h"
#include "CShader.h"
#include "CTexture.h"

class CUIBar{
private:

	CTexture* m_pTexture;	//テクスチャー
	CVector2D m_vPos;		//位置(pixel)
	CVector2D m_vCenter;	//中心位置(pixel)
	CVector2D m_vSize;		//サイズ(pixel)
	CRect srcRect;			//表示元矩形
	float m_ang;			//回転値
	bool m_flipH;			//左右反転
	CColorRGBA m_color;		//色
	

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
	*	ファイルから画像を読み込む
	*/
	bool Load(char* path);
	/*
	*	開放する
	*/
	void Release();
	/*
	*	表示位置の設定
	*/
	void setPos(float x,float y) {
		m_vPos.x = x; m_vPos.y = y;
	}
	/*
	*	中心位置の設定
	*/
	void setCenter(float x,float y) {
		m_vCenter.x = x; m_vCenter.y = y;
	}
	/*
	*	表示サイズを設定
	*/
	void setSize(float w,float h) {
		m_vSize.x = w; m_vSize.y = h;
	}
	/*
	*	表示元矩形を設定
	*/
	void setRect(int left,int top,int right,int bottom){
		srcRect.m_left = left;srcRect.m_right = right;
		srcRect.m_top = top;srcRect.m_bottom = bottom;
	}
	/*
	*	回転値を設定
	*/
	void setAng(float ang){
		m_ang = ang;
	}
	/*
	*	左右反転フラグ
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
	*	描画
	*/
	void Draw();

	float getPoint() {
		return m_vSize.x;
	}

	void  setPoint(float p) {
		m_vSize.x = p;
	}

};