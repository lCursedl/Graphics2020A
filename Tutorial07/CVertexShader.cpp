#include "CVertexShader.h"

CVertexShader::CVertexShader()
{
#ifdef D3D11
	m_pVSBlob = NULL;
	m_pVertexShader = NULL;
	m_pInputLayout = NULL;
#endif // D3D11
}

void * CVertexShader::getBlob()
{
#ifdef D3D11
	return m_pVSBlob;
#else
	return nullptr;
#endif
}

void * CVertexShader::getVS()
{
#ifdef D3D11
	return m_pVertexShader;
#else
	return nullptr;
#endif
}

void * CVertexShader::getInputLayout()
{
#ifdef D3D11
	return m_pInputLayout;
#else
	return nullptr;
#endif
}
