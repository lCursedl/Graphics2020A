#include "CRenderTargetView.h"

CRenderTargetView::CRenderTargetView()
{
#ifdef D3D11
	m_pRTV = NULL;
#endif
}

void CRenderTargetView::init(RenderTargetViewStruct S)
{

}