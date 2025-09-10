#pragma once

class CRect {
public:
	float m_left;
	float m_top;
	float m_right;
	float m_bottom;
	CRect(float l,float t,float r,float b){
		m_left = l;
		m_top = t;
		m_right = r;
		m_bottom = b;
	}
};