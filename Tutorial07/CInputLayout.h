#pragma once
#include "Define.h"
class CInputLayout
{
public:

#ifdef D3D11
	ID3D11InputLayout * m_pInputLayout;
#endif // D3D11
};