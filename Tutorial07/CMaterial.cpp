#include "CMaterial.h"

CMaterial::CMaterial()
{
#ifdef D3D11
	m_TextureDiffuse = NULL;
	m_TextureNormal = NULL;
#endif // D3D11	
}

CMaterial::~CMaterial()
{

}

void CMaterial::shutdown()
{
#ifdef D3D11
	if (m_TextureDiffuse)
	{
		m_TextureDiffuse->Release();
	}
#endif // D3D11
}
