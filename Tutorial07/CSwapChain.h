#pragma once
#include "Define.h"

struct SwapChainStruct
{
#ifdef D3D11
	UINT bufferCount;
	UINT W;
	UINT H;
	DXGI_FORMAT format;
	UINT refreshNumerator;
	UINT refreshDenominator;
	DXGI_USAGE bufferUsage;
	HWND outputWND;
	UINT count;
	UINT quality;
	BOOL windowed;
#endif // D3D11
};

class CSwapChain
{
public:

	SwapChainStruct m_struct;

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