#pragma once
#include "Define.h"

struct SAMPLER_DESC
{
	FILTER filter;
	TEXTURE_ADDRESS_MODE addresU;
	TEXTURE_ADDRESS_MODE addresV;
	TEXTURE_ADDRESS_MODE addresW;
	float mipLODBias;
	unsigned int maxAnisotropy;
	COMPARISON_FUNC comparisonFunc;
	float borderColor[4];
	float minLOD;
	float maxLOD;
};

class CSamplerState
{
public:
	CSamplerState();
	void init(SAMPLER_DESC S);
	void * getSampler();
#ifdef D3D11
	ID3D11SamplerState * m_pSamplerLinear;
	D3D11_SAMPLER_DESC m_Desc;
#endif // D3D11
};