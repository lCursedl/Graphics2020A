#pragma once
#include "Define.h"

class CVertexShader
{
public:
	CVertexShader();
#ifdef D3D11
	ID3DBlob * m_pVSBlob;
	ID3D11VertexShader * m_pVertexShader;
	ID3D11InputLayout * m_pInputLayout;
#endif // D3D11
};