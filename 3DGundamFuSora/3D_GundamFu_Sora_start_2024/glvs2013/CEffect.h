#pragma once
#include "CModelX.h"

#define ALPHA_SPEED 0.025f
#define R_SPEED 0.05f

class CEffect : public CModelX
{
public:
	bool m_alive;
	float m_alpha;
	float m_size;
	CEffect() :m_alive(false),m_alpha(1.0f),m_size(0.0f) {}

	void SetColor(float r, float g, float b);

	void Start();
	void Init();

	void Update();
	void Render();
};