#pragma once

#include "Includes.h"
#include <string>

class CMaterial
{
public:

	CMaterial();
	~CMaterial();

	void shutdown();
	std::string			m_Normal;
	bool				m_HasNormal;
	bool				m_HasDiffuse;
	std::string			m_TextureDir;
	std::string			m_Diffuse;

#ifdef D3D11
	ID3D11ShaderResourceView*	m_TextureDiffuse;
	ID3D11ShaderResourceView*	m_TextureNormal;
#endif // D3D11	
};