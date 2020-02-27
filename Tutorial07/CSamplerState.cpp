#include "CSamplerState.h"

CSamplerState::CSamplerState()
{
	m_pSamplerLinear = NULL;
}

void CSamplerState::init(SAMPLER_DESC S)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
	m_Desc.Filter = (D3D11_FILTER)S.filter;
	m_Desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)S.addresU;
	m_Desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)S.addresV;
	m_Desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)S.addresW;
	m_Desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)S.comparisonFunc;
	m_Desc.MinLOD = S.minLOD;
	m_Desc.MaxLOD = S.maxLOD;
}