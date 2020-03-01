#pragma once
#include "Define.h"

class CPixelShader
{
public:
	CPixelShader();
	void * getPS();
#ifdef D3D11
	ID3DBlob * m_pPSBlob;
	ID3D11PixelShader * m_pPixelShader;
#endif // D3D11

};