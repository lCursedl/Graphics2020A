#include "CSwapChain.h"

CSwapChain::CSwapChain(){}


void CSwapChain::init(SwapChainStruct s)
{
#ifdef D3D11
	ZeroMemory(&m_sd, sizeof(m_sd));
	m_sd.BufferCount = s.bufferCount;
	m_sd.BufferDesc.Width = s.W;
	m_sd.BufferDesc.Height = s.H;
	m_sd.BufferDesc.Format = (DXGI_FORMAT)s.format;
	m_sd.BufferDesc.RefreshRate.Numerator = s.refreshNumerator;
	m_sd.BufferDesc.RefreshRate.Denominator = s.refreshDenominator;
	m_sd.BufferUsage = s.bufferUsage;
	m_sd.OutputWindow = s.outputWND;
	m_sd.SampleDesc.Count = s.count;
	m_sd.SampleDesc.Quality = s.quality;
	m_sd.Windowed = s.windowed;
#endif
}

CSwapChain * CSwapChain::getInstance()
{
	if (m_pSCInstance == nullptr)
	{
		m_pSCInstance = new CSwapChain();
	}
	return m_pSCInstance;
}