#include "CVertexShader.h"

CVertexShader::CVertexShader()
{
#ifdef D3D11
	m_pVSBlob = NULL;
	m_pVertexShader = NULL;
	m_pInputLayout = NULL;
#endif // D3D11
}