#include "CSamplerState.h"

CSamplerState::CSamplerState()
{
#ifdef D3D11
	m_pSamplerLinear = NULL;
#endif
}

void CSamplerState::init(SAMPLER_DESC S)
{
#ifdef D3D11
	ZeroMemory(&m_Desc, sizeof(m_Desc));
	m_Desc.Filter = (D3D11_FILTER)S.filter;
	m_Desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)S.addresU;
	m_Desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)S.addresV;
	m_Desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)S.addresW;
	m_Desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)S.comparisonFunc;
	m_Desc.MinLOD = S.minLOD;
	m_Desc.MaxLOD = S.maxLOD;
#endif
}

void * CSamplerState::getSampler()
{
#ifdef D3D11
	return m_pSamplerLinear;
#else
	return nullptr;
#endif
}
