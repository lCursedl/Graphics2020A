#include "CViewport.h"

CViewport::CViewport()
{
}

void CViewport::init(ViewportStruct V)
{
#ifdef D3D11
	m_Viewport.Width = V.W;
	m_Viewport.Height = V.H;
	m_Viewport.MinDepth = V.minDepth;
	m_Viewport.MaxDepth = V.maxDepth;
	m_Viewport.TopLeftX = V.topLeftX;
	m_Viewport.TopLeftY = V.topLeftY;
#endif
}