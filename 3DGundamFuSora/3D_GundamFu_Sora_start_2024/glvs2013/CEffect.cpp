#include "CEffect.h"

void CEffect::Update() {
	if (!m_alive)
		return;
	m_alpha -= ALPHA_SPEED;
	if (m_alpha < 0.0f) {
		m_alpha = 0.0f;
		m_alive = false;
	}
	std::vector<CMaterial*>::iterator it;
	std::vector<CMaterial*> materialList = getMaterialList();
	for (it = materialList.begin(); it != materialList.end(); it++) {
		(*it)->faceColor.a = m_alpha;
	}

	m_size += R_SPEED;

	setScale(m_size, m_size, m_size);
}

void CEffect::Render() {
	if (m_alive)
		CModelX::Render();
}

void CEffect::Start() {
	m_alive = true;
	m_alpha = 0.5f;
	m_size = 0.5;
	setScale(m_size, m_size, m_size);
}

void CEffect::Init() {
	Load("effect.x");
	m_alive = false;
}

void CEffect::SetColor(float r, float g, float b) {
	std::vector<CMaterial*>::iterator it;
	std::vector<CMaterial*> materialList = getMaterialList();
	for (it = materialList.begin(); it != materialList.end(); it++) {
		(*it)->faceColor.r = r;
		(*it)->faceColor.g = g;
		(*it)->faceColor.b = b;
		(*it)->faceColor.a = m_alpha;
	}
}

