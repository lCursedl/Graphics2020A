#include "CPixelShader.h"

CPixelShader::CPixelShader()
{
#ifdef D3D11
	m_pPSBlob = NULL;
	m_pPixelShader = NULL;
#endif // D3D11
}

void * CPixelShader::getPS()
{
#ifdef D3D11
	return m_pPixelShader;
#else
	return nullptr;
#endif // D3D11
}
