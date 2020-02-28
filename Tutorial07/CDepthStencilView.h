#pragma once
#include "Define.h"

struct DepthStencilViewStruct
{
	FORMAT format;
	DSV_DIMENSION viewDimension;
	unsigned int flags;
	union
	{
		TEX1D_DSV textureID;
		TEX1D_ARRAY_DSV texture1DArray;
		TEX2D_DSV texture2D;
		TEX2D_ARRAY_DSV texture2DArray;
		TEX2DMS_DSV texture2DMS;
		TEX2DMS_ARRAY_DSV texture2DMSArray;
	};
};

class CDepthStencilView
{
public:
	CDepthStencilView();
	void init(DepthStencilViewStruct D, FORMAT F);
#ifdef D3D11
	ID3D11DepthStencilView * m_pDepthStencilView;
	D3D11_DEPTH_STENCIL_VIEW_DESC m_Desc;
#endif // D3D11

};