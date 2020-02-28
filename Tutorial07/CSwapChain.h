#pragma once
#include "Define.h"

struct SwapChainStruct
{
	unsigned int bufferCount;
	unsigned int W;
	unsigned int H;
	FORMAT format;
	unsigned int refreshNumerator;
	unsigned int refreshDenominator;
	unsigned int bufferUsage;
#ifdef D3D11
	HWND outputWND;
#endif
	unsigned int count;
	unsigned int quality;
	bool windowed;
};

class CSwapChain
{
public:

	void init(SwapChainStruct s);
	static CSwapChain * getInstance();
	void operator = (CSwapChain const&) = delete;	

#ifdef D3D11
	IDXGISwapChain * m_pSwapChain;
	DXGI_SWAP_CHAIN_DESC m_sd;
#endif // D3D11

private:
	CSwapChain();
	static CSwapChain * m_pSCInstance;
};