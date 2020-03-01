#include "CDepthStencilView.h"

CDepthStencilView::CDepthStencilView()
{
#ifdef D3D11
	m_pDepthStencilView = NULL;
#endif // D3D11
}

void CDepthStencilView::init(DepthStencilViewStruct D, FORMAT F)
{
#ifdef D3D11
	ZeroMemory(&m_pDepthStencilView, sizeof(m_pDepthStencilView));
	m_Desc.Format = (DXGI_FORMAT)F;
	m_Desc.ViewDimension = (D3D11_DSV_DIMENSION)D.viewDimension;
	m_Desc.Texture2D.MipSlice = D.texture2D.mipSlice;
#endif // D3D11
}

void * CDepthStencilView::getDSV()
{
#ifdef D3D11
	return m_pDepthStencilView;
#else
	return nullptr;
#endif // D3D11	
}
