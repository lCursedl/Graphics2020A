#include "CDepthStencilView.h"

CDepthStencilView::CDepthStencilView()
{
#ifdef D3D11
	m_pDepthStencilView = NULL;
#endif // D3D11
}

void CDepthStencilView::init(DepthStencilViewStruct D, DXGI_FORMAT F)
{
	ZeroMemory(&m_pDepthStencilView, sizeof(m_pDepthStencilView));
	m_Desc.Format = F;
	m_Desc.ViewDimension = (D3D11_DSV_DIMENSION)D.viewDimension;
	m_Desc.Texture2D.MipSlice = D.texture2D.mipSlice;
}