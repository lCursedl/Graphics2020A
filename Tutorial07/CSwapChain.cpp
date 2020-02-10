#include "CSwapChain.h"

CSwapChain::CSwapChain(){}


void CSwapChain::init(SwapChainStruct s)
{
	m_struct = s;

	ZeroMemory(&m_sd, sizeof(m_sd));
	m_sd.BufferCount = m_struct.bufferCount;
	m_sd.BufferDesc.Width = m_struct.W;
	m_sd.BufferDesc.Height = m_struct.H;
	m_sd.BufferDesc.Format = m_struct.format;
	m_sd.BufferDesc.RefreshRate.Numerator = m_struct.refreshNumerator;
	m_sd.BufferDesc.RefreshRate.Denominator = m_struct.refreshDenominator;
	m_sd.BufferUsage = m_struct.bufferUsage;
	m_sd.OutputWindow = m_struct.outputWND;
	m_sd.SampleDesc.Count = m_struct.count;
	m_sd.SampleDesc.Quality = m_struct.quality;
	m_sd.Windowed = m_struct.windowed;
}

CSwapChain * CSwapChain::getInstance()
{
	if (m_pSCInstance == nullptr)
	{
		m_pSCInstance = new CSwapChain();
	}
	return m_pSCInstance;
}
