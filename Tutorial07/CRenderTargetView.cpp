#include "CRenderTargetView.h"

CRenderTargetView::CRenderTargetView()
{
#ifdef D3D11
	m_pRTV = NULL;
#endif
}

void CRenderTargetView::init(RenderTargetViewStruct S)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
	m_Desc.Format = (DXGI_FORMAT)S.format;
	m_Desc.ViewDimension = (D3D11_RTV_DIMENSION)S.viewDimension;
	m_Desc.Texture2D.MipSlice = S.texture2D.mipSlice;
}

void * CRenderTargetView::getRTV()
{
#ifdef D3D11
	return m_pRTV;
#else
	return nullptr;
#endif
}
