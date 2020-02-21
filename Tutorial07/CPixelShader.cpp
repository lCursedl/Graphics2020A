#include "CPixelShader.h"

CPixelShader::CPixelShader()
{
#ifdef D3D11
	m_pPSBlob = NULL;
	m_pPixelShader = NULL;
#endif // D3D11
}