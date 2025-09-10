#pragma once
#include "CImage.h"

class CImage2 : public CImage {
public:
	void Draw(float x, float y, float left, float top, float right, float bottom); 
	void DrawCenter(float left, float top, float right, float bottom); 
	void Draw(float left, float top, float right, float bottom) {
		DrawCenter(left, top, right, bottom);
	};
};
