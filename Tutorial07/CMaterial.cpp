#include "CMaterial.h"

CMaterial::CMaterial()
{
#ifdef D3D11
	m_TextureDiffuse = nullptr;
	m_TextureNormal = nullptr;
	m_TextureSpecular = nullptr;
#endif // D3D11	
}

CMaterial::~CMaterial(){}

void CMaterial::shutdown()
{
#ifdef D3D11
	if (m_TextureDiffuse)
	{
		m_TextureDiffuse->Release();
	}
	if (m_TextureNormal)
	{
		m_TextureNormal->Release();
	}
	if (m_TextureSpecular)
	{
		m_TextureSpecular->Release();
	}
#endif // D3D11
}