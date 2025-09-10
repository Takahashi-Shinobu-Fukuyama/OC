#pragma once

struct CXSVector3{
	float x;
	float y;
	float z;
};
struct CXSTexCoord{
	float u;
	float v;
};
struct CXSColorRGB {
	float r;
	float g;
	float b;
};
struct CXSColorRGBA {
	float r;
	float g;
	float b;
	float a;
};
struct CXSMaterial{
	CXSColorRGBA faceColor;
	float power;
	CXSColorRGB specularColor;
	CXSColorRGB emissiveColor;
	char filePath[64];
 
};
class CXSModelLoader {
public:
	CXSVector3	*m_vertex;
	int			*m_vertexIndex;
	int			m_vertexNum;
	int			m_faceNum;
	CXSVector3	*m_normal;
	int			m_normalNum;
	int			*m_normalIndex;
	CXSTexCoord *m_texCoord;
	CXSMaterial *m_material;
	int			*m_materialList;
	int			m_materialNum;
	
	CXSModelLoader(const char* filePath);
	~CXSModelLoader();

};