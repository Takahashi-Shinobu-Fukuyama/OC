#include "CImage2.h"
#include "CCamera.h"

void CImage2::Draw(float x, float y, float left, float top, float right, float bottom) {
	setRect(left,top,right,bottom);
	setSize(right - left,bottom - top);
	setPos(x,y);
	CImage::Draw();
}

void CImage2::DrawCenter(float left, float top, float right, float bottom) {
	setRect(left,top,right,bottom);
	setSize(right - left,bottom - top);
	setPos((CCamera::getCurrent()->getWhidth() - right + left) / 2,
		(CCamera::getCurrent()->getHeight() - bottom + top) / 2);
	CImage::Draw();
}
